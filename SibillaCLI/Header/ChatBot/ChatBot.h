#pragma once
#include "Message/MessageRecv.h"
#include "Message/MessageSend.h"
#include "ChatBot/Adapter.h"

// #include <thread>

namespace scli
{
	class ChatBot
	{
	public:
		ChatBot() = delete;
		ChatBot(AdapterType adapter);
		~ChatBot();

		void run();

		bool msgProcessCallBack(MessageRecv* recv, MessageSend* send);

	private:
		Adapter* m_Adapter = nullptr;

		// std::thread m_CoreThread;
	};
} // namespace scli