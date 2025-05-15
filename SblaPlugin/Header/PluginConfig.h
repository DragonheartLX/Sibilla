#pragma once
/**
 * PluginConfig.h
 * Auto Generate by CMake do not modify
 * 
 * https://github.com/DragonheartLX/Sibilla
 * 
 */

#define SBLA_PLUGIN_NAME "SblaPlugin"
#define SBLA_PLUGIN_VERSION "0.0.0-Dev"
#define SBLA_PLUGIN_TYPE "ALL"

#define SBLA_PLATFORM
#define SBLA_CHATBOT

#include <SblaCore/Macros.h>
#include <SblaInterface/Plugin.h>

#include <string>

using namespace sbla;

void initPlugin(PluginInfo* info);

#ifdef SBLA_PLATFORM
void loadPlatForm(std::string name, IPlatform* platform);
#endif

#ifdef SBLA_CHATBOT
void loadChatbot(std::string name, IChatbot* chatbot);
#endif

#ifdef SBL_BUILD_SHARED
	#ifdef __cplusplus
extern "C"
{
	#endif
	inline void loadPlugin(PluginInfo* info)
	{
		info->name	  = SBLA_PLUGIN_NAME;
		info->version = SBLA_PLUGIN_VERSION;
		info->type	  = SBLA_PLUGIN_TYPE;
		initPlugin(info);
	};
	#ifdef __cplusplus
}
	#endif
#endif
