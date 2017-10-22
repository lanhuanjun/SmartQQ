//
// Created by lan on 16-9-7.
//

#include <sstream>
#include "qq_temp.h"

qq::QQTemp::QQTemp(HttpClient * CLIENT)
	:CLIENT(CLIENT) 
{
	
}

qq::QQTemp::~QQTemp() {}


bool qq::QQTemp::GetOnLineBuddies(QQSession &session, std::list <FriendOnLine> &onlines) {
    std::string url = "http://d1.web2.qq.com/channel/get_online_buddies2?vfwebqq=" + session["vfwebqq"] +
                 "&CLIENTid=53999199&psessionid=" + session["psessionid"] + "&t=1473239796826";
    CLIENT->SetUrl(url);
    CLIENT->SetTempHeader(Header("Host", "d1.web2.qq.com"));
    CLIENT->SetTempHeader(Header("Referer", "http://d1.web2.qq.com/proxy.html?v=20151105001&callback=1&id=2"));

    if (!CLIENT->Execute(HttpClient::GET)) {
		LOG(ERROR) << "QQTemp::GetOnLineBuddies request error.url:" << url;
        return false;
    }
    auto response = CLIENT->GetResponse();
	if (200 != response->m_code)
	{
		LOG(ERROR) << "QQTemp::GetOnLineBuddies request error.code:" << response->m_code << " url:"<<url;
		return false;
	}
	std::string str = response->m_data;
	LOG(DEBUG) << "QQTemp::GetOnLineBuddies. json:" << str;
    bool isOK = ParseOnLineBuddies(str, onlines);

    return isOK;
}

bool qq::QQTemp::ParseOnLineBuddies(std::string &json, std::list <qq::FriendOnLine> &onlines) {
    Json::Value root;
	if (!StringToJsonValue(root, json))
	{
		return false;
	}

    int retcode = root["retcode"].asInt();
    if (0 != retcode) {
        //请求不成功
        return false;
    }
    Json::Value result = root["result"];
    Json::Value item;
    int count = result.size();
    for (int i = 0; i < count; i++) {
        item = result[i];
        FriendOnLine friendOnLine;
        friendOnLine.id = item["uin"].asUInt64();
        friendOnLine.client_type = item["clienttype"].asInt();
        friendOnLine.status = item["status"].asString();
        onlines.push_back(friendOnLine);
    }
    return true;
}

bool qq::QQTemp::GetFriendQQNum(QQSession &session, uint64 uin, uint64 &qq_num) {

    std::stringstream ssm;
    ssm << "http://s.web2.qq.com/api/get_friend_uin2?tuin="
        << uin << "&type=1&vfwebqq=" + session["vfwebqq"] + "&t=1473240203103";
    CLIENT->SetUrl(ssm.str());
    CLIENT->SetTempHeader(Header("Host", "s.web2.qq.com"));
    CLIENT->SetTempHeader(Header("Referer", "http://s.web2.qq.com/proxy.html?v=20130916001&callback=1&id=1"));

    if (!CLIENT->Execute(HttpClient::GET)) {
		LOG(ERROR) << "QQTemp::GetFriendQQNum request error.url:" << ssm.str();
        return false;
    }
    auto response = CLIENT->GetResponse();
	if (200 != response->m_code)
	{
		LOG(ERROR) << "QQTemp::GetFriendQQNum request error.code:"<<response->m_code <<" url:" << ssm.str();
		return false;
	}
	LOG(DEBUG) << "QQTemp::GetFriendQQNum. json:" << response->m_data;

	Json::Value root;
	if (!StringToJsonValue(root, response->m_data))
	{
		return false;
	}
    int retcode = root["retcode"].asInt();
    if (0 != retcode) {
        //请求不成功
        return false;
    }
    Json::Value result = root["result"];
    qq_num = result["account"].asUInt64();
    return true;
}

bool qq::QQTemp::GetSingleLongNick(QQSession &session, uint64 uin, std::string &long_nick) {

    std::stringstream ssm;
    ssm << "http://s.web2.qq.com/api/get_single_long_nick2?tuin="
        << uin << "&type=1&vfwebqq=" + session["vfwebqq"] + "&t=1473240203103";
    CLIENT->SetUrl(ssm.str());
    CLIENT->SetTempHeader(Header("Host", "s.web2.qq.com"));
    CLIENT->SetTempHeader(Header("Referer", "http://s.web2.qq.com/proxy.html?v=20130916001&callback=1&id=1"));

    if (!CLIENT->Execute(HttpClient::GET)) {
		LOG(ERROR) << "QQTemp::GetSingleLongNick request error.url:" << ssm.str();
        return false;
    }
    auto response = CLIENT->GetResponse();
	if (200 != response->m_code)
	{
		LOG(ERROR) << "QQTemp::GetSingleLongNick request error.code:" << response->m_code << " url:" << ssm.str();
		return false;
	}
	LOG(DEBUG) << "QQTemp::GetSingleLongNick. json:" << response->m_data;

	Json::Value root;
	if (!StringToJsonValue(root, response->m_data))
	{
		return false;
	}
    int retcode = root["retcode"].asInt();
    if (0 != retcode) {
        //请求不成功
        return false;
    }
    Json::Value result = root["result"][0];
    long_nick = result["lnick"].asString();
    return true;
}

bool qq::QQTemp::GetSelfInfo(QQSession &session, QQInfo &qi) {

    std::string url = "http://s.web2.qq.com/api/get_self_info2?t=1473237774012";
    CLIENT->SetUrl(url);
    CLIENT->SetTempHeader(Header("Host", "s.web2.qq.com"));
    CLIENT->SetTempHeader(Header("Referer", "http://s.web2.qq.com/proxy.html?v=20130916001&callback=1&id=1"));

    if (!CLIENT->Execute(HttpClient::GET)) {
		LOG(ERROR) << "QQTemp::GetSelfInfo request error.url:" << url;
        return false;
    }
	auto response = CLIENT->GetResponse();
	if (200 != response->m_code)
	{
		LOG(ERROR) << "QQTemp::GetSelfInfo request error.code:" << response->m_code << " url:" << url;
		return false;
	}
	LOG(DEBUG) << "QQTemp::GetSelfInfo. json:" << response->m_data;
	std::string json = response->m_data;
    return ParseQQInfo(json, qi);
}

bool qq::QQTemp::ParseQQInfo(const std::string &json, QQInfo &qi) {
	Json::Value root;
	if (!StringToJsonValue(root, json))
	{
		return false;
	}

    int retcode = root["retcode"].asInt();
    if (0 != retcode) {
        //请求不成功
        return false;
    }
    Json::Value result = root["result"];
    qi.id = result["uin"].asUInt64();
    qi.allow = result["allow"].asInt();
    qi.year = result["year"]["birthday"].asInt();
    qi.month = result["month"]["birthday"].asInt();
    qi.day = result["day"]["birthday"].asInt();
    qi.blood = result["blood"].asInt();
    qi.city = result["city"].asString();
    qi.collage = result["collage"].asString();
    qi.constel = result["constel"].asInt();
    qi.country = result["country"].asString();
    qi.email = result["email"].asString();
    qi.face = result["face"].asInt();
    qi.gender = result["gender"].asString();
    qi.homepage = result["homepage"].asString();
    qi.mobile = result["mobile"].asString();
    qi.nick = result["nick"].asString();
    qi.occupation = result["occupation"].asString();
    qi.personal = result["personal"].asString();
    qi.phone = result["phone"].asString();
    qi.province = result["province"].asString();
    qi.shengxiao = result["shengxiao"].asInt();
    qi.stat = result["stat"].asInt();
    qi.vip_info = result["vip_info"].asInt();
    return true;
}

bool qq::QQTemp::GetQQInfo(QQSession &session, uint64 uin, QQInfo &qi) {

    std::stringstream ssm;
    ssm << "http://s.web2.qq.com/api/get_friend_info2?tuin="
        << uin 
		<< "&vfwebqq=" << session["vfwebqq"] 
		<< "&CLIENTid=53999199&psessionid=" << session["psessionid"] 
		<< "&t=1473240203103";

    CLIENT->SetUrl(ssm.str());
    CLIENT->SetTempHeader(Header("Host", "s.web2.qq.com"));
    CLIENT->SetTempHeader(Header("Referer", "http://s.web2.qq.com/proxy.html?v=20130916001&callback=1&id=1"));

    if (!CLIENT->Execute(HttpClient::GET)) {
		LOG(ERROR) << "QQTemp::GetSelfInfo request error.url:" << ssm.str();
        return false;
    }
	auto response = CLIENT->GetResponse();
	if (200 != response->m_code)
	{
		LOG(ERROR) << "QQTemp::GetSelfInfo request error.code:" << response->m_code << " url:" << ssm.str();
		return false;
	}
	LOG(DEBUG) << "QQTemp::GetSelfInfo. json:" << response->m_data;
	std::string json = response->m_data;
    return ParseQQInfo(json, qi);
}

bool qq::QQTemp::GetGroupDetailInfo(QQSession &session, uint64 gcode, GroupDetailInfo &groupDetailInfo) {
    std::stringstream ssm;
    ssm << "http://s.web2.qq.com/api/get_group_info_ext2?gcode="
        << gcode << "&type=1&vfwebqq=" + session["vfwebqq"] + "&t=1473240203103";
    CLIENT->SetUrl(ssm.str());
    CLIENT->SetTempHeader(Header("Host", "s.web2.qq.com"));
    CLIENT->SetTempHeader(Header("Referer", "http://s.web2.qq.com/proxy.html?v=20130916001&callback=1&id=1"));

	if (!CLIENT->Execute(HttpClient::GET)) {
		LOG(ERROR) << "QQTemp::GetGroupDetailInfo request error.url:" << ssm.str();
		return false;
	}
	auto response = CLIENT->GetResponse();
	if (200 != response->m_code)
	{
		LOG(ERROR) << "QQTemp::GetGroupDetailInfo request error.code:" << response->m_code << " url:" << ssm.str();
		return false;
	}
	LOG(DEBUG) << "QQTemp::GetGroupDetailInfo. json:" << response->m_data;
	std::string json = response->m_data;

    return ParseGroupDetailInfo(json, groupDetailInfo);
}

bool qq::QQTemp::ParseGroupDetailInfo(std::string &json, GroupDetailInfo &groupDetailInfo) {
	Json::Value root;
	if (!StringToJsonValue(root, json))
	{
		return false;
	}

    int retcode = root["retcode"].asInt();
    if (0 != retcode) {
        //请求不成功
        return false;
    }
    Json::Value result = root["result"];
    Json::Value cards = result["cards"];
    Json::Value ginfo = result["ginfo"];
    Json::Value minfo = result["minfo"];
    Json::Value stats = result["stats"];
    Json::Value vipinfo = result["vipinfo"];
    Json::Value members = ginfo["members"];

    int count = members.size();
    for (int i = 0; i < count; ++i) {
        GroupMem groupMem;
        groupMem.uin = members[i]["muin"].asUInt64();
        groupMem.flag = members[i]["mflag"].asUInt();
        groupDetailInfo.members.insert(std::pair<uint64, GroupMem>(groupMem.uin, groupMem));
    }
    count = cards.size();
    for (int i = 0; i < count; ++i) {
        uint64 id = cards[i]["muin"].asUInt64();
        groupDetailInfo.members[id].card = cards[i]["card"].asString();
    }
    count = minfo.size();
    for (int i = 0; i < count; ++i) {
        uint64 id = minfo[i]["uin"].asUInt64();
        groupDetailInfo.members[id].nick = minfo[i]["nick"].asString();
        groupDetailInfo.members[id].province = minfo[i]["province"].asString();
        groupDetailInfo.members[id].gender = minfo[i]["gender"].asString();
        groupDetailInfo.members[id].contry = minfo[i]["contry"].asString();
        groupDetailInfo.members[id].city = minfo[i]["city"].asString();
    }
    count = stats.size();
    for (int i = 0; i < count; ++i) {
        uint64 id = stats[i]["uin"].asUInt64();
        groupDetailInfo.members[id].client_type = stats[i]["CLIENTtype"].asInt();
        groupDetailInfo.members[id].stat = stats[i]["stat"].asInt();
    }
    count = vipinfo.size();
    for (int i = 0; i < count; ++i) {
        uint64 id = vipinfo[i]["uin"].asUInt64();
        groupDetailInfo.members[id].vip_level = vipinfo[i]["vip_level"].asInt();
        groupDetailInfo.members[id].is_vip = vipinfo[i]["is_vip"].asInt();
    }

    groupDetailInfo.class_ = ginfo["class"].asInt();
    groupDetailInfo.code = ginfo["code"].asUInt64();
    groupDetailInfo.create_time = ginfo["createtime"].asUInt64();
    groupDetailInfo.face = ginfo["face"].asInt();
    groupDetailInfo.fingermemo = ginfo["fingermemo"].asString();
    groupDetailInfo.flag = ginfo["flag"].asUInt64();
    groupDetailInfo.id = ginfo["id"].asUInt64();
    groupDetailInfo.level = ginfo["level"].asInt();
    groupDetailInfo.memo = ginfo["memo"].asString();
    groupDetailInfo.name = ginfo["name"].asString();
    groupDetailInfo.option = ginfo["option"].asInt();
    groupDetailInfo.owner = ginfo["owner"].asUInt64();
    return true;
}

bool qq::QQTemp::GetDiscusDetailInfo(QQSession &session, uint64 did, DiscusDetailInfo &ddi) {
    std::stringstream ssm;
    ssm << "http://d1.web2.qq.com/channel/get_discu_info?did="
        << did << "&vfwebqq=" + session["vfwebqq"]
        << "&CLIENTid=53999199&psessionid=" << session["psessionid"] << "&t=1473239796826";
    CLIENT->SetUrl(ssm.str());

    CLIENT->SetTempHeader(Header("Host", "d1.web2.qq.com"));
    CLIENT->SetTempHeader(Header("Referer", "http://d1.web2.qq.com/proxy.html?v=20151105001&callback=1&id=2"));

	if (!CLIENT->Execute(HttpClient::GET)) {
		LOG(ERROR) << "QQTemp::GetDiscusDetailInfo request error.url:" << ssm.str();
		return false;
	}
	auto response = CLIENT->GetResponse();
	if (200 != response->m_code)
	{
		LOG(ERROR) << "QQTemp::GetDiscusDetailInfo request error.code:" << response->m_code << " url:" << ssm.str();
		return false;
	}
	LOG(DEBUG) << "QQTemp::GetDiscusDetailInfo. json:" << response->m_data;
	std::string json = response->m_data;
    return ParseDiscusDetailInfo(json, ddi);
}

bool qq::QQTemp::ParseDiscusDetailInfo(std::string &json, DiscusDetailInfo &ddi) {
	Json::Value root;
	if (!StringToJsonValue(root, json))
	{
		return false;
	}

    int retcode = root["retcode"].asInt();
    if (0 != retcode) {
        //请求不成功
        return false;
    }
    Json::Value result = root["result"];

    Json::Value info = result["info"];
    Json::Value memlist = info["memlist"];
    Json::Value meminfo = result["meminfo"];
    int count = memlist.size();
    for (int i = 0; i < count; ++i) {
        DiscusMem dm;
        dm.uin = memlist[i]["mem_uin"].asUInt64();
        dm.ruin = memlist[i]["ruin"].asUInt64();
        ddi.mems.insert(std::pair<uint64, DiscusMem>(dm.uin, dm));
    }
    count = meminfo.size();
    for (int i = 0; i < count; ++i) {
        uint64 id = meminfo[i]["uin"].asUInt64();
        ddi.mems[id].nick = meminfo[i]["nick"].asString();
    }
    ddi.name = info["discu_name"].asString();
    ddi.did = info["did"].asUInt64();
    return true;
}

bool qq::QQTemp::GetUserFace(QQSession &session, uint64 uin, std::string &data) {
    std::stringstream ssm;
    ssm << "http://face1.web.qq.com/cgi/svr/face/getface?cache=1&type=1&f=40&uin="
		<< uin << "&t=1473242771&vfwebqq=" + session["vfwebqq"];
    CLIENT->SetUrl(ssm.str());

    CLIENT->SetTempHeader(Header("Host", "face1.web.qq.com"));
    CLIENT->SetTempHeader(Header("Referer", "http://w.qq.com/"));

	if (!CLIENT->Execute(HttpClient::GET)) {
		LOG(ERROR) << "QQTemp::GetUserFace request error.url:" << ssm.str();
		return false;
	}
	auto response = CLIENT->GetResponse();
	if (200 != response->m_code)
	{
		LOG(ERROR) << "QQTemp::GetUserFace request error.code:" << response->m_code << " url:" << ssm.str();
		return false;
	}
	if (!response->m_data.empty())
	{
		data.assign(response->m_data);
		return true;
	}
    return false;
}

bool qq::QQTemp::ChangeStatus(QQSession &session, QQStatus status) {
    std::string str = "";
    switch (status) {
        case ONLINE:
            str = "online";
            break;
        case AWAY:
            str = "away";
            break;
        case CALLME:
            str = "callme";
            break;
        case BUSY:
            str = "busy";
            break;
        case SLIENT:
            str = "slient";
            break;
        case HIDDEN:
            str = "hidden";
            break;
        case OFFLINE:
            str = "offline";
            break;
    }
    std::string url = "http://d1.web2.qq.com/channel/change_status2?newstatus=" + str +
                 "&CLIENTid=53999199&psessionid=" + session["psessionid"] + "&t=1473239796826";
    CLIENT->SetUrl(url);
    CLIENT->SetTempHeader(Header("Host", "d1.web2.qq.com"));
    CLIENT->SetTempHeader(Header("Referer", "http://d1.web2.qq.com/proxy.html?v=20151105001&callback=1&id=2"));

    
	if (!CLIENT->Execute(HttpClient::GET)) {
		LOG(ERROR) << "QQTemp::ChangeStatus request error.url:" << url;
		return false;
	}
	auto response = CLIENT->GetResponse();
	if (200 != response->m_code)
	{
		LOG(ERROR) << "QQTemp::ChangeStatus request error.code:" << response->m_code << " url:" << url;
		return false;
	}
	LOG(DEBUG) << "QQTemp::GetDiscusDetailInfo. json:" << response->m_data;

	Json::Value root;
	if (!StringToJsonValue(root, response->m_data))
	{
		return false;
	}
    int retcode = root["retcode"].asInt();

    return 0 == retcode;
}

bool qq::QQTemp::SendOneMessage(QQSession &session, SendQQMessage sendMessage) {

    switch (sendMessage.GetMessageType()) {
        case USER:
            CLIENT->SetUrl("https://d1.web2.qq.com/channel/send_buddy_msg2");
            break;
        case GROUP:
            CLIENT->SetUrl("https://d1.web2.qq.com/channel/send_qun_msg2");
            break;
        case DISCUS:
            CLIENT->SetUrl("https://d1.web2.qq.com/channel/send_discu_msg2");
            break;
        case UNKNOW_TYPE:
            return false;
    }
    CLIENT->SetTempHeader(Header("Host", "d1.web2.qq.com"));
    CLIENT->SetTempHeader(Header("Origin", "http://d1.web2.qq.com"));
    CLIENT->SetTempHeader(Header("Referer", "http://d1.web2.qq.com/proxy.html?v=20151105001&callback=1&id=2"));

    CLIENT->SetPostField(Field("r", CLIENT->URLEncoded(sendMessage.PackageMessage())));

    if (!CLIENT->Execute(HttpClient::POST)) {
		LOG(ERROR) << "QQTemp::SendOneMessage request error. type:" << sendMessage.GetMessageType();
        return false;
    }
	auto response = CLIENT->GetResponse();
	if (200 != response->m_code)
	{
		LOG(ERROR) << "QQTemp::ChangeStatus request error.code:" << response->m_code << " type:" << sendMessage.GetMessageType();
		return false;
	}

	LOG(DEBUG) << "QQTemp::GetDiscusDetailInfo. json:" << response->m_data;

    Json::Value root;
	if (!StringToJsonValue(root, response->m_data))
	{
		return false;
	}
    int errCode = root["errCode"].asInt();

    return 0 == errCode;
}
