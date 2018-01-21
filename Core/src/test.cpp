#include "qq_control.h"
#include "core_main.h"

#include <fstream>
using namespace std;
using namespace qq;

bool SaveImg(const string & data)
{
	ofstream file_stream("qrc.png", std::ios::binary|ios::trunc);
	file_stream << data;
	file_stream.flush();
	file_stream.close();
	return true;
}

std::string m_url;

void Listener(QRC_Code code, std::string msg)
{
	std::cout << msg << std::endl;
	switch (code)
	{
	case qq::SUCCESS:
		m_url.assign(msg);
		break;
	case qq::INVALID:
		break;
	case qq::VALID:
		break;
	case qq::SCAN:
		break;
	case qq::UNKNOW_ERROR:
		break;
	default:
		break;
	}
}
bool receiveMessageListener(bool hasMessage, ReceiveMessage &receiveMessage)
{
	if (hasMessage)
	{
		MessageContent content = receiveMessage.GetQQMessage();
		for (auto iter = content.msg.begin(); iter != content.msg.end();++iter)
		{
			//LOG(DEBUG) <<"type:"<<iter->first<<" msg:" <<iter->second;
			std::cout << "type:" << iter->first << " msg:" << iter->second;
		}
	}
	return true;
}
int main(int argc, char* argv[])
{
	core_global_init(argc, argv);
	
	QQControl ctrl;
	std::string img;
	if (!ctrl.GetQRCImg(img))
	{
		perror("can not get img");
		return 0;
	}
	SaveImg(img);
	if (!ctrl.LoginQQ(Listener))
	{
		perror("listener error.");
		return 0;
	}
	if (!ctrl.GetControlNeed(m_url))
	{
		perror("GetControlNeed error.");
		return 0;
	}

	FriendBaseInfo friends;
	ctrl.GetUserFriends(friends);
	GroupMap groupMap;
	ctrl.GetGroupNameList(groupMap);
	DiscusMap discuss;
	ctrl.GetDicusList(discuss);
	RecentMap recents;
	ctrl.GetRecentList(recents);
	std::list<FriendOnLine> onlines;
	ctrl.GetOnLineBuddies(onlines);
	QQInfo self_info;
	ctrl.GetSelfInfo(self_info);
	ctrl.ChangeStatus(QQStatus::BUSY);
	//ctrl.GetFriendQQNum(uint64 uin);
	ctrl.StartPoll(receiveMessageListener);

	core_global_clean_up();
	return 0;
}