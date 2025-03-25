#pragma once

template <typename T>
	requires std::is_arithmetic_v<T>
struct Quaternion
{
};

using QuaternionF = Quaternion<f32>;
using QuaternionD = Quaternion<f64>;
using QuaternionI = Quaternion<s32>;
