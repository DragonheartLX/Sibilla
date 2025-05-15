#include "SblaCore/Utils/UUID.h"

#include <algorithm>
#include <cctype>
#include <iomanip>
#include <random>
#include <sstream>

static std::random_device s_RandomDevice;
static std::mt19937_64 s_Engine(s_RandomDevice());
static std::uniform_int_distribution<uint64_t> s_UniformDistribution;

namespace sbla
{
	UUID::UUID() : m_UUID(s_UniformDistribution(s_Engine)) {}
	UUID::UUID(uint64_t uuid) : m_UUID(uuid) {}

	std::string UUID::toString(bool upperCase, bool withHyphens) const
	{
		std::ostringstream oss;
		oss << std::hex << std::setfill('0') << std::setw(16) << m_UUID;
		std::string formatUUID = oss.str();

		if (upperCase)
			std::transform(formatUUID.begin(), formatUUID.end(), formatUUID.begin(), std::toupper);
		else
			std::transform(formatUUID.begin(), formatUUID.end(), formatUUID.begin(), std::tolower);

		// XXXX XXXX XXXX XXXX
		// XXXX-XXXX-XXXX-XXXX
		if (withHyphens)
		{
			formatUUID.insert(4, "-");
			formatUUID.insert(9, "-");
			formatUUID.insert(14, "-");
		}

		return formatUUID;
	}
} // namespace sbla