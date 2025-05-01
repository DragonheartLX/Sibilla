#pragma once
#include "Utils/Singleton.h"
#include "ChatBot/ChatBot.h"

#include <list>

namespace scli
{
    class SibillaCLI:public Singleton<SibillaCLI>
    {
    public:
        ~SibillaCLI();

        bool run();
    public:
        bool m_IsRunning = true;

        std::list<ChatBot*> m_Bots;
        friend class Singleton<SibillaCLI>;
        SibillaCLI();
    };
}