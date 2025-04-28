#include <curl/curl.h>
#include <thread>

#include "Console.h"

int main(int argc, char **argv)
{
    scli::Console::setLoggerLevel(scli::trace);

    scli::Console::debug("Init libcurl");
    scli::Console::debug("libcurl verion: {0}", curl_version());


    curl_global_init(CURL_GLOBAL_ALL);

    for (size_t i = 0; i < 100; i++)
    {
        scli::Console::info("Log {0}", i);
        std::this_thread::sleep_for(std::chrono::seconds(2));
    }

    curl_global_cleanup();

    return 0;
}
