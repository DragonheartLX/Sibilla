#pragma once
#include <thread>

namespace scli
{
    class Adapter
    {
    public:
        Adapter();
        virtual ~Adapter();

        virtual bool init() = 0;
        virtual void run() = 0;
        virtual void exit() = 0;
    private:
        std::thread m_RunThread;
    };
}