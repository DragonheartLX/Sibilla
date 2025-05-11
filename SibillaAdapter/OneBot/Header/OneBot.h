#pragma once

#include <SibillaCommon.h>

class OneBot: public scom::Adapter
{
public:
	OneBot();
	~OneBot();

	virtual void receive() override;
	virtual void send() override;
};
