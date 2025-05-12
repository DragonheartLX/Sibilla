#pragma once
#include <atomic>
#include <functional>
#include <mutex>
#include <queue>
#include <thread>

#include "SblaCore/Message/MessageRecv.h"
#include "SblaCore/Message/MessageSend.h"

namespace sbla
{
	// ==========
	// Adapter
	// ==========

	using AdapterCallBack = std::function<bool(MessageRecv*, MessageSend*)>;

	// Adapter Interface
	class IAdapter
	{
	public:
		IAdapter() = default;
		virtual ~IAdapter();

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
} // namespace sbla