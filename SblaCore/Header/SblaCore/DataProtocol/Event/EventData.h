#pragma once

#include <cstdint>
#include <string>

#include "SblaCore/DataProtocol/Chatbot.h"
#include "SblaCore/DataProtocol/Event/Event.h"
#include "SblaCore/DataProtocol/Message/Message.h"
#include "SblaCore/DataProtocol/Protocol.h"

#define EVENTDATA_MEMBER(T) \
public:                     \
	T()	 = default;         \
	~T() = default

#define EVENTDATA_VALUE(TYPE, NAME, VAL)                   \
public:                                                    \
	inline TYPE NAME() { return m_##NAME; };               \
	inline void set_##NAME(TYPE& val) { m_##NAME = val; }; \
                                                           \
private:                                                   \
	TYPE m_##NAME = VAL

#define EVENTDATA_TYPE(TYPE, DETAIL, SUB)                                                                     \
public:                                                                                                       \
	virtual bool checkType(Event* event) override                                                             \
	{                                                                                                         \
		return event->type == m_Type && event->detail_type == m_Detail_Type && event->sub_type == m_Sub_Type; \
	}                                                                                                         \
                                                                                                              \
private:                                                                                                      \
	EventType m_Type		  = TYPE;                                                                         \
	std::string m_Detail_Type = DETAIL;                                                                       \
	std::string m_Sub_Type	  = SUB

namespace sbla
{

	class EventData
	{
		EVENTDATA_MEMBER(EventData);

	public:
		virtual bool checkType(Event* event) = 0;

		template <typename T>
		T* as()
		{
			return dynamic_cast<T*>(this);
		}

		template <typename T>
		const T* as() const
		{
			return dynamic_cast<const T*>(this);
		}
	};

	// Meta
	class MetaConnect: public EventData
	{
		EVENTDATA_MEMBER(MetaConnect);
		EVENTDATA_TYPE(EventType::Meta, "connect", "");

		EVENTDATA_VALUE(std::string, version, "");
	};

	class MetaHeartbeat: public EventData
	{
		EVENTDATA_MEMBER(MetaHeartbeat);
		EVENTDATA_TYPE(EventType::Meta, "heartbeat", "");

		EVENTDATA_VALUE(bool, enable, false);
		EVENTDATA_VALUE(int64_t, interval, 0);
	};

	class MetaStatusUpdate: public EventData
	{
		EVENTDATA_MEMBER(MetaStatusUpdate);
		EVENTDATA_TYPE(EventType::Meta, "status_update", "");

		EVENTDATA_VALUE(Self, self, {});
		EVENTDATA_VALUE(SblaMap, status, {});
	};

	// Message
	class MessagePrivate: public EventData
	{
		EVENTDATA_MEMBER(MessagePrivate);
		EVENTDATA_TYPE(EventType::Message, "private", "");

		EVENTDATA_VALUE(std::string, messageID, "");
		EVENTDATA_VALUE(Message, message, {});
		EVENTDATA_VALUE(std::string, alt_message, "");
		EVENTDATA_VALUE(std::string, user_id, "");
	};

	// Message Notice private/group
	// TODO

	class MessageGroup: public EventData
	{
		EVENTDATA_MEMBER(MessageGroup);
		EVENTDATA_TYPE(EventType::Message, "group", "");

		EVENTDATA_VALUE(std::string, messageID, "");
		EVENTDATA_VALUE(Message, message, {});
		EVENTDATA_VALUE(std::string, alt_message, "");
		EVENTDATA_VALUE(std::string, group_id, "");
		EVENTDATA_VALUE(std::string, user_id, "");
	};
} // namespace sbla