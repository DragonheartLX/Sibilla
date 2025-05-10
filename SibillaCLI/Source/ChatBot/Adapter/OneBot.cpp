#include "Console.h"
#include "Message/MessageRecv.h"
#include "ChatBot/Adapter/OneBot.h"

#include <nlohmann/json.hpp>

namespace scli
{
	OneBot::OneBot()
	{
		m_Curlm = curl_multi_init();
		m_Curl = curl_easy_init();

		m_WSData.curl = m_Curl;
		m_WSData.callBack = std::bind(&OneBot::msgCallBack, this);

		if (m_Curl == nullptr) Console::error("Failed init curl");

		curl_easy_setopt(m_Curl, CURLOPT_URL, "ws://127.0.0.1");
		curl_easy_setopt(m_Curl, CURLOPT_WRITEFUNCTION, wsCallback);
		curl_easy_setopt(m_Curl, CURLOPT_WRITEDATA, &m_WSData);

		curl_slist* header = nullptr;
		std::string token =
			fmt::format("Authorization: Bearer {0}", "1234567890");
		header = curl_slist_append(header, token.c_str());
		curl_easy_setopt(m_Curl, CURLOPT_HTTPHEADER, header);

		// curl_easy_setopt(m_Curl, CURLOPT_VERBOSE, 1L);

		curl_multi_add_handle(m_Curlm, m_Curl);
	}

	OneBot::~OneBot()
	{
		curl_easy_cleanup(m_Curl);
		curl_multi_cleanup(m_Curlm);
	}

	void OneBot::receive()
	{
		int running = 1;
		CURLMcode curlCode = CURLM_OK;

		while (running && curlCode == CURLM_OK)
		{
			curlCode = curl_multi_perform(m_Curlm, &running);
			if (running && curlCode == CURLM_OK)
			{
				curlCode = curl_multi_poll(m_Curlm, nullptr, 0, 160, nullptr);
			}
			else
			{
				break;
			}
		}
	}

	void OneBot::send()
	{
		MessageSend msg;
		while (isRunning())
		{
			if (sendMsg(&msg))
			{
				nlohmann::json jsonData;

				switch (msg.type)
				{
					case MessageType::Group:
						jsonData["action"] = "send_group_msg";
						jsonData["params"]["group_id"] = msg.groupId;
						jsonData["params"]["message"] = nlohmann::json::array();

						for (SingleMsg message : msg.msg)
						{
							if (message.type == SingleMsgType::Text)
							{
								jsonData["params"]["message"].push_back(
									nlohmann::json{
										{"type", "text"},
										{"data", {{"text", message.data}}}});
							}

							if (message.type == SingleMsgType::Image)
							{
								jsonData["params"]["message"].push_back(
									nlohmann::json{
										{"type", "image"},
										{"data", {{"file", message.data}}}});
							}

							if (message.type == SingleMsgType::Reply)
							{
								jsonData["params"]["message"].push_back(
									nlohmann::json{
										{"type", "reply"},
										{"data", {{"id", message.data}}}});
							}

							if (message.type == SingleMsgType::At)
							{
								jsonData["params"]["message"].push_back(
									nlohmann::json{
										{"type", "at"},
										{"data", {{"qq", message.data}}}});
							}

							if (message.type == SingleMsgType::File)
							{
								// jsonData["params"]["message"] = nlohmann::json::array();
								// jsonData["params"]["message"].push_back(nlohmann::json
								// {
								//     { "type", ""},
								//     { "data", {{ "", message.data }}}
								// });
							}
						}
						break;
					case MessageType::Private: break;
					default: break;
				}

				size_t offset = 0;
				CURLcode res = CURLE_OK;
				std::string data = jsonData.dump();
				while (!res)
				{
					size_t sent = 0;
					res = curl_ws_send(m_Curl, data.c_str() + offset,
									   data.size() - offset, &sent, 0,
									   CURLWS_TEXT);

					offset += sent;

					if (res == CURLE_OK && offset == data.size()) break;
				}
			}
		}
	}

	void OneBot::msgCallBack()
	{
		nlohmann::json msg = nlohmann::json::parse(m_WSData.msg);

		if (msg.find("time") == msg.end()) return;

		std::time_t time = msg["time"].get<time_t>(); // 时间戳
		int64_t self_id = msg["time"].get<int64_t>(); // 收到事件的机器人 QQ 号
		std::string post_type = msg["post_type"].get<std::string>(); // 事件类型

		// meta_event：元事件 包括 OneBot 生命周期、心跳等
		if (post_type == "meta_event")
		{
			std::string meta_event_type =
				msg["meta_event_type"].get<std::string>();

			if (meta_event_type == "lifecycle")
			{
				std::string sub_type = msg["sub_type"].get<std::string>();

				if (sub_type == "connect")
					Console::debug("Bot websocket connected.");

				if (sub_type == "heartbeat") Console::debug("Bot heartbeat.");
			}
		}

		// message：消息事件 包括私聊消息、群消息等
		if (post_type == "message")
		{
			std::string message_type = msg["message_type"].get<std::string>();

			if (message_type == "group")
			{
				MessageRecv recvMsg;
				recvMsg.time = msg["time"].get<int64_t>();
				recvMsg.botId = msg["self_id"].get<int64_t>();
				recvMsg.type = MessageType::Group;
				recvMsg.messageId = msg["message_id"].get<int64_t>();
				recvMsg.senderId = msg["user_id"].get<int64_t>();
				recvMsg.senderNickname =
					msg["sender"]["nickname"].get<std::string>();
				recvMsg.groupId = msg["group_id"].get<int64_t>();

				for (nlohmann::json message : msg["message"])
				{
					if (message["type"] == "text")
						recvMsg.msg.push_back(SingleMsg{
							SingleMsgType::Text,
							message["data"]["text"].get<std::string>()});

					if (message["type"] == "image")
						recvMsg.msg.push_back(SingleMsg{
							SingleMsgType::Image,
							message["data"]["file"].get<std::string>()});

					if (message["type"] == "reply")
						recvMsg.msg.push_back(SingleMsg{
							SingleMsgType::Reply,
							message["data"]["id"].get<std::string>()});

					if (message["type"] == "at")
						recvMsg.msg.push_back(SingleMsg{
							SingleMsgType::At,
							message["data"]["qq"].get<std::string>()});

					// if (message["type"] == "file")
					//     recvMsg.msg.push_back(SingleMsg{ SingleMsgType::File, message["data"][""].get<std::string>() });
				}

				receiveMsg(&recvMsg);
			}
		}

		// notice：通知事件 包括群成员变动、好友变动等
		// request：请求事件 包括加群请求、加好友请求等
	}
} // namespace scli