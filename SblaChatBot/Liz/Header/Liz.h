#pragma once

#include <SblaCore/Message/MessageRecv.h>
#include <SblaCore/Message/MessageSend.h>
#include <SblaInterface/IChatBot.h>

class Liz: public sbla::IChatBot
{
public:
	Liz();
	~Liz();

	virtual bool msgProcessCallBack(sbla::MessageRecv* recv, sbla::MessageSend* send) override;
};
