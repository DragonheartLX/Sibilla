#include "MessageHub/MessageHub.h"
#include "Utils/Uuid.h"

namespace scli
{
    MsgCallBack::MsgCallBack()
    {
        m_Id = GenerateUUID();
    }

    bool MsgCallBack::operator==(MsgCallBack& other)
    {
        return this->m_Id == other.m_Id;
    };

    void MsgCallBack::operator=(std::function<bool(std::string, void*)> callback)
    {
        m_Func = callback;
    };

    void MsgCallBack::exec(std::string topic, void* data)
    {
        m_Func(topic, data);
    };

    MessageHub::MessageHub()
    {
        m_PublishMap.clear();
        m_SubscribeMap.clear();

        m_CoreProcessThread = std::thread([this]()
        {
            while (m_IsRunning)
            {
                std::lock_guard<std::mutex> subMutex(m_SubscribeMutex);
                auto subIt = m_SubscribeMap.begin();

                while (subIt != m_SubscribeMap.end())
                {
                    std::lock_guard<std::mutex> pubMutex(m_PublishMutex);
                    if (m_PublishMap.find(subIt->first) != m_PublishMap.end())
                    {
                        auto funIt = subIt->second.begin();
                        while (funIt != subIt->second.end())
                        {
                            auto beginIt = m_PublishMap.find(subIt->first)->second.begin();
                            while (beginIt != m_PublishMap.find(subIt->first)->second.end())
                            {
                                (*funIt)->exec(subIt->first, *beginIt);
                                ++beginIt;
                            }

                            ++funIt;
                        }

                        m_PublishMap.find(subIt->first)->second.clear();
                        m_PublishMap.erase(subIt->first);
                    }

                    ++subIt;
                }
            }
        });
    }

    MessageHub::~MessageHub()
    {
        m_CoreProcessThread.join();
    }

    void MessageHub::registPublish(std::string topic, void* msgData, size_t dataSize)
    {
        std::lock_guard<std::mutex> pubMutex(m_PublishMutex);
        void* tempData = new char[dataSize];
        memcpy(tempData, msgData, dataSize);
        m_PublishMap[topic].push_back(tempData);
    };

    void MessageHub::registSubscribe(std::string topic, MsgCallBack* callback)
    {
        std::lock_guard<std::mutex> lock(m_SubscribeMutex);
        m_SubscribeMap[topic].remove(callback);
        m_SubscribeMap[topic].push_back(callback);
    };

    void MessageHub::unRegistSubscribe(std::string topic, MsgCallBack* callback)
    {
        std::lock_guard<std::mutex> lock(m_SubscribeMutex);
        if (m_SubscribeMap.find(topic) != m_SubscribeMap.end())
            m_SubscribeMap[topic].remove(callback);
    };
}