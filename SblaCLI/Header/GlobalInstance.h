#pragma once

#include <atomic>
#include <map>
#include <string>

#include "SblaCore/Config.h"
#include "SblaCore/Utils/Singleton.h"

namespace sbla
{
	class GlobalInstance: public Singleton<GlobalInstance>
	{
	public:
		~GlobalInstance();

		std::atomic<bool> isRunning;
		std::map<std::string, Config> config;

	private:
		friend class Singleton<GlobalInstance>;
		GlobalInstance();
	};
} // namespace sbla