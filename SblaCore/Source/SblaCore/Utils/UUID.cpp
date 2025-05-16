#include "SblaCore/Utils/UUID.h"

#include <iomanip>
#include <ios>
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
		oss << std::hex << (upperCase ? std::uppercase : std::nouppercase) << std::setfill('0') << std::setw(16) << m_UUID;
		std::string formatUUID = oss.str();

		// XXXX XXXX XXXX XXXX
		// XXXX-XXXX-XXXX-XXXX
		if (withHyphens)
			for (int i = 0; i < 3; i++) formatUUID.insert(5 * i + 4, "-");

		return formatUUID;
	}
} // namespace sbla