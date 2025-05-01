#pragma once
#include "Message.h"
#include "ChatBot/Adapter.h"

#include <thread>

namespace scli
{
    class ChatBot
    {
    public:
        ChatBot() = delete;
        ChatBot(AdapterType adapter);
        ~ChatBot();
        
        void run();

        bool msgProcessCallBack(Message* recv, Message* send);
    private:
        Adapter* m_Adapter = nullptr;

        std::thread m_CoreThread;
    };
}