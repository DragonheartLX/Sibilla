#pragma once

#include "SblaCore/Utils/Singleton.h"
namespace sbla
{
	class GlobalInstance: public Singleton<GlobalInstance>
	{
	public:
		~GlobalInstance();

	private:
		friend class Singleton<GlobalInstance>;
		GlobalInstance();
	};
} // namespace sbla