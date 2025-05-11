#pragma once

#include <SibillaCommon.h>

#include "Message/MessageRecv.h"
#include "Message/MessageSend.h"

class Liz: public scom::ChatBot
{
public:
	Liz();
	~Liz();

	virtual bool msgProcessCallBack(scom::MessageRecv* recv, scom::MessageSend* send) override;
};
