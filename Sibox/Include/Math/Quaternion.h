#pragma once

template <typename T>
	requires std::is_arithmetic_v<T>
struct Quaternion
{
	
};

using QuaternionF = Quaternion<float>;
using QuaternionD = Quaternion<f64>;
using QuaternionI = Quaternion<int>;
