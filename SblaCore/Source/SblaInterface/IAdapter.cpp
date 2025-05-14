#include "SblaInterface/IAdapter.h"

namespace sbla
{
	IAdapter::~IAdapter() {}

	void IAdapter::run()
	{
		m_Running		   = true;

		m_ReceiveThread	   = std::thread(&IAdapter::receive, this);
		m_SendThread	   = std::thread(&IAdapter::send, this);
		m_MsgProcessThread = std::thread(&IAdapter::m_MsgProcess, this);
	}
	void IAdapter::pushMsg(MessageRecv* recv)
	{
		MessageRecv* msgCopy = new MessageRecv(*recv);

		std::lock_guard<std::mutex> lock(m_ReceiveMutex);
		m_ReceiveQueue.push(msgCopy);
	}
	bool IAdapter::pullMsg(MessageSend* send)
	{
		std::lock_guard<std::mutex> lock(m_SendMutex);
		if (m_SendQueue.empty()) return false;

		MessageSend* msgData = m_SendQueue.front();
		m_SendQueue.pop();
		*send = *msgData;

		delete msgData;
		return true;
	}

	void IAdapter::bindCallBack(AdapterCallBack cb) { m_MsgProcessCallBack = cb; }

	bool IAdapter::isRunning() { return m_Running; }

	void IAdapter::exit()
	{
		m_Running = false;
		onExit();

		m_ReceiveThread.join();
		m_MsgProcessThread.join();
		m_SendThread.join();
	}

	void IAdapter::m_MsgProcess()
	{
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
	}

} // namespace sbla