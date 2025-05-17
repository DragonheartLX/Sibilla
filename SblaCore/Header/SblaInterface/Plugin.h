#pragma once
#include <atomic>
#include <functional>
#include <mutex>
#include <queue>
#include <string>
#include <thread>
#include <vector>

#include "SblaCore/DataProtocol/Action/Request.h"
#include "SblaCore/DataProtocol/Event/Event.h"
#include "SblaInterface/ILogger.h"

namespace sbla
{
	struct PluginInfo
	{
		ILogger* logger;

		std::string name;
		std::string version;
		std::string type;

		std::vector<std::string> platform;
		std::vector<std::string> chatbot;
	};

	using ChatbotCallBack = std::function<bool(Event*, Request*)>;

	class IPlatform
	{
	public:
		IPlatform()							   = default;
		virtual ~IPlatform()				   = default;

		virtual void onInit()				   = 0;
		virtual void inLoop()				   = 0;
		virtual void sendRequest(Request* req) = 0;
		virtual void onExit()				   = 0;

		void init();
		void run();
		void exit();

		void pushEvent(Event* event);

		void bindCallBack(ChatbotCallBack cb);

		bool isRunning();

	private:
		void m_HandleRequest();

		std::atomic<bool> m_Running;

		// error use std::mutex
		std::recursive_mutex m_EventMutex;
		std::queue<Event*> m_EventQueue;

		ChatbotCallBack m_EventProcessCallBack;

		std::thread m_MainThread;
		std::thread m_EventProcessThread;
	};

	class IChatbot
	{
	public:
		IChatbot()										 = default;
		virtual ~IChatbot()								 = default;

		virtual void onInit()							 = 0;
		virtual void onExit()							 = 0;

		virtual void onEnable()							 = 0;
		virtual void onDisable()						 = 0;

		virtual bool onEvent(Event* event, Request* req) = 0;
	};
} // namespace sbla