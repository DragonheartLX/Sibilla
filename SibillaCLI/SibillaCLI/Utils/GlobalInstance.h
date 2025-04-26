#pragma once
#include "Utils/Singleton.h"

namespace scli
{
    class GlobalInstance: public Singleton<GlobalInstance>
    {
    public:
        ~GlobalInstance();

        std::mutex consoleMutex;
    private:
        friend class Singleton<GlobalInstance>;
        GlobalInstance();
    };
}