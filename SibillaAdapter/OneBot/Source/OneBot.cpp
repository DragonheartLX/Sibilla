#include "OneBot.h"

#include <Export.h>
#include <Logger.h>

scom::Adapter* createAdapter(scom::AdapterInfo* info)
{
	info->name	  = "OneBot";
	info->version = "0.0.0-dev";
	return new OneBot;
};

OneBot::OneBot() { scom::Logger::info("OneBot v11 init."); }

OneBot::~OneBot() {}

void OneBot::receive() {}

void OneBot::send() {}