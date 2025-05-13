#include "OneBot.h"

#include <SblaInterface/Exports.h>

sbla::IAdapter* initAdapter(AdapterInfo* info)
{
	info->name	  = "OneBot";
	info->version = "0.0.0-dev";
	return new OneBot;
};

OneBot::OneBot() { logger->info("OneBot v11 init."); }

OneBot::~OneBot() { logger->info("OneBot v11 exit."); }

void OneBot::receive() {}

void OneBot::send() {}