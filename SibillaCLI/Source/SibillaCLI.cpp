#include <curl/curl.h>
#include <atomic>

#include "Logger/Logger.h"
#include "Command/Command.h"

int main(int argc, char **argv)
{
    scli::Logger::setLoggerLevel(scli::trace);

    scli::Logger::debug("Init libcurl");
    scli::Logger::debug("libcurl verion: {0}", curl_version());

    std::atomic<bool> isRunning(true);
    std::thread loggerThread([&isRunning]()
    {
        scli::Logger::getInstance()->run(isRunning);
    });

    std::thread commandThread([&isRunning]()
    {
        scli::Command::getInstance()->run(isRunning);
    });


    curl_global_init(CURL_GLOBAL_ALL);

    
    
    loggerThread.join();
    commandThread.join();

    curl_global_cleanup();

    return 0;
}
