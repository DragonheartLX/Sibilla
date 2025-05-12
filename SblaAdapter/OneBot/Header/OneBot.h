#pragma once
#include <SblaInterface/IAdapter.h>

class OneBot: public sbla::IAdapter
{
public:
	OneBot();
	~OneBot();

	virtual void receive() override;
	virtual void send() override;
};
