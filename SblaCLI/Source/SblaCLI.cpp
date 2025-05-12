#include "SblaClI.h"

namespace sbla
{
	SblaCLI::SblaCLI() {}

	SblaCLI::~SblaCLI() {}

	bool SblaCLI::run()
	{
		while (m_IsRunning);
		return true;
	}
} // namespace sbla