#pragma once
#include <SblaInterface/Plugin.h>
#include <hv/WebSocketClient.h>

#include <string>

class OneBot: public sbla::IPlatform
{
public:
	OneBot(std::string& url, std::string& token);

	virtual void onInit();
	virtual void inLoop();
	virtual void sendRequest(sbla::Request* req);
	virtual void onExit();

private:
	std::string m_WSUrl;
	std::string m_WSToken;

	void m_WSonOpen();
	void m_WSonMessage(const std::string& msg);
	void m_WSonClose();

	hv::WebSocketClient m_WSClient;
};

class Liz: public sbla::IChatbot
{
public:
	virtual void onInit();
	virtual void onExit();

	virtual void onEnable();
	virtual void onDisable();

	virtual bool onEvent(sbla::Event* event, sbla::Request* req);
};