#include "ChatBot/Adapter.h"

namespace scli
{
    Adapter::Adapter()
    {
        m_RunThread = std::thread([this]()
        {
            this->run();
        });
    };
}