#ifndef __QQ_POLL_INCLUDE__
#define __QQ_POLL_INCLUDE__
#include "../net/http_client.h"
#include "../set/qq_info.h"
#include "../set/qq_message.h"

namespace qq
{
	class QQPoll
	{
	public:
		void StartPoll(QQSession &session, bool receiveMessageListener(bool hasMessage, ReceiveMessage &receiveMessage));
	};
};

#endif // __QQ_POLL_INCLUDE__

