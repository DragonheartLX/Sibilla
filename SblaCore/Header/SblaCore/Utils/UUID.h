#pragma once

#include <cstddef>
#include <cstdint>
#include <string>

namespace sbla
{
	class UUID
	{
	public:
		UUID();
		UUID(uint64_t uuid);
		UUID(const UUID &) = default;

		operator uint64_t() const { return m_UUID; }
		std::string toString(bool upperCase = true, bool withHyphens = true) const;

	private:
		uint64_t m_UUID;
	};
} // namespace sbla

namespace std
{
	template <typename T>
	struct hash;

	template <>
	struct hash<sbla::UUID>
	{
		std::size_t operator()(const sbla::UUID &uuid) const { return (uint64_t)uuid; }
	};
} // namespace std