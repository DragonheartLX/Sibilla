#pragma once
/**
 * PluginConfig.h
 * Auto Generate by CMake do not modify
 * 
 * https://github.com/DragonheartLX/Sibilla
 * 
 */

#include <SblaCore/Config.h>
#include <SblaCore/Macros.h>
#include <SblaInterface/ILogger.h>
#include <SblaInterface/Plugin.h>

#define SBLA_PLUGIN_NAME "SblaPlugin"
#define SBLA_PLUGIN_VERSION "0.0.0-Dev"
#define SBLA_PLUGIN_TYPE "ALL"

#define SBLA_PLATFORM
#define SBLA_CHATBOT

using namespace sbla;

extern ILogger* logger;

void initPlugin(PluginInfo* info);

#ifdef SBL_BUILD_SHARED
	#ifdef __cplusplus
extern "C"
{
	#endif

	#ifdef SBLA_PLATFORM
	SBL_API IPlatform* loadPlatForm(std::string name, Config* config);
	#endif

	#ifdef SBLA_CHATBOT
	SBL_API IChatbot* loadChatbot(std::string name, Config* config);
	#endif

	SBL_API inline void loadPlugin(PluginInfo* info);

	#ifdef __cplusplus
}
	#endif
#endif
