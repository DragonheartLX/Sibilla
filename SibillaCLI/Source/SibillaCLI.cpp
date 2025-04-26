#include <curl/curl.h>
#include <atomic>
#include <thread>

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

    for (size_t i = 0; i < 100; i++)
    {
        scli::Logger::info("Log {0}", i);
        std::this_thread::sleep_for(std::chrono::seconds(2));
    }
    
    loggerThread.join();
    commandThread.join();

    curl_global_cleanup();

    return 0;
}
