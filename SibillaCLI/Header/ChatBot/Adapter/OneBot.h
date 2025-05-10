#pragma once
#include "ChatBot/Adapter.h"
#include "Utils/Utils.h"

#include <curl/curl.h>

namespace scli
{
    // struct MetaMessage
    // {
    //     int64_t time;
    //     int64_t selfId;
    //     std::string post_type;
    //     std::string meta_event_type;
    //     std::string sub_type;
    // };

    class OneBot: public Adapter
    {
    public:
        OneBot();
        ~OneBot();

        void receive() override;
        void send() override;

        void msgCallBack();

    private:
        CURLM* m_Curlm = nullptr;
        CURL* m_Curl = nullptr;

        CurlWSStruct m_WSData;
    };
} // namespace scli