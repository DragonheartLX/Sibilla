#pragma once

#include <string>
#include <vector>

#include "SblaCore/DataProtocol/Protocol.h"

namespace sbla
{
	struct MessageSegments
	{
		std::string type = "";
		SblaMap data	 = {};
	};

	using Message = std::vector<MessageSegments>;
} // namespace sbla