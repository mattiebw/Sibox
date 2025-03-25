#include "siboxpch.h"

// Define this function here to avoid circular dependencies.
template <typename T> requires std::is_arithmetic_v<T>
Rotator<T> Vector3<T>::GetDirectionRotator(bool normalise) const
{
	if (normalise)
		return Normalized().GetDirectionRotator(false);

	// MW @todo: Implement

	Rotator<T> result;
	return result;
}
