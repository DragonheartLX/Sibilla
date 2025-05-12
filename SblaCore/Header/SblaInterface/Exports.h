#pragma once
#include "SblaCore/Macros.h"
#include "SblaInterface/ILogger.h"

// Export entry point
#ifdef SBL_BUILD_SHARED
	#ifdef __cplusplus
extern "C"
{
	#endif

	#ifdef SBL_ADAPTER_BUILD
	SBL_API scom::Adapter* createAdapter(scom::AdapterInfo* info);

	SBL_API void init(scom::LoggerCallBack cb)
	{
		scom::Logger::bind(cb);
		scom::Logger::setLoggerLocation("Adapter");
	};
	#endif

	#ifdef SBL_CHATBOT_BUILD
	SBL_API scom::ChatBot* createChatBot(scom::ChatBotInfo* info);

	SBL_API void init(scom::LoggerCallBack cb)
	{
		scom::Logger::bind(cb);
		scom::Logger::setLoggerLocation("ChatBot");
	};
	#endif

	#ifdef __cplusplus
}
	#endif
#endif