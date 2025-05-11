#include "SibillaCLI.h"

namespace scli
{
	SibillaCLI::SibillaCLI() {}

	SibillaCLI::~SibillaCLI() { m_IsRunning = false; }

	bool SibillaCLI::run()
	{
		m_IsRunning = true;
		while (true)
		{
		}
	}
} // namespace scli