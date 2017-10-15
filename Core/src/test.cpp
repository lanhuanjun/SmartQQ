#include "qq_control.h"
#include "core_main.h"

#include <fstream>
using namespace std;
using namespace qq;

bool SaveImg(const string & data)
{
	ofstream file_stream("E:\\qrc.png", std::ios::binary|ios::app);
	file_stream << data;
	file_stream.flush();
	file_stream.close();
	return true;
}

std::string m_url;

void Listener(QRC_Code code, std::string msg)
{
	printf("%s", msg.c_str());
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
		for (auto iter = receiveMessage.GetMessage().begin(); iter != receiveMessage.GetMessage().end();++iter)
		{
			LOG(DEBUG) <<"type:"<<iter->first<<" msg:" <<iter->second;
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
	ctrl.StartPoll(receiveMessageListener);

	core_global_clean_up();
	return 0;
}