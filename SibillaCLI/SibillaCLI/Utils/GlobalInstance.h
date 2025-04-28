#pragma once
#include "Utils/Singleton.h"

namespace scli
{
    class GlobalInstance: public Singleton<GlobalInstance>
    {
    public:
        ~GlobalInstance();

    private:
        friend class Singleton<GlobalInstance>;
        GlobalInstance();
    };
}