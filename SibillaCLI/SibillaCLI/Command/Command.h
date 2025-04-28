#pragma once
#include "Utils/Singleton.h"

namespace scli
{
    class Command: public Singleton<Command>
    {
    public:
        ~Command();

    private:

        friend class Singleton<Command>;
        Command();
    };
}