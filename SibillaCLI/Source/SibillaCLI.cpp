#include <curl/curl.h>
#include <thread>
#include <filesystem>

#include "Console.h"
#include "Utils/Config.h"

struct CurlStruct
{
    CURLM* curlm = nullptr;
    CURL* curl = nullptr;
    std::mutex msgMtx;
    std::string msg;
};

size_t callback(char* ptr, size_t size, size_t nmemb, void* userdata)
{
    auto self = static_cast<CurlStruct*>(userdata);
    auto frame = curl_ws_meta(self->curl);
    auto result{ size * nmemb };
    {
       std::lock_guard<std::mutex> lock(self->msgMtx);
       self->msg.append(ptr, result);
    }
    if (frame->bytesleft > 0)
    {
        return result;
    }
    
    scli::Console::debug("received one message: {0}", self->msg);
    {
        std::lock_guard<std::mutex> lock(self->msgMtx);
        self->msg.clear();
    }
    return result;
}

int main(int argc, char **argv)
{
    scli::Console::init();
    scli::Config::init();
    scli::Console::setLoggerLevel();

    scli::Console::debug("Init libcurl");
    scli::Console::debug("libcurl verion: {0}", curl_version());

    scli::Console::debug("WorkspaceRoot: {0}", std::filesystem::current_path().string());


    curl_global_init(CURL_GLOBAL_ALL);

    CurlStruct cs;
    cs.curlm = curl_multi_init();
    cs.curl = curl_easy_init();
    if (cs.curl == nullptr)
        scli::Console::error("Failed init curl");

    curl_easy_setopt(cs.curl, CURLOPT_URL, scli::Config::getInstance()->data["WSReverse"]["ws_url"].value<std::string>()->c_str());
    curl_easy_setopt(cs.curl, CURLOPT_WRITEFUNCTION, callback);
    curl_easy_setopt(cs.curl, CURLOPT_WRITEDATA, &cs);

    curl_slist* header = nullptr;
    std::string token = fmt::format("Authorization: Bearer {0}", scli::Config::getInstance()->data["WSReverse"]["access_token"].value<std::string>()->c_str());
    scli::Console::debug("Token: {0}", token);
    header = curl_slist_append(header, token.c_str());
    curl_easy_setopt(cs.curl, CURLOPT_HTTPHEADER, header);

    curl_multi_add_handle(cs.curlm, cs.curl);

    int running = 1;
    CURLMcode mc = CURLM_OK;
    while (running && mc == CURLM_OK)
    {
        mc = curl_multi_perform(cs.curlm, &running);
        if (running && mc == CURLM_OK)
        {
            mc = curl_multi_poll(cs.curlm, nullptr, 0, 160, nullptr);
        }
        else
        {
            break;
        }
    }

    curl_global_cleanup();

    return 0;
}
