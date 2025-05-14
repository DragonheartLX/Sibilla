#pragma once

#include <cstdint>
#include <string>

#include "SblaCore/DataProtocol/Chatbot.h"
#include "SblaCore/DataProtocol/Message/Message.h"
#include "SblaCore/DataProtocol/Protocol.h"

#define EVENTDATA_MEMBER(T) \
public:                     \
	T()	 = default;         \
	~T() = default

#define VALUE(TYPE, NAME, VAL)                             \
public:                                                    \
	inline TYPE NAME() { return m_##NAME; };               \
	inline void set_##NAME(TYPE& val) { m_##NAME = val; }; \
                                                           \
private:                                                   \
	TYPE m_##NAME = VAL

namespace sbla
{

	class EventData
	{
		EVENTDATA_MEMBER(EventData);

	public:
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

		VALUE(std::string, version, "");
	};

	class MetaHeartbeat: public EventData
	{
		EVENTDATA_MEMBER(MetaHeartbeat);

		VALUE(bool, enable, false);
		VALUE(int64_t, interval, 0);
	};

	class MetaStatusUpdate: public EventData
	{
		EVENTDATA_MEMBER(MetaStatusUpdate);

		VALUE(Self, self, {});
		VALUE(SblaMap, status, {});
	};

	// Message
	class MessagePrivate: public EventData
	{
		EVENTDATA_MEMBER(MessagePrivate);

		VALUE(std::string, messageID, "");
		VALUE(Message, message, {});
		VALUE(std::string, alt_message, "");
		VALUE(std::string, user_id, "");
	};

	// Message Notice private/group
	// TODO

	class MessageGroup: public EventData
	{
		EVENTDATA_MEMBER(MessageGroup);

		VALUE(std::string, messageID, "");
		VALUE(Message, message, {});
		VALUE(std::string, alt_message, "");
		VALUE(std::string, group_id, "");
		VALUE(std::string, user_id, "");
	};
} // namespace sbla