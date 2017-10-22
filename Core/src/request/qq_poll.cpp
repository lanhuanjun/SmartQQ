#include "qq_poll.h"

void qq::QQPoll::StartPoll(QQSession &session, bool receiveMessageListener(bool hasMessage, ReceiveMessage &receiveMessage))
{
	HttpClient *PollClient = new HttpClient();
	
	if (nullptr == PollClient) 
	{
		return;
	}
	if (!PollClient->Init())
	{
		return;
	}
	const int RE_CONNECT = 100;
	//设置失败重连次数
	int re_connet_count = 0;
	
	std::vector<Cookie> PollCookie;
	for(Cookie cookie : session)
	{
		if (cookie.first == "ptcz")
		{
			PollCookie.push_back(cookie);
		}
		else if (cookie.first == "p_skey")
		{
			PollCookie.push_back(cookie);
		}
		else if (cookie.first == "pt2gguin")
		{
			PollCookie.push_back(cookie);
		}
		else if (cookie.first == "pt4_token")
		{
			PollCookie.push_back(cookie);
		}
		else if (cookie.first == "skey")
		{
			PollCookie.push_back(cookie);
		}
		else if (cookie.first == "uin")
		{
			PollCookie.push_back(cookie);
		}
	}

	std::string r = "{\"ptwebqq\":\"" + session["ptwebqq"] + "\",\"CLIENTid\":53999199,\"psessionid\":\"" +
		session["psessionid"] + "\",\"key\":\"\"}";

	while (true) {
		PollClient->SetUrl("https://d1.web2.qq.com/channel/poll2");
		PollClient->SetTempHeader(Header("Host", "d1.web2.qq.com"));
		PollClient->SetTempHeader(Header("Origin", "http://d1.web2.qq.com"));
		PollClient->SetTempHeader(Header("Referer", "http://d1.web2.qq.com/proxy.html?v=20151105001&callback=1&id=2"));
		
		for(Cookie &cookie : PollCookie)
		{
			PollClient->SetSendCookies(cookie);
		}

		PollClient->SetPostField(Field("r", PollClient->URLEncoded(r)));

		if (!PollClient->Execute(HttpClient::POST)) {
			LOG(ERROR) << "QQTemp::Poll request error.url https://d1.web2.qq.com/channel/poll2";
			re_connet_count++;
		}

		if (re_connet_count > RE_CONNECT) {
			return;
		}

		auto response = PollClient->GetResponse();
		if (200 != response->m_code)
		{
			LOG(ERROR) << "QQPoll::StartPoll error.code:" << response->m_code;
			re_connet_count++;
		}
		LOG(DEBUG) << "QQPoll::StartPoll. json:" << response->m_data;

		Json::Value root;
		if (!StringToJsonValue(root, response->m_data))
		{
			re_connet_count++;
		}

		int retcode = root["retcode"].asInt();
		if (0 != retcode) {
			if (re_connet_count > RE_CONNECT) {
				break;
			}
			re_connet_count++;
		}
		Json::Value result = root["result"];
		Json::Value errMsg = root["errmsg"];

		ReceiveMessage receiveMessage;

		re_connet_count = 0;

		if (errMsg.isNull()) {
			receiveMessage.ParseMessage(result);
			receiveMessageListener(true, receiveMessage);
		}
		else {
			receiveMessageListener(false, receiveMessage);
		}
	}
}

void qq::QQPoll::StartPoll(HttpClient * CLIENT, QQSession & session, bool receiveMessageListener(bool hasMessage, ReceiveMessage &receiveMessage))
{
	HttpClient *PollClient = CLIENT;

	if (nullptr == PollClient)
	{
		return;
	}
	if (!PollClient->Init())
	{
		return;
	}
	const int RE_CONNECT = 100;
	//设置失败重连次数
	int re_connet_count = 0;
	while (true) {
		PollClient->SetUrl("https://d1.web2.qq.com/channel/poll2");
		PollClient->SetTempHeader(Header("Host", "d1.web2.qq.com"));
		PollClient->SetTempHeader(Header("Origin", "http://d1.web2.qq.com"));
		PollClient->SetTempHeader(Header("Referer", "http://d1.web2.qq.com/proxy.html?v=20151105001&callback=1&id=2"));

		std::string r = "{\"ptwebqq\":\"" + session["ptwebqq"] + "\",\"CLIENTid\":53999199,\"psessionid\":\"" +
			session["psessionid"] + "\",\"key\":\"\"}";

		PollClient->SetPostField(Field("r", PollClient->URLEncoded(r)));

		if (!PollClient->Execute(HttpClient::POST)) {
			LOG(ERROR) << "QQTemp::Poll request error.url https://d1.web2.qq.com/channel/poll2";
			re_connet_count++;
		}

		if (re_connet_count > RE_CONNECT) {
			return;
		}

		auto response = PollClient->GetResponse();
		if (200 != response->m_code)
		{
			LOG(ERROR) << "QQPoll::StartPoll error.code:" << response->m_code;
			re_connet_count++;
		}
		LOG(DEBUG) << "QQPoll::StartPoll. json:" << response->m_data;

		Json::Value root;
		if (!StringToJsonValue(root, response->m_data))
		{
			re_connet_count++;
		}

		int retcode = root["retcode"].asInt();
		if (0 != retcode) {
			if (re_connet_count > RE_CONNECT) {
				break;
			}
			re_connet_count++;
		}
		Json::Value result = root["result"];
		Json::Value errMsg = root["errmsg"];

		ReceiveMessage receiveMessage;

		re_connet_count = 0;

		if (errMsg.isNull()) {
			receiveMessage.ParseMessage(result);
			receiveMessageListener(true, receiveMessage);
		}
		else {
			receiveMessageListener(false, receiveMessage);
		}
	}
}
