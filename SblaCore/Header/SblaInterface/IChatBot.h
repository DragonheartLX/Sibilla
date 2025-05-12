#pragma once
#include "SblaCore/Message/MessageRecv.h"
#include "SblaCore/Message/MessageSend.h"

namespace sbla
{
	// ==========
	// ChatBot
	// ==========

	class IChatBot
	{
	public:
		IChatBot()															  = default;
		virtual ~IChatBot()													  = default;

		virtual bool msgProcessCallBack(MessageRecv* recv, MessageSend* send) = 0;
	};
} // namespace sbla