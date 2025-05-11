#include "Liz.h"

#include <SibillaCommon.h>

scom::ChatBot* createChatBot(scom::ChatBotInfo* info)
{
	info->name	  = "Liz Brucstine";
	info->version = "0.0.0-dev";
	return new Liz;
};

Liz::Liz() {}

Liz::~Liz() {}

bool Liz::msgProcessCallBack(scom::MessageRecv* recv, scom::MessageSend* send) { return false; }
