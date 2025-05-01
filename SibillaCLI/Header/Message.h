#pragma once
#include <string>
#include <vector>

namespace scli
{
    enum class SingleMsgType
    {
        Null,
        Text,
        Image,
        Reply,
        At,
        File
    };

    enum class MessageType
    {
        Null,
        Private,
        Group
    };

    struct SingleMsg
    {
        SingleMsgType type = SingleMsgType::Null;
        std::string data = "";
    };

    struct Message
    {
        int64_t time = 0;
        int64_t botId = 0;
        MessageType type = MessageType::Null;
        int32_t messageId = 0;
        int64_t senderId = 0;
        std::string senderNickname = "";
        std::vector<SingleMsg> msg;

        Message() = default;

        Message(Message* other)
        {
            time = other->time;
            botId = other->botId;
            type = other->type;
            messageId = other->messageId;
            senderId = other->senderId;
            msg = other->msg;
        }

        void operator=(Message* other)
        {
            time = other->time;
            botId = other->botId;
            type = other->type;
            messageId = other->messageId;
            senderId = other->senderId;
            msg = other->msg;
        }
    };
}