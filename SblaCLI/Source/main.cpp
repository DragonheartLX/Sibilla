#include <chrono>
#include <cstdlib>
#include <dylib.hpp>
#include <thread>

#include "Console.h"
#include "SblaInterface/Exports.h"
#include "SblaInterface/ILogger.h"

int main(int argc, char** argv)
{
	if (sbla::Console::init() == false) return EXIT_FAILURE;
	sbla::Console::setLoggerLevel(sbla::LoggerLevel::trace);

	sbla::Console::info("init.");

	InitInfo info = {};
	info.log	  = sbla::Console::getInstance().get();

	dylib adapter("./Adapter", "OneBotAdapter");
	auto initAdapterFunc = adapter.get_function<void(InitInfo*)>("init");
	initAdapterFunc(&info);
	sbla::Console::info("Name: {0}, Version: {1}", info.adapterInfo.name, info.adapterInfo.version);

	dylib chatBot("./Bot", "LizBot");
	auto initChatBotFunc = chatBot.get_function<void(InitInfo*)>("init");
	initChatBotFunc(&info);
	sbla::Console::info("Name: {0}, Version: {1}", info.chatBotInfo.name, info.chatBotInfo.version);

	while (true)
	{
		std::this_thread::sleep_for(std::chrono::milliseconds(1000));
		sbla::Console::info("Tick.");
	};
	return EXIT_SUCCESS;
}