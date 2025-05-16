#include "SblaInterface/Plugin.h"

#include <thread>

#include "SblaCore/DataProtocol/Action/Request.h"

namespace sbla
{
	void IPlatform::init()
	{
		m_Running = false;
		onInit();
	}

	void IPlatform::run()
	{
		m_Running			 = true;
		m_MainThread		 = std::thread(&IPlatform::inLoop, this);
		m_EventProcessThread = std::thread(&IPlatform::m_HandleRequest, this);
	}

	void IPlatform::exit()
	{
		m_Running = false;
		onExit();

		m_MainThread.join();
		m_EventProcessThread.join();
	}

	void IPlatform::pushEvent(Event* event)
	{
		Event* copy = new Event(*event);

		m_EventMutex.lock();
		m_EventQueue.push(copy);
		m_EventMutex.unlock();
	}

	void IPlatform::bindCallBack(ChatbotCallBack cb) { m_EventProcessCallBack = cb; }

	bool IPlatform::isRunning() { return m_Running; }

	void IPlatform::m_HandleRequest()
	{
		while (isRunning())
		{
			m_EventMutex.lock();
			if (!m_EventQueue.empty())
			{
				Event* event = m_EventQueue.front();
				m_EventQueue.pop();
				m_EventMutex.unlock();

				Request* req = new Request;
				if (m_EventProcessCallBack(event, req)) sendRequest(req);

				delete req;
			}
		}
	}
} // namespace sbla