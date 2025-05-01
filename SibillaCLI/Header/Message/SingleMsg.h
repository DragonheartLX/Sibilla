#pragma once
#include <string>

namespace scli
{
    enum class SingleMsgType
    {
        Null,
        Text,
        Image,
        Reply,
        At,
        File
    };

    struct SingleMsg
    {
        SingleMsgType type = SingleMsgType::Null;
        std::string data = "";
    };
}