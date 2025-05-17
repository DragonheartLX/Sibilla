#pragma once
#include <SblaInterface/Plugin.h>

class OneBot: public sbla::IPlatform
{
public:
	virtual void onInit() {};
	virtual void inLoop() { while (isRunning()); };
	virtual void sendRequest(sbla::Request* req) {};
	virtual void onExit() {};
};

class Liz: public sbla::IChatbot
{
public:
	virtual void onInit() {};
	virtual void onExit() {};

	virtual void onEnable() {};
	virtual void onDisable() {};

	virtual bool onEvent(sbla::Event* event, sbla::Request* req) { return false; };
};