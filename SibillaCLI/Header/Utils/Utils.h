#pragma once

#include <string>
#include <mutex>
#include <functional>
#include <curl/curl.h>

namespace scli
{   
    struct CurlWSStruct
    {
        CURL* curl = nullptr;
        std::mutex msgMtx;
        std::string msg;
        std::function<void(void)> callBack;
    };

    size_t wsCallback(char* ptr, size_t size, size_t nmemb, void* userdata);
}