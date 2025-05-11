#pragma once

#include <SibillaCommon.h>

class OneBot: public scom::Adapter
{
public:
	virtual void receive() override;
	virtual void send() override;
};
