#pragma once

#include <string>

#include "SblaCore/DataProtocol/Chatbot.h"
#include "SblaCore/DataProtocol/Protocol.h"

namespace sbla
{
	struct Request
	{
		std::string aciton = "";
		SblaMap pramas	   = {};

		std::string echo   = "";
		Self self		   = {};
	};
} // namespace sbla