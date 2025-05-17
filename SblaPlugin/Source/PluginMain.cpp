#include <string>

#include "PluginConfig.h"
#include "SblaPlugin.h"

void initPlugin(PluginInfo* info)
{
	info->platform.push_back("Onebot V11");
	info->chatbot.push_back("Liz Brucstine");
	logger->info("SblaPlugin init.");
}

IPlatform* loadPlatForm(std::string name, Config* config)
{
	if (name == "Onebot V11") return new OneBot;
	return nullptr;
}

IChatbot* loadChatbot(std::string name, Config* config)
{
	if (name == "Liz Brucstine") return new Liz;
	return nullptr;
}