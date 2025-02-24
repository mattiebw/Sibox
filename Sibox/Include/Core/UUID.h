#pragma once

// MW @credit: From https://github.com/TheCherno/Hazel/blob/1feb70572fa87fa1c4ba784a2cfeada5b4a500db/Hazel/src/Hazel/Core/UUID.h

struct UUID
{
	UUID();
	UUID(uint64_t uuid);
	UUID(const UUID &other) = default;

	operator uint64_t() const { return m_UUID; }

private:
	uint64_t m_UUID;
};

namespace std
{
	template <typename T>
	struct hash;

	template <>
	struct hash<UUID>
	{
		std::size_t operator()(const UUID &uuid) const noexcept
		{
			return uuid;
		}
	};
}
