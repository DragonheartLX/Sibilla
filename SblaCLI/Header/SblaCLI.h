#pragma once

#include <SblaCore/Utils/Singleton.h>

#include "SblaInterface/Exports.h"

namespace sbla
{
	using DyLibInitFunc = void (*)(InitInfo *);

	class SblaCLI: public Singleton<SblaCLI>
	{
	public:
		~SblaCLI();

		bool run();

	public:
		bool m_IsRunning = true;

		friend class Singleton<SblaCLI>;
		SblaCLI();
	};
} // namespace sbla