#include "PluginConfig.h"

ILogger* logger = nullptr;

void loadPlugin(PluginInfo* info)
{
	logger		  = info->logger;
	info->name	  = SBLA_PLUGIN_NAME;
	info->version = SBLA_PLUGIN_VERSION;
	info->type	  = SBLA_PLUGIN_TYPE;
	initPlugin(info);
};