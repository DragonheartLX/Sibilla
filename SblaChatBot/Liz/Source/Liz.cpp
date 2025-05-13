#include "Liz.h"

#include <SblaCore/Message/SingleMsg.h>
#include <SblaInterface/Exports.h>
#include <curl/curl.h>

#include <cstddef>
#include <nlohmann/json.hpp>
#include <string>

sbla::IChatBot* initChatBot(ChatBotInfo* info)
{
	info->name	  = "Liz Brucstine";
	info->version = "0.0.0-dev";
	return new Liz;
};

size_t write_data(void* buffer, size_t size, size_t nmemb, void* userp)
{
	size_t real_size	  = size * nmemb;
	std::string* response = static_cast<std::string*>(userp);
	response->append(static_cast<char*>(buffer), real_size);
	return real_size;
}

Liz::Liz() { curl_global_init(CURL_GLOBAL_ALL); }

Liz::~Liz() { curl_global_cleanup(); }

bool Liz::msgProcessCallBack(sbla::MessageRecv* recv, sbla::MessageSend* send)
{
	for (sbla::SingleMsg sMsg : recv->content.content)
	{
		if (sMsg.type == sbla::SingleMsgType::Text) logger->debug("Receive message: {0}", sMsg.data);

		if (sMsg.data == "测试")
		{
			send->groupId	   = recv->groupId;
			send->userId	   = recv->senderId;
			send->content.type = recv->content.type;
			send->content.content.push_back(sbla::SingleMsg{sbla::SingleMsgType::Text, "测试回复"});
			return true;
		}

		if (sMsg.data == "一言")
		{
			CURL* curl;
			CURLcode res;
			curl = curl_easy_init();
			if (curl)
			{
				std::string response_buffer;
				curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "GET");
				curl_easy_setopt(curl, CURLOPT_URL, "https://v1.hitokoto.cn/");
				curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
				curl_easy_setopt(curl, CURLOPT_DEFAULT_PROTOCOL, "https");
				curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_data);
				curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response_buffer);
				curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, false);
				curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 0);

				res = curl_easy_perform(curl);

				if (res == CURLE_OK)
				{
					nlohmann::json data = nlohmann::json::parse(response_buffer);
					send->groupId		= recv->groupId;
					send->userId		= recv->senderId;
					send->content.type	= recv->content.type;
					send->content.content.push_back(sbla::SingleMsg{sbla::SingleMsgType::Text, data["hitokoto"].get<std::string>()});
				}
			}
			curl_easy_cleanup(curl);
			if (res == CURLE_OK) return true;
		}
	}

	return false;
}
