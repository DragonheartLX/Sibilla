#include "ChatBot/Adapter.h"

namespace scli
{
    Adapter::Adapter()
    {
        m_Running = true;
    }

    Adapter::~Adapter()
    {
        m_Running = false;
        m_ReceiveThread.join();
        m_MsgProcessThread.join();
        m_SendThread.join();
    }

    void Adapter::run()
    {
        m_ReceiveThread = std::thread(std::bind(&Adapter::receive, this));
        m_SendThread = std::thread(std::bind(&Adapter::send, this));
        m_MsgProcessThread = std::thread([this]()
        {
            Message* msg = nullptr;

            while (isRunning())
            {
                m_ReceiveMutex.lock();
                if (!m_ReceiveQueue.empty())
                {
                    msg = m_ReceiveQueue.front();
                    m_ReceiveQueue.pop();
                    m_ReceiveMutex.unlock();

                    Message* send = new Message();
                    if(m_MsgProcessCallBack(msg, send))
                    {
                        m_SendMutex.lock();
                        m_SendQueue.push(send);
                        m_SendMutex.unlock();
                    }
                    
                    delete msg;
                    continue;
                }
                m_ReceiveMutex.unlock();
            }
        });
    }

    bool Adapter::isRunning()
    {
        return m_Running;
    }

    void Adapter::bindCallBack(AdapterCallBack cb)
    {
        m_MsgProcessCallBack = cb;
    }

    void Adapter::receiveMsg(Message* msg)
    {
        Message* msgCopy = new Message(msg);

        std::lock_guard<std::mutex> lock(m_ReceiveMutex);
        m_ReceiveQueue.push(msgCopy);
    }

    bool Adapter::sendMsg(Message* msg)
    {
        std::lock_guard<std::mutex> lock(m_SendMutex);
        if (m_SendQueue.empty())
            return false;

        Message* msgData = m_SendQueue.front();
        m_SendQueue.pop();
        msg = msgData;

        delete msgData;
        return true;
    }
}