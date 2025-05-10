#include "ChatBot/ChatBot.h"
#include "ChatBot/Adapter/OneBot.h"
#include "Console.h"

namespace scli
{
	ChatBot::ChatBot(AdapterType adapter)
	{
		switch (adapter)
		{
			case AdapterType::OneBot: m_Adapter = new class OneBot(); break;
			default: break;
		}

		if (m_Adapter != nullptr)
			m_Adapter->bindCallBack(std::bind(&ChatBot::msgProcessCallBack,
											  this, std::placeholders::_1,
											  std::placeholders::_2));
	}

	ChatBot::~ChatBot()
	{
		// m_CoreThread.join();
		delete m_Adapter;
	}

	void ChatBot::run() { m_Adapter->run(); }

	bool ChatBot::msgProcessCallBack(MessageRecv* recv, MessageSend* send)
	{
		for (SingleMsg sMsg : recv->msg)
		{
			if (sMsg.type == SingleMsgType::Text)
				Console::debug("Receive message: {0}", sMsg.data);

			if (sMsg.data == "测试")
			{
				send->groupId = recv->groupId;
				send->userId = recv->senderId;
				send->type = recv->type;
				send->msg.push_back(SingleMsg{SingleMsgType::Text, "测试回复"});
				return true;
			}
		}

		return false;
	}
} // namespace scli