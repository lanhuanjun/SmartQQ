//
// Created by lan on 16-9-7.
//

#include "qq_control.h"
qq::QQControl::QQControl() {
	m_login = nullptr;
	m_contact = nullptr;
	m_temp = nullptr;
	m_poll = nullptr;
    CLIENT = new HttpClient();
    if(!CLIENT->Init()){
        throw ("can't init client");
    }
    CLIENT->SetDefaultHeader(Header("Accept","*/*"));
    CLIENT->SetDefaultHeader(Header("Accept-Encoding","gzip, deflate, sdch"));
    CLIENT->SetDefaultHeader(Header("Accept-Language","en-US,en;q=0.5"));
    CLIENT->SetDefaultHeader(Header("Connection","keep-alive"));
    CLIENT->SetDefaultHeader(Header("User-Agent","Mozilla/5.0 (X11; Linux x86_64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/52.0.2743.116 Safari/537.36"));

    this->m_login = new QQLogin(CLIENT);
}

qq::QQControl::~QQControl() {
    if(this->m_login){
        delete  m_login;
		m_login = nullptr;
    }
    if(this->m_contact){
        delete m_contact;
		m_contact = nullptr;
    }
    if(this->m_temp){
        delete m_temp;
		m_temp = nullptr;
    }
}

bool qq::QQControl::GetQRCImg(std::string & data) {
    if(!m_login){
        return false;
    }
    if(!m_login->GetLoginSig()){
        return nullptr;
    }
    m_login->GetQRC(data);
	data.clear();
    return m_login->GetQRC(data);
}

bool qq::QQControl::LoginQQ(void Listener(QRC_Code, std::string)) {
    if(!m_login){
        return false;
    }
    return m_login->CheckQRC(Listener);
}
bool qq::QQControl::GetControlNeed(std::string url) {
    if(!m_login->CheckSig(url)){
        return false;
    }
    if(!m_login->GetVFWebqq()){
        return false;
    }
    if(!m_login->Login()){
        return false;
    }
	m_session = m_login->GetSession();

    m_contact = new QQContact(CLIENT);
    m_temp = new QQTemp(CLIENT);
    return true;
}

bool qq::QQControl::GetUserFriends(qq::FriendBaseInfo &friendBaseInfo) {
    if(nullptr == m_contact){
        return false;
    }
	return m_contact->GetUserFriends(m_session, friendBaseInfo);
}

bool qq::QQControl::GetGroupNameList(GroupMap &groups) {
    if(nullptr == m_contact){
        return false;
    }
	return m_contact->GetGroupNameList(m_session, groups);
}

bool qq::QQControl::GetDicusList(DiscusMap &discuss) {
    if(nullptr == m_contact){
        return false;
    }

	return m_contact->GetDicusList(m_session, discuss);
}

bool qq::QQControl::GetRecentList(RecentMap &recents) {
    if(nullptr == m_contact){
        return false;
    }
	return m_contact->GetRecentList(m_session, recents);
}

bool qq::QQControl::GetOnLineBuddies(std::list <qq::FriendOnLine> &onlines) {
    if(nullptr == m_temp){
        return false;
    }
	return m_temp->GetOnLineBuddies(m_session,onlines);
}

uint64 qq::QQControl::GetFriendQQNum(uint64 uin) {
    if(nullptr == m_temp){
        return false;
    }

    uint64 qq_num = 0;

	if (!m_temp->GetFriendQQNum(m_session, uin, qq_num))
	{
		return 0;
	}
    return qq_num;
}

std::string qq::QQControl::GetSingleLongNick(uint64 uin) {
    if(nullptr == m_temp){
        return std::string();
    }
    std::string long_nick = "";
	if (!m_temp->GetSingleLongNick(m_session, uin, long_nick))
	{
		return std::string();
	}
    return long_nick;
}

bool qq::QQControl::GetSelfInfo(qq::QQInfo &qi) {
    if(nullptr == m_temp){
        return false;
    }
	return m_temp->GetSelfInfo(m_session, qi);
}

bool qq::QQControl::GetQQInfo(uint64 uin, QQInfo &qi) {
    if(nullptr == m_temp){
        return false;
    }
	return m_temp->GetQQInfo(m_session, uin, qi);
}

bool qq::QQControl::GetGroupDetailInfo(uint64 gcode, qq::GroupDetailInfo &groupDetailInfo) {
    if(nullptr == m_temp){
        return false;
    }
	return m_temp->GetGroupDetailInfo(m_session, gcode, groupDetailInfo);
}

bool qq::QQControl::GetDiscusDetailInfo(uint64 did, DiscusDetailInfo &ddi) {
    if(nullptr == m_temp){
        return false;
    }
	return m_temp->GetDiscusDetailInfo(m_session, did, ddi);
}

bool qq::QQControl::GetUserFace(uint64 uin, std::string &data) {
    if(nullptr == m_temp){
        return nullptr;
    }
	return m_temp->GetUserFace(m_session, uin, data);
}

bool qq::QQControl::ChangeStatus(qq::QQStatus status) {
    if(nullptr == m_temp){
        return false;
    }
	return m_temp->ChangeStatus(m_session, status);
}

bool qq::QQControl::StartPoll(bool receiveMessageListener(bool hasMessage,ReceiveMessage &receiveMessage)) {
	m_poll = new QQPoll();
	m_poll->StartPoll(m_session, receiveMessageListener);
	return true;
}

bool qq::QQControl::SendOneMessage(qq::SendMessage &sendMessage) {
    if(nullptr == m_temp){
        return false;
    }
	return m_temp->SendOneMessage(m_session, sendMessage);
}







