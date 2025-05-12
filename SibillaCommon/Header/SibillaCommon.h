#pragma once
#include <functional>
#include <mutex>
#include <queue>
#include <string>
#include <thread>

#include "Message/MessageRecv.h"
#include "Message/MessageSend.h"

namespace scom
{
	// ==========
	// Adapter
	// ==========

	struct AdapterInfo
	{
		std::string name	= "";
		std::string version = "";
	};

	using AdapterCallBack = std::function<bool(MessageRecv*, MessageSend*)>;

	// Adapter Interface
	class Adapter
	{
	public:
		Adapter() = default;
		virtual ~Adapter();

		virtual void receive() = 0;
		virtual void send()	   = 0;

		void run();
		void pushMsg(MessageRecv* recv);
		bool pullMsg(MessageSend* send);

		void bindCallBack(AdapterCallBack cb);
		bool isRunning();

	private:
		std::atomic<bool> m_Running;

		std::mutex m_ReceiveMutex;
		std::mutex m_SendMutex;

		std::queue<MessageRecv*> m_ReceiveQueue;
		std::queue<MessageSend*> m_SendQueue;

		AdapterCallBack m_MsgProcessCallBack;

		std::thread m_ReceiveThread;
		std::thread m_SendThread;
		std::thread m_MsgProcessThread;
	};

	// ==========
	// ChatBot
	// ==========

	struct ChatBotInfo
	{
		std::string name	= "";
		std::string version = "";
		std::string adapter = "";
	};

	class ChatBot
	{
	public:
		ChatBot()															  = default;
		virtual ~ChatBot()													  = default;

		virtual bool msgProcessCallBack(MessageRecv* recv, MessageSend* send) = 0;
	};
} // namespace scom