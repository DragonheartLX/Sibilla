#pragma once
#include "Utils/Singleton.h"

#include <toml++/toml.hpp>

namespace scli 
{
    class Config:public Singleton<Config>
    {
    public:
        ~Config();

        toml::table data;
    private:

        friend class Singleton<Config>;
        Config();
    };
}