//
// Created by lan on 16-9-2.
//
#include "qq_login.h"
#include "../other/base.h"

#include <chrono>
#include <thread>

qq::QQLogin::QQLogin(qq::HttpClient *client)
	:CLIENT(client)
{
}
qq::QQLogin::~QQLogin() {}

bool qq::QQLogin::GetLoginSig() {
	std::string url = "https://ui.ptlogin2.qq.com/cgi-bin/login?daid=164&target=self&style=16&mibao_css=m_webqq&appid=501004106&enable_qlogin=0&no_verifyimg=1&s_url=http%3A%2F%2Fw.qq.com%2Fproxy.html&f_url=loginerroralert&strong_login=1&login_state=10&t=20131024001";
    CLIENT->SetUrl(url);
    CLIENT->SetTempHeader(Header("Host","ui.ptlogin2.qq.com"));
    CLIENT->SetTempHeader(Header("Referer","http://w.qq.com/"));
    CLIENT->SetTempHeader(Header("Upgrade-Insecure-Requests","1"));
    if(!CLIENT->Execute(HttpClient::GET)){
		LOG(ERROR) << "QQLogin::GetLoginSig Execute error. url" << url;
        return false;
    }
	auto res = CLIENT->GetResponse();
    for(auto item:res->m_cookies){
        if(item.first == "pt_login_sig"){
            m_session.insert(item);
			LOG(INFO) << "QQLogin::GetLoginSig succ." << item.second;
			break;
        }
    }
    return true;
}

bool qq::QQLogin::GetQRC(std::string &store) {
	std::string url = "https://ssl.ptlogin2.qq.com/ptqrshow?appid=501004106&e=0&l=M&s=5&d=72&v=4&t=0.6176311824459054";
    CLIENT->SetUrl(url);
    CLIENT->SetTempHeader(Header("Host","ssl.ptlogin2.qq.com"));
    CLIENT->SetTempHeader(Header("Referer","https://ui.ptlogin2.qq.com/cgi-bin/login?daid=164&target=self&style=16&mibao_css=m_webqq&appid=501004106"
            "&enable_qlogin=0&no_verifyimg=1&s_url=http%3A%2F%2Fw.qq.com%2Fproxy.html&f_url=loginerroralert&strong_login"
            "=1&login_state=10&t=20131024001"));
    if(!CLIENT->Execute(HttpClient::GET)){
		LOG(ERROR) << "QQLogin::GetLoginSig Execute error. url" << url;
        return false;
    }
	LOG(INFO) << "QQLogin::GetQRC succ.";
	auto res = CLIENT->GetResponse();
	store.assign(res->m_data);
	for (auto iter = res->m_cookies.begin();iter != res->m_cookies.end();++iter)
	{
		m_session.insert(*iter);
	}
	return true;
}

bool qq::QQLogin::CheckQRC(void listener(QRC_Code, std::string)) {
	auto qrsig = m_session.find("qrsig");
	if (qrsig == m_session.end())
	{
		LOG(ERROR) << "QQLogin::CheckQRC can not find qrsig" << std::endl;
	}

	std::stringstream ssm;

	

	ssm << "https://ssl.ptlogin2.qq.com/ptqrlogin?u1=http%3A%2F%2Fw.qq.com%2Fproxy.html&ptqrtoken="
		<<Hash33(qrsig->second) << "&ptredirect=0&h=1&t=1&g=1&from_ui=1&ptlang=2052&action=0-0-"
		<< time(NULL) << "&js_ver=10230&js_type=1&login_sig="
		<< m_session["pt_login_sig"]
		<< "P&pt_uistyle=40&aid=501004106&daid=164&mibao_css=m_webqq&";
	while (true){
        CLIENT->SetUrl(ssm.str());
        CLIENT->SetTempHeader(Header("Host","ssl.ptlogin2.qq.com"));
        CLIENT->SetTempHeader(Header("Referer","https://xui.ptlogin2.qq.com/cgi-bin/xlogin?daid=164&target=self&style=40&pt_disable_pwd=1&mibao_css=m_webqq&appid=501004106&enable_qlogin=0&no_verifyimg=1&s_url=http%3A%2F%2Fw.qq.com%2Fproxy.html&f_url=loginerroralert&strong_login=1&login_state=10&t=20131024001"));
		auto log_sig = m_session.find("pt_login_sig");
		if (log_sig != m_session.end())
		{
			CLIENT->SetSendCookies(*log_sig);
		}
		auto qrsig = m_session.find("qrsig");
		if (qrsig != m_session.end())
		{
			CLIENT->SetSendCookies(*qrsig);
		}
        if(!CLIENT->Execute(HttpClient::GET)){
            LOG(ERROR)<< "QQLogin::CheckQRC error. url"<< ssm.str();
            return false;
        }
		auto res = CLIENT->GetResponse();
		if (200 != res->m_code)
		{
			LOG(ERROR) << "QQLogin::CheckQRC error. ";
			LOG(ERROR) << ssm.str();
			LOG(ERROR) << res->m_data;
			return false;
		}

		std::string response_data(res->m_data);
		std::string returnMsg;
        QRC_Code code = ParseCheckQRC(response_data,returnMsg);
        listener(code,returnMsg);
        if(SUCCESS == code){
            for(auto item:res->m_cookies){
				m_session.insert(item);
            }
            return true;
        }
        if(UNKNOW_ERROR == code || INVALID == code){
            return false;
        }
        //线程休息一秒
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }
}

qq::QRC_Code qq::QQLogin::ParseCheckQRC(const std::string &msg, std::string &returnMsg) {
    if(msg.empty()){
        return UNKNOW_ERROR;
    }
    //查看状态码
    int f_first = msg.find('\'');
    int f_second = msg.find('\'',f_first+1);
    std::string str_code = msg.substr(f_first+1,f_second-f_first-1);
    int retcode =  atoi(str_code.c_str());
    switch (retcode){
        case SUCCESS:
            f_first = msg.find('\'',15);
            f_second = msg.find('\'',f_first+1);
            returnMsg = msg.substr(f_first+1,f_second - f_first -1);
            return SUCCESS;
        case INVALID:
            returnMsg = "二维码已经失效,请重新加载!";
            return INVALID;
        case VALID:
            returnMsg = "二维码未失效,请扫描!";
            return VALID;
        case SCAN:
            returnMsg = "二维码认证中!";
            return SCAN;
        default:
            returnMsg = "未知错误!";
            return UNKNOW_ERROR;
    }
}

bool qq::QQLogin::CheckSig(std::string url) {
    CLIENT->SetUrl(url);
    CLIENT->SetTempHeader(Header("Host","ptlogin2.web2.qq.com"));
    CLIENT->SetTempHeader(Header("Upgrade-Insecure-Requests","1"));
    if(!CLIENT->Execute(HttpClient::GET)){
		LOG(ERROR) << "QQLogin::CheckSig error. url:" << url;
        return false;
    }
	auto res = CLIENT->GetResponse();
	LOG(INFO) << "QQLogin::CheckSig succ.code:" << res->m_code;
    for(auto item:res->m_cookies){
		LOG(INFO) << "QQLogin::CheckSig. cooke[" << item.first << ":" << item.second;
		m_session.insert(item);
    }
    return true;
}

bool qq::QQLogin::GetVFWebqq() {
	std::stringstream ssm;
	ssm << "http://s.web2.qq.com/api/getvfwebqq?ptwebqq="
		<< m_session["ptwebqq"]
        << "&clientid=53999199&psessionid=&t=1472870102086";
    CLIENT->SetUrl(ssm.str());
    CLIENT->SetTempHeader(Header("Host","s.web2.qq.com"));
    CLIENT->SetTempHeader(Header("Referer","http://s.web2.qq.com/proxy.html?v=20130916001&callback=1&id=1"));
    if(!CLIENT->Execute(HttpClient::GET)){
		LOG(ERROR) << "QQLogin::GetVFWebqq error. url:" << ssm.str();
        return false;
    }
	auto res = CLIENT->GetResponse();

	LOG(INFO) << "QQLogin::GetVFWebqq succ.code:" << res->m_code << " data:"<< res->m_data;

    Json::Value root;
    if(!StringToJsonValue(root,res->m_data)){
        return false;
    }
    int retcode = root["retcode"].asInt();
    if(0 != retcode){
        return false;
    }
    std::string vfwebqq = root["result"]["vfwebqq"].asString();
	LOG(INFO) << "QQLogin::GetVFWebqq succ. vfwebqq" << vfwebqq;
    m_session.insert(std::make_pair(std::string("vfwebqq"),vfwebqq));
    return true;
}

bool qq::QQLogin::Login() {
    CLIENT->SetUrl("http://d1.web2.qq.com/channel/login2");
    CLIENT->SetTempHeader(Header("Host","d1.web2.qq.com"));
    CLIENT->SetTempHeader(Header("Origin","http://d1.web2.qq.com"));
    CLIENT->SetTempHeader(Header("Referer","http://d1.web2.qq.com/proxy.html?v=20151105001&callback=1&id=2"));

    std::string r = "{\"ptwebqq\":\"\",\"clientid\":53999199,\"psessionid\":\"\",\"status\":\"online\"}";

	LOG(DEBUG) << "QQLogin::Login post: r = " << r;

    CLIENT->SetPostField(Field("r",CLIENT->URLEncoded(r)));

    if(!CLIENT->Execute(HttpClient::POST)){
		LOG(ERROR) << "QQLogin::Login execute error.";
        return false;
    }
	auto res = CLIENT->GetResponse();
	LOG(INFO) << "QQLogin::Login succ. size:" << res->m_data.size() << " data:" << res->m_data;
	Json::Value root;
	std::ofstream os("E:\\a.txt",std::ios::trunc|std::ios::binary);
	os << res->m_data;
	os.flush();
	os.close();
	
	if (!StringToJsonValue(root, res->m_data))
	{
		return false;
	}
    int retcode = root["retcode"].asInt();
    if(0 !=retcode){
        return false;
    }
    Json::Value result = root["result"];
    m_session.insert(std::make_pair(std::string("cip"), result["cip"].asString()));
    m_session.insert(std::make_pair(std::string("index"), result["index"].asString()));
    m_session.insert(std::make_pair(std::string("port"), result["port"].asString()));
    m_session.insert(std::make_pair(std::string("psessionid"), result["psessionid"].asString()));
    m_session.insert(std::make_pair(std::string("uin"), result["uin"].asString()));
    m_session.insert(std::make_pair(std::string("login_vfwebqq"), result["vfwebqq"].asString()));
    return true;
}

const qq::QQSession qq::QQLogin::GetSession()
{
	return m_session;
}




