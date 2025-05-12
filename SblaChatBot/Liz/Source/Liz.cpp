#include "Liz.h"

#include <SblaInterface/Exports.h>

sbla::IChatBot* initChatBot(ChatBotInfo* info)
{
	info->name	  = "Liz Brucstine";
	info->version = "0.0.0-dev";
	info->adapter = "OneBot";
	return new Liz;
};

Liz::Liz() {}

Liz::~Liz() {}

bool Liz::msgProcessCallBack(sbla::MessageRecv* recv, sbla::MessageSend* send) { return false; }
