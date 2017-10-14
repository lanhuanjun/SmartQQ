#include "qq_poll.h"

void qq::Poll::StartPoll(QQSession &session, bool receiveMessageListener(bool hasMessage, ReceiveMessage &receiveMessage))
{
	HttpClient *PollClient = new HttpClient();

	if (nullptr == PollClient) {
		return;
	}
	const int RE_CONNECT = 5;
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
			if (re_connet_count > RE_CONNECT) {
				break;
			}
			re_connet_count++;
		}
		auto response = PollClient->GetResponse();
		if (200 != response->m_code)
		{
			LOG(ERROR) << "QQTemp::GetDiscusDetailInfo request error.code:" << response->m_code;
			if (re_connet_count > RE_CONNECT) {
				break;
			}
			re_connet_count++;
		}
		LOG(DEBUG) << "QQTemp::GetDiscusDetailInfo. json:" << response->m_data;

		Json::Reader reader;
		Json::Value root;

		if (!reader.parse(response->m_data, root)) {
			if (re_connet_count > RE_CONNECT) {
				break;
			}
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
