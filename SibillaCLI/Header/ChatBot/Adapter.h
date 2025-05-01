#pragma once
#include "Message.h"

#include <queue>
#include <mutex>
#include <thread>
#include <functional>

namespace scli
{
    enum class AdapterType
    {
        OneBot,
        Count
    };

    using AdapterCallBack = std::function<bool(Message*, Message*)>;

    class Adapter
    {
    public:
        Adapter();
        virtual ~Adapter();
        
        virtual void receive() = 0;
        virtual void send() = 0;
        
        void run();
        bool isRunning();
        void bindCallBack(AdapterCallBack cb);

        void receiveMsg(Message* msg);
        bool sendMsg(Message* msg);

    private:
        std::atomic<bool> m_Running;

        std::mutex m_ReceiveMutex;
        std::mutex m_SendMutex;

        std::queue<Message*> m_ReceiveQueue;
        std::queue<Message*> m_SendQueue;
        
        AdapterCallBack m_MsgProcessCallBack;
        
        std::thread m_ReceiveThread;
        std::thread m_SendThread;
        std::thread m_MsgProcessThread;
    };
}