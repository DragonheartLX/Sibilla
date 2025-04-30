#pragma once
#include "Utils/Singleton.h"

#include <map>
#include <list>
#include <functional>

#include <mutex>
#include <atomic>
#include <thread>

namespace scli
{
    class MsgCallBack
    {
    public:
        MsgCallBack();
        ~MsgCallBack();

        bool operator==(MsgCallBack& other);
        void operator=(std::function<bool(std::string, void*)> callback);
        void exec(std::string topic, void* data);
    private:
        std::string m_Id;
        std::function<bool(std::string, void*)> m_Func;
    };

    class MessageHub:public Singleton<MessageHub>
    {
    public:
        ~MessageHub();

        void registPublish(std::string topic, void* msgData, size_t dataSize);
        void registSubscribe(std::string topic, MsgCallBack* callback);
        void unRegistSubscribe(std::string topic, MsgCallBack* callback);
    private:
        std::atomic<bool> m_IsRunning;
        std::thread m_CoreProcessThread;

        std::mutex m_PublishMutex;
        std::map<std::string, std::list<void*>> m_PublishMap;

        std::mutex m_SubscribeMutex;
        std::map<std::string, std::list<MsgCallBack*>> m_SubscribeMap;

        friend class Singleton<MessageHub>;
        MessageHub();
    };
}
