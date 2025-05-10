#pragma once

#include <cstddef>
#include <cstdint>
namespace scom
{
	class UUID
	{
	public:
		UUID();
		UUID(uint64_t uuid);
		UUID(const UUID &) = default;

		operator uint64_t() const { return m_UUID; }

	private:
		uint64_t m_UUID;
	};
} // namespace scom

namespace std
{
	template <typename T>
	struct hash;

	template <>
	struct hash<scom::UUID>
	{
		std::size_t operator()(const scom::UUID &uuid) const { return (uint64_t)uuid; }
	};
} // namespace std