#pragma once
#include "Message/MessageRecv.h"
#include "Message/MessageSend.h"

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

    using AdapterCallBack = std::function<bool(MessageRecv*, MessageSend*)>;

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

        void receiveMsg(MessageRecv* msg);
        bool sendMsg(MessageSend* msg);

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
} // namespace scli