//
// Created by lan on 16-9-5.
//

#include "qq_contact.h"
#include "../other/base.h"

qq::QQContact::QQContact(HttpClient *CLIENT)
: CLIENT(CLIENT)
{
}
qq::QQContact::~QQContact() {
}

bool qq::QQContact::GetUserFriends(QQSession &session,FriendBaseInfo &friendBaseInfo) {
    CLIENT->SetUrl("http://s.web2.qq.com/api/get_user_friends2");
    CLIENT->SetTempHeader(Header("Host","s.web2.qq.com"));
    CLIENT->SetTempHeader(Header("Origin","http://s.web2.qq.com"));
    CLIENT->SetTempHeader(Header("Referer","http://s.web2.qq.com/proxy.html?v=20130916001&callback=1&id=1"));
    long qq_id = std::atol(session["uin"].c_str());

    std::string r = "{\"vfwebqq\":\""+session["vfwebqq"] +"\",\"hash\":\""+GetHash(qq_id,session["ptwebqq"])+"\"}";

    CLIENT->SetPostField(Field("r",CLIENT->URLEncoded(r)));

    if(!CLIENT->Execute(HttpClient::POST)){
		LOG(ERROR) << "QQLogin::GetUserFriends Execute error. r:" << r;
        return false;
    }
	auto response = CLIENT->GetResponse();
	if (200 != response->m_code)
	{
		LOG(ERROR) << "QQTemp::GetUserFriends request error.code:" << response->m_code;
		return false;
	}
	LOG(DEBUG) << "QQTemp::GetUserFriends. json:" << response->m_data;

	std::string json = response->m_data;
    bool isOK = PaserUserFriendsJson(json,friendBaseInfo.friendGroups,friendBaseInfo.friendInfos);

    return isOK;
}
bool qq::QQContact::PaserUserFriendsJson(const std::string &json, FriendGroupMap &friendGroups, FriendInfoMap &friendList) {
    Json::Value root;

    if(!StringToJsonValue(root,json)){
        //Json数据有误
        return false;
    }

    int retcode = root["retcode"].asInt();
    if(0 !=retcode){
        //请求不成功
        return false;
    }
    Json::Value result = root["result"];
    //获取所有分组的信息
    Json::Value categories = result["categories"];
    int count = categories.size();
    Json::Value item;
    for(int i = 0 ; i < count ; ++i){
        item = categories[i];
        FriendGroup friendGroup;
        friendGroup.index = item["index"].asInt();
        friendGroup.sort = item["sort"].asInt();
        friendGroup.name = item["name"].asString();
        friendGroups.insert(std::make_pair(friendGroup.index,friendGroup));
    }
    //获取好友所在分组
    Json::Value friends = result["friends"];
    count = friends.size();
    for(int i =0 ; i < count ; ++i){
        item = friends[i];
        FriendInfo friendInfo;
        friendInfo.id = item["uin"].asUInt64();
        friendInfo.group_index = item["categories"].asInt();
		friendList.insert(std::make_pair(friendInfo.id,friendInfo));
    }
    //获取好友的昵称,头像信息
    Json::Value infos = result["info"];
    count = infos.size();
    for(int i =0 ; i < count ; ++i){
        item = infos[i];
        uint64  id_t = item["uin"].asUInt64();
		friendList[id_t].face = item["face"].asInt();
		friendList[id_t].nick_name = item["nick"].asString();
    }
    //获取所有好友备注信息
    Json::Value mark_names = result["marknames"];
    count = mark_names.size();
    for(int i =0 ; i < count ; ++i){
        item = mark_names[i];
        uint64  id_t = item["uin"].asUInt64();
		friendList[id_t].mark_name = item["markname"].asString();
    }
    //获取所有好友vip信息
    Json::Value vips = result["vipinfo"];
    count = mark_names.size();
    for(int i =0 ; i < count ; ++i){
        item = mark_names[i];
        uint64  id_t = item["u"].asUInt64();
		friendList[id_t].vip_level = item["vip_level"].asInt();
        int vipCode = item["is_vip"].asInt();
		friendList[id_t].is_vip = (0 == vipCode?false:true);
    }
    return true;
}
const std::string qq::QQContact::GetHash(const long &uin, const std::string &ptwebqq) {
    int a[4] = {0};
    for (size_t i = 0; i < ptwebqq.length(); ++i) {
        a[i % 4] ^= ptwebqq[i];
    }
    char w[4] = {'E', 'C', 'O', 'K'};
    long d[4]={0};
    long lb = uin;
    d[0] = lb >> 24 & 255 ^ w[0];
    d[1] = lb >> 16 & 255 ^ w[1];
    d[2] = lb >> 8 & 255 ^ w[2];
    d[3] = lb & 255 ^ w[3];

    char a1[] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F'};
    char dl[16]={0};
    for (int i = 0; i < 8; ++i) {
        long t = (i % 2 == 0 ? a[i >> 1] : d[i >> 1]);
        dl[i * 2] = a1[(int) (t >> 4 & 15)];
        dl[i * 2 + 1] = a1[(int) (t & 15)];
    }

    return std::string(dl, 0, 16);
}

bool qq::QQContact::GetGroupNameList(QQSession &session,GroupMap &groupInfos) {
    CLIENT->SetUrl("http://s.web2.qq.com/api/get_group_name_list_mask2");
    CLIENT->SetTempHeader(Header("Host","s.web2.qq.com"));
    CLIENT->SetTempHeader(Header("Origin","http://s.web2.qq.com"));
    CLIENT->SetTempHeader(Header("Referer","http://s.web2.qq.com/proxy.html?v=20130916001&callback=1&id=1"));

    long qq_id = atol(session["uin"].c_str());

    std::string r = "{\"vfwebqq\":\""+session["vfwebqq"] +"\",\"hash\":\""+GetHash(qq_id, session["ptwebqq"])+"\"}";

    CLIENT->SetPostField(Field("r",CLIENT->URLEncoded(r)));

    if(!CLIENT->Execute(HttpClient::POST)){
		LOG(ERROR) << "QQLogin::GetGroupNameList Execute error. r:" << r;
        return false;
    }
	auto response = CLIENT->GetResponse();
	if (200 != response->m_code)
	{
		LOG(ERROR) << "QQTemp::GetGroupNameList request error.code:" << response->m_code;
		return false;
	}
	LOG(DEBUG) << "QQTemp::GetGroupNameList. json:" << response->m_data;

	std::string json = response->m_data;

    bool isOK = PaserGroupNameListJson(json,groupInfos);

    return isOK;
}

bool qq::QQContact::PaserGroupNameListJson(const std::string &json,GroupMap &groupInfos) {
	Json::Value root;

	if (!StringToJsonValue(root, json)) {
		//Json数据有误
		return false;
	}

    int retcode = root["retcode"].asInt();
    if(0 !=retcode){
        //请求不成功
        return false;
    }
    Json::Value namelist = root["result"]["gnamelist"];
    int count = namelist.size();
    Json::Value item;
    for(int i = 0 ;i < count ; ++i){
        item = namelist[i];
        GroupInfo groupInfo;
        groupInfo.id = item["gid"].asUInt64();
        groupInfo.code = item["code"].asUInt64();
        groupInfo.name = item["name"].asString();
        groupInfos.insert(std::make_pair(groupInfo.id,groupInfo));
    }
    return true;
}

bool qq::QQContact::GetDicusList(QQSession &session,DiscusMap &discusList) {

    std::string url = "http://s.web2.qq.com/api/get_discus_list?CLIENTid=53999199&psessionid="
                 +session["psessionid"]
                 +"&vfwebqq="
                 +session["vfwebqq"]
                 +"&t=1473237774012";
    CLIENT->SetUrl(url);
    CLIENT->SetTempHeader(Header("Host","s.web2.qq.com"));
    CLIENT->SetTempHeader(Header("Referer","http://s.web2.qq.com/proxy.html?v=20130916001&callback=1&id=1"));

    if(!CLIENT->Execute(HttpClient::GET)){
		LOG(ERROR) << "QQLogin::GetDicusList Execute error. url:" << url;
        return false;
    }
	auto response = CLIENT->GetResponse();
	if (200 != response->m_code)
	{
		LOG(ERROR) << "QQTemp::GetDiscusDetailInfo request error.code:" << response->m_code;
		return false;
	}
	LOG(DEBUG) << "QQTemp::GetDiscusDetailInfo. json:" << response->m_data;

	std::string json = response->m_data;

    bool isOK = PaserDiscusListJson(json,discusList);

    return isOK;
}

bool qq::QQContact::PaserDiscusListJson(const std::string &json,DiscusMap &discusInfos) {
	Json::Value root;

	if (!StringToJsonValue(root, json)) {
		//Json数据有误
		return false;
	}
    int retcode = root["retcode"].asInt();
    if(0 !=retcode){
        //请求不成功
        return false;
    }
    Json::Value namelist = root["result"]["dnamelist"];
    int count = namelist.size();
    Json::Value item;
    for(int i = 0 ;i < count ; ++i){
        item = namelist[i];
        DiscusDetailInfo di;
        di.did = item["did"].asUInt64();
        di.name = item["name"].asString();
        discusInfos.insert(std::make_pair(di.did,di));
    }
    return true;
}

bool qq::QQContact::GetRecentList(QQSession &session,RecentMap &recentList) {
    CLIENT->SetUrl("http://d1.web2.qq.com/channel/get_recent_list2");
    CLIENT->SetTempHeader(Header("Host","d1.web2.qq.com"));
    CLIENT->SetTempHeader(Header("Origin","http://d1.web2.qq.com"));
    CLIENT->SetTempHeader(Header("Referer","http://d1.web2.qq.com/proxy.html?v=20151105001&callback=1&id=2"));

    std::string r ="{\"vfwebqq\":\""+session["vfwebqq"] +"\",\"CLIENTid\":53999199,\"psessionid\":\""+session["psessionid"] +"\"}";

    CLIENT->SetPostField(Field("r",CLIENT->URLEncoded(r)));

    if(!CLIENT->Execute(HttpClient::POST)){
		LOG(ERROR) << "QQLogin::GetRecentList Execute error. r:" << r;
        return false;
    }

	auto response = CLIENT->GetResponse();
	if (200 != response->m_code)
	{
		LOG(ERROR) << "QQTemp::GetRecentList request error.code:" << response->m_code;
		return false;
	}
	LOG(DEBUG) << "QQTemp::GetRecentList. json:" << response->m_data;
	std::string json = response->m_data;
    bool isOK = PaserRecentListJson(json,recentList);

    return isOK;
}

bool qq::QQContact::PaserRecentListJson(const std::string &json,RecentMap &recentList) {
	Json::Value root;

	if (!StringToJsonValue(root, json)) {
		//Json数据有误
		return false;
	}
    int retcode = root["retcode"].asInt();
    if(0 !=retcode){
        //请求不成功
        return false;
    }
    Json::Value result = root["result"];
    int count = result.size();
    Json::Value item;
    for(int i = 0; i < count ; ++i){
        RecentItem ri;
        item = result[i];
        ri.id = item["uin"].asUInt64();
        ri.type = item["type"].asInt();
        ri.status = item["status"].asString();
        recentList.insert(std::make_pair(ri.id,ri));
    }
    return true;
}



