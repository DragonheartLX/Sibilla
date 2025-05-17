#pragma once

#include <atomic>

#include "SblaCore/Utils/Singleton.h"

namespace sbla
{
	class GlobalInstance: public Singleton<GlobalInstance>
	{
	public:
		~GlobalInstance();

		std::atomic<bool> isRunning;

	private:
		friend class Singleton<GlobalInstance>;
		GlobalInstance();
	};
} // namespace sbla