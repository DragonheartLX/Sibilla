#pragma once

#include "SblaCore/Config.h"
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
};

struct InitInfo
{
	sbla::ILogger* logger	= nullptr;
	sbla::IAdapter* adapter = nullptr;
	sbla::IChatBot* chatBot = nullptr;
	AdapterInfo adapterInfo = {};
	ChatBotInfo chatBotInfo = {};
	sbla::Config* config	= nullptr;
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
		logger = info->logger;
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