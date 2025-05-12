#pragma once
#include "SblaCore/Macros.h"
#include "SblaInterface/IAdapter.h"
#include "SblaInterface/IChatBot.h"
#include "SblaInterface/ILogger.h"

struct AdapterInfo
{
	std::string name	= "";
	std::string version = "";
};

struct ChatBotInfo
{
	std::string name	= "";
	std::string version = "";
	std::string adapter = "";
};

struct InitInfo
{
	sbla::ILogger* log		= nullptr;
	sbla::IAdapter* adapter = nullptr;
	sbla::IChatBot* chatBot = nullptr;
	AdapterInfo adapterInfo = {};
	ChatBotInfo chatBotInfo = {};
};

static sbla::ILogger* logger = nullptr;

#ifdef SBL_ADAPTER_BUILD
sbla::IAdapter* initAdapter(AdapterInfo* info);
#endif

#ifdef SBL_CHATBOT_BUILD
sbla::IChatBot* initChatBot(ChatBotInfo* info);
#endif

// Export entry point
#ifdef SBL_BUILD_SHARED
	#ifdef __cplusplus
extern "C"
{
	#endif

	SBL_API void init(InitInfo* info)
	{
		logger = info->log;
	#ifdef SBL_ADAPTER_BUILD
		info->adapter = initAdapter(&info->adapterInfo);
	#endif

	#ifdef SBL_CHATBOT_BUILD
		info->chatBot = initChatBot(&info->chatBotInfo);
	#endif
	};

	#ifdef __cplusplus
}
	#endif
#endif