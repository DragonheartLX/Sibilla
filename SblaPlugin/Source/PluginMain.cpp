#include <string>

#include "PluginConfig.h"
#include "SblaPlugin.h"

void initPlugin(PluginInfo* info)
{
	info->platform.push_back("Onebot V11");
	info->chatbot.push_back("Liz Brucstine");
}

void loadPlatForm(std::string name, IPlatform* platform)
{
	if (name == "Onebot V11") platform = new OneBot;
}

void loadChatbot(std::string name, IChatbot* chatbot)
{
	if (name == "Liz Brucstine") chatbot = new Liz;
}