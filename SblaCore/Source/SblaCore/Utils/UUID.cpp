#include "SblaCore/Utils/UUID.h"

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

	std::string UUID::toString() const
	{
		std::ostringstream oss;
		oss << std::hex << std::setfill('0') << std::setw(16) << m_UUID;
		return oss.str();
	}
} // namespace sbla