#pragma once
#include <functional>
#include <mutex>
#include <queue>
#include <string>

#include "Macros.h"
#include "Message/MessageRecv.h"
#include "Message/MessageSend.h"

#ifdef SBL_PLATFORM_WINDOWS
	#ifdef SBL_BUILD_SHARED
		#ifdef SBL_BUILD_DLL
			#define SBL_API __declspec(dllexport)
		#else
			#define SBL_API __declspec(dllimport)
		#endif
	#else
		#define SBL_API
	#endif
#else
	#define SBL_API
#endif

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
	};

	class ChatBot
	{
	public:
		ChatBot()															  = default;
		virtual ~ChatBot()													  = default;

		virtual bool msgProcessCallBack(MessageRecv* recv, MessageSend* send) = 0;
	};
} // namespace scom

// Export entry point

#ifdef __cplusplus
extern "C"
{
#endif

#ifdef SBL_ADAPTER_BUILD
	SBL_API scom::Adapter* createAdapter(scom::AdapterInfo* info);
#endif

#ifdef SBL_CHATBOT_BUILD
	SBL_API scom::ChatBot* createChatBot(scom::ChatBotInfo* info);
#endif

#ifdef __cplusplus
}
#endif