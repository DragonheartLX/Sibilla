#pragma once
#include "Message/Message.h"
#include "Message/SingleMsg.h"

#include <vector>

namespace scli
{
    struct MessageSend
    {
        int64_t userId = 0;
        int64_t groupId = 0;
        MessageType type = MessageType::Null;
        std::vector<SingleMsg> msg;

        MessageSend() = default;

        MessageSend(MessageSend* other)
        {
            type = other->type;
            userId = other->userId;
            groupId = other->groupId;
            msg = other->msg;
        }

        void operator=(MessageSend* other)
        {
            type = other->type;
            userId = other->userId;
            groupId = other->groupId;
            msg = other->msg;
        }
    };
} // namespace scli