#pragma once
#include "Utils/Singleton.h"

namespace scli
{
    class Command: public Singleton<Command>
    {
    public:
        ~Command();

        void run(bool isRunning);
        
        std::string inputCommand;
    private:

        friend class Singleton<Command>;
        Command();
    };
}