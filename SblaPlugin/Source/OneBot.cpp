#include <SblaCore/DataProtocol/Event/Event.h>
#include <SblaCore/DataProtocol/Event/EventData.h>
#include <hv/HttpMessage.h>

#include <format>
#include <functional>
#include <string>

#include "PluginConfig.h"
#include "SblaCore/DataProtocol/Message/Message.h"
#include "SblaPlugin.h"

OneBot::OneBot(std::string& url, std::string& token) : m_WSUrl(url), m_WSToken(token) {}

void OneBot::onInit()
{
	m_WSClient.onopen	 = std::bind(&OneBot::m_WSonOpen, this);
	m_WSClient.onmessage = std::bind(&OneBot::m_WSonMessage, this, std::placeholders::_1);
	m_WSClient.onclose	 = std::bind(&OneBot::m_WSonClose, this);

	reconn_setting_t reconn;
	reconn_setting_init(&reconn);

	reconn.min_delay	= 1000;
	reconn.max_delay	= 10000;
	reconn.delay_policy = 2;

	m_WSClient.setReconnect(&reconn);
}

void OneBot::inLoop()
{
	http_headers header;
	header["Authorization"] = std::format("Bearer {0}", m_WSToken);

	m_WSClient.open(m_WSUrl.c_str(), header);

	while (isRunning());
}

void OneBot::sendRequest(sbla::Request* req) {}

void OneBot::onExit() {}

void OneBot::m_WSonOpen() { logger->info("Websocket connected."); }

void OneBot::m_WSonMessage(const std::string& msg)
{
	hv::Json json = hv::Json::parse(msg);

	logger->debug("{0}", json.dump(4));

	// ignore
	if (json.find("retcode") != json.end()) return;

	if (json["post_type"] != "message") return;

	for (hv::Json message : json["message"])
	{
		if (message["type"] == "text") logger->info("Get message:\n{0}", message["data"]["text"].get<std::string>());
	}
}

void OneBot::m_WSonClose() { logger->info("Websocket disconnected."); }