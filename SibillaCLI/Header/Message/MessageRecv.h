#pragma once
#include "Message/Message.h"
#include "Message/SingleMsg.h"

#include <string>
#include <vector>

namespace scli
{
    struct MessageRecv
    {
        int64_t time = 0;
        int64_t botId = 0;
        MessageType type = MessageType::Null;
        int32_t messageId = 0;
        int64_t senderId = 0;
        int64_t groupId = 0;
        std::string senderNickname = "";
        std::vector<SingleMsg> msg;

        MessageRecv() = default;

        MessageRecv(MessageRecv* other)
        {
            time = other->time;
            botId = other->botId;
            type = other->type;
            messageId = other->messageId;
            senderId = other->senderId;
            groupId = other->groupId;
            msg = other->msg;
        }

        void operator=(MessageRecv* other)
        {
            time = other->time;
            botId = other->botId;
            type = other->type;
            messageId = other->messageId;
            senderId = other->senderId;
            groupId = other->groupId;
            msg = other->msg;
        }
    };
}