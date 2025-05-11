#include "SibillaCommon.h"

#include <functional>
#include <memory>
#include <mutex>
#include <thread>

#include "Logger.h"
#include "Message/MessageSend.h"

namespace scom
{
	Adapter::~Adapter()
	{
		m_Running = false;
		m_ReceiveThread.join();
		m_MsgProcessThread.join();
		m_SendThread.join();
	}

	void Adapter::run()
	{
		m_ReceiveThread	   = std::thread(std::bind(&Adapter::receive, this));
		m_SendThread	   = std::thread(std::bind(&Adapter::send, this));
		m_MsgProcessThread = std::thread([this]() {
			MessageRecv* recv = nullptr;

			while (isRunning())
			{
				m_ReceiveMutex.lock();
				if (!m_ReceiveQueue.empty())
				{
					recv = m_ReceiveQueue.front();
					m_ReceiveQueue.pop();
					m_ReceiveMutex.unlock();

					MessageSend* send = new MessageSend();
					if (m_MsgProcessCallBack(recv, send))
					{
						m_SendMutex.lock();
						m_SendQueue.push(send);
						m_SendMutex.unlock();
					}

					delete recv;
					continue;
				}
				m_ReceiveMutex.unlock();
			}
		});
	}
	void Adapter::pushMsg(MessageRecv* recv)
	{
		MessageRecv* msgCopy = new MessageRecv(*recv);

		std::lock_guard<std::mutex> lock(m_ReceiveMutex);
		m_ReceiveQueue.push(msgCopy);
	}
	bool Adapter::pullMsg(MessageSend* send)
	{
		std::lock_guard<std::mutex> lock(m_SendMutex);
		if (m_SendQueue.empty()) return false;

		MessageSend* msgData = m_SendQueue.front();
		m_SendQueue.pop();
		*send = *msgData;

		delete msgData;
		return true;
	}

	void Adapter::bindCallBack(AdapterCallBack cb) { m_MsgProcessCallBack = cb; }

	bool Adapter::isRunning() { return m_Running; }

	void Adapter::pullLog(LogInfo* info)
	{
		std::shared_ptr<Logger> log = Logger::getInstance();
		log->pull(info);
	}

	void ChatBot::pullLog(LogInfo* info)
	{
		std::shared_ptr<Logger> log = Logger::getInstance();
		log->pull(info);
	}
} // namespace scom