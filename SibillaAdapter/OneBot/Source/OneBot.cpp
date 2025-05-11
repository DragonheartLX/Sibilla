#include "OneBot.h"

#include <SibillaCommon.h>

scom::Adapter* createAdapter(scom::AdapterInfo* info)
{
	info->name	  = "OneBot";
	info->version = "0.0.0-dev";
	return new OneBot;
};

void OneBot::receive() {}

void OneBot::send() {}