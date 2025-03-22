#pragma once

// template<typename T, int Size>
// struct Vector
// {
// public:
//     union
//     {
//         T Data[Size];
//         struct
//         {
//             T X, Y, Z, W;
//         };
//     };
// };

// Forward declare Vector3 and Vector4 so we can use them in Vector2.
template<typename T>
    requires std::is_arithmetic_v<T>
struct Vector3;

template<typename T>
    requires std::is_arithmetic_v<T>
struct Vector4;

template<typename T>
	requires std::is_arithmetic_v<T>
struct Vector2
{
public:
    union { T X, R; };
    union { T Y, G; };

    Vector2()
        : X(0), Y(0)
    { }
    
    Vector2(T scalar)
        : X(scalar), Y(scalar)
    { }

    Vector2(T x, T y)
        : X(x), Y(y)
    { }

    bool operator==(const Vector2 &other) const
    {
        return X == other.X && Y == other.Y;
    }

    bool operator!=(const Vector2 &other) const
    {
        return !(*this == other);
    }
    
    Vector2 operator+(const Vector2 &other) const
    {
        return { X + other.X, Y + other.Y };
    }

    Vector2 operator-(const Vector2 &other) const
    {
        return { X - other.X, Y - other.Y };
    }

    Vector2 operator*(const Vector2 &other) const
    {
        return { X * other.X, Y * other.Y };
    }

    Vector2 operator*(T scalar) const
    {
        return { X * scalar, Y * scalar };
    }

    Vector2 operator/(const Vector2 &other) const
    {
        return { X / other.X, Y / other.Y };
    }

    Vector2 operator/(T scalar) const
    {
        return { X / scalar, Y / scalar };
    }

    void operator+=(const Vector2 &other)
    {
        X += other.X;
        Y += other.Y;
    }

    void operator-=(const Vector2 &other)
    {
        X -= other.X;
        Y -= other.Y;
    }

    void operator*=(const Vector2 &other)
    {
        X *= other.X;
        Y *= other.Y;
    }

    void operator*=(T scalar)
    {
        X *= scalar;
        Y *= scalar;
    }

    void operator/=(const Vector2 &other)
    {
        X /= other.X;
        Y /= other.Y;
    }

    void operator/=(T scalar)
    {
        X /= scalar;
        Y /= scalar;
    }

    Vector2 operator-() const
    {
        return { -X, -Y };
    }
    
    T operator[](int index) const
    {
        return (&X)[index];
    }

    T &operator[](int index)
    {
        return (&X)[index];
    }

    NODISCARD FORCEINLINE T* Data() { return &X; }
    
    NODISCARD FORCEINLINE T LengthSquared() const
    {
        return X * X + Y * Y;
    }

    NODISCARD FORCEINLINE T Length() const
    {
        return sqrtf(LengthSquared());
    }

    FORCEINLINE void Normalize()
    {
        T length = Length();
        X /= length;
        Y /= length;
    }

    NODISCARD FORCEINLINE Vector2 Normalized() const
    {
        Vector2 result = *this;
        result.Normalize();
        return result;
    }

    NODISCARD FORCEINLINE bool IsNormalized() const
    {
        return LengthSquared() - 1.0f < 0.0001f;
    }
    
    // Calculate the dot product between this vector and another.
    NODISCARD FORCEINLINE T Dot(const Vector2 &other, bool normalise = true) const
    {
        if (normalise)
            return Normalized().Dot(other.Normalized(), false);
        
        return (X * other.X)
            + (Y * other.Y);
    }

    FORCEINLINE T DistanceSquared(const Vector2 &other) const
    {
        return (*this - other).LengthSquared();
    }
    
    FORCEINLINE T Distance(const Vector2 &other) const
    {
        return (*this - other).Length();
    }

    FORCEINLINE void Zero()
    {
        X = 0;
        Y = 0;
    }

    FORCEINLINE bool IsValid() const
    {
        // A quirk of floating point arithmetic is that NaN values are NEVER equal to anything - not even themselves.
        // Therefore, if a float multiplied by zero is not equal to the same float multiplied by zero, then it is NaN.
        // We'll also check for infinity.
        if (X * 0.0f != X * 0.0f || std::isinf(X))
            return false;
        if (Y * 0.0f != Y * 0.0f || std::isinf(Y))
            return false;
        
        return true;
    }

    // Casting operators
    template<typename OT>
    FORCEINLINE explicit operator Vector3<OT>() const
    {
        return { static_cast<OT>(X), static_cast<OT>(Y), 0 };
    }

    template<typename OT>
    FORCEINLINE explicit operator Vector4<OT>() const
    {
        return { static_cast<OT>(X), static_cast<OT>(Y), 0, 0 };
    }
};

template<typename T>
	requires std::is_arithmetic_v<T>
struct Vector3
{
public:
    union { T X, R; };
    union { T Y, G; };
    union { T Z, B; };

    Vector3()
        : X(0), Y(0), Z(0)
    { }
    
    Vector3(T scalar)
        : X(scalar), Y(scalar), Z(scalar)
    { }

    Vector3(T x, T y, T z)
        : X(x), Y(y), Z(z)
    { }

    template<typename OT>
    Vector3(Vector2<OT> xy, T z)
        : X(static_cast<T>(xy.X)), Y(static_cast<T>(xy.Y)), Z(z)
    { }

    bool operator==(const Vector3 &other) const
    {
        return X == other.X && Y == other.Y && Z == other.Z;
    }

    bool operator!=(const Vector3 &other) const
    {
        return !(*this == other);
    }
    
    Vector3 operator+(const Vector3 &other) const
    {
        return { X + other.X, Y + other.Y, Z + other.Z };
    }

    Vector3 operator-(const Vector3 &other) const
    {
        return { X - other.X, Y - other.Y, Z - other.Z };
    }
    
    Vector3 operator*(const Vector3 &other) const
    {
        return { X * other.X, Y * other.Y, Z * other.Z };
    }

    Vector3 operator*(T scalar) const
    {
        return { X * scalar, Y * scalar, Z * scalar };
    }

    Vector3 operator/(const Vector3 &other) const
    {
        return { X / other.X, Y / other.Y, Z / other.Z };
    }

    Vector3 operator/(T scalar) const
    {
        return { X / scalar, Y / scalar, Z / scalar };
    }
    
    void operator+=(const Vector3 &other)
    {
        X += other.X;
        Y += other.Y;
        Z += other.Z;
    }

    void operator-=(const Vector3 &other)
    {
        X -= other.X;
        Y -= other.Y;
        Z -= other.Z;
    }

    void operator*=(const Vector3 &other)
    {
        X *= other.X;
        Y *= other.Y;
        Z *= other.Z;
    }

    void operator*=(T scalar)
    {
        X *= scalar;
        Y *= scalar;
        Z *= scalar;
    }

    void operator/=(const Vector3 &other)
    {
        X /= other.X;
        Y /= other.Y;
        Z /= other.Z;
    }

    void operator/=(T scalar)
    {
        X /= scalar;
        Y /= scalar;
        Z /= scalar;
    }
    
    Vector3 operator-() const
    {
        return { -X, -Y, -Z };
    }
    
    T operator[](int index) const
    {
        return (&X)[index];
    }

    T &operator[](int index)
    {
        return (&X)[index];
    }
    
    NODISCARD FORCEINLINE T* Data() { return &X; }
    
    NODISCARD FORCEINLINE T LengthSquared() const
    {
        return X * X + Y * Y + Z * Z;
    }

    NODISCARD FORCEINLINE T Length() const
    {
        return sqrtf(LengthSquared());
    }

    FORCEINLINE void Normalize()
    {
        T length = Length();
        X /= length;
        Y /= length;
        Z /= length;
    }

    NODISCARD FORCEINLINE Vector3 Normalized() const
    {
        Vector2 result = *this;
        result.Normalize();
        return result;
    }

    NODISCARD FORCEINLINE bool IsNormalized() const
    {
        return LengthSquared() - 1.0f < 0.0001f;
    }
    
    // Calculate the dot product between this vector and another.
    NODISCARD FORCEINLINE T Dot(const Vector3 &other, bool normalise = true) const
    {
        if (normalise)
            return Normalized().Dot(other.Normalized(), false);
        
        return (X * other.X)
            + (Y * other.Y)
            + (Z * other.Z);
    }
    
    FORCEINLINE T DistanceSquared(const Vector3 &other) const
    {
        return (*this - other).LengthSquared();
    }
    
    FORCEINLINE T Distance(const Vector3 &other) const
    {
        return (*this - other).Length();
    }

    FORCEINLINE Vector3 Cross(const Vector3 &other) const
    {
        return
        {
            (Y * other.Z) - (Z * other.Y),
            (Z * other.X) - (X * other.Z),
            (X * other.Y) - (Y * other.X)
        };
    }
    
    FORCEINLINE void Zero()
    {
        X = 0;
        Y = 0;
        Z = 0;
    }
    
    FORCEINLINE bool IsValid() const
    {
        if (X * 0.0f != X * 0.0f || std::isinf(X))
            return false;
        if (Y * 0.0f != Y * 0.0f || std::isinf(Y))
            return false;
        if (Z * 0.0f != Z * 0.0f || std::isinf(Z))
            return false;
        
        return true;
    }

    // Casting operators
    template<typename OT>
    FORCEINLINE explicit operator Vector2<OT>() const
    {
        return { static_cast<OT>(X), static_cast<OT>(Y) };
    }

    template<typename OT>
    FORCEINLINE explicit operator Vector4<OT>() const
    {
        return { static_cast<OT>(X), static_cast<OT>(Y), static_cast<OT>(Z), 0 };
    }
};

template<typename T>
	requires std::is_arithmetic_v<T>
struct Vector4
{
public:
    union { T X, R; };
    union { T Y, G; };
    union { T Z, B; };
    union { T W, A; };

    Vector4()
        : X(0), Y(0), Z(0), W(0)
    { }
    
    Vector4(T scalar)
        : X(scalar), Y(scalar), Z(scalar), W(scalar)
    { }

    Vector4(T x, T y, T z, T w)
        : X(x), Y(y), Z(z), W(w)
    { }

    template<typename OT>
    Vector4(Vector2<OT> xy, T z, T w)
        : X(static_cast<T>(xy.X)), Y(static_cast<T>(xy.Y)), Z(z), W(w)
    { }

    template<typename OT>
    Vector4(Vector2<OT> xy, Vector2<T> zw)
        : X(static_cast<T>(xy.X)), Y(static_cast<T>(xy.Y)), Z(static_cast<T>(zw.X)), W(static_cast<T>(zw.Y))
    { }

    template<typename OT>
    Vector4(Vector3<OT> xyz, T w)
        : X(static_cast<T>(xyz.X)), Y(static_cast<T>(xyz.Y)), Z(static_cast<T>(xyz.Z)), W(w)
    { }

    bool operator==(const Vector4 &other) const
    {
        return X == other.X && Y == other.Y && Z == other.Z && W == other.W;
    }

    bool operator!=(const Vector4 &other) const
    {
        return !(*this == other);
    }
    
    Vector4 operator+(const Vector4 &other) const
    {
        return { X + other.X, Y + other.Y, Z + other.Z, W + other.W };
    }

    Vector4 operator-(const Vector4 &other) const
    {
        return { X - other.X, Y - other.Y, Z - other.Z, W - other.W };
    }
    
    Vector4 operator*(const Vector4 &other) const
    {
        return { X * other.X, Y * other.Y, Z * other.Z, W * other.W };
    }

    Vector4 operator*(T scalar) const
    {
        return { X * scalar, Y * scalar, Z * scalar, W * scalar };
    }

    Vector4 operator/(const Vector4 &other) const
    {
        return { X / other.X, Y / other.Y, Z / other.Z, W / other.W };
    }

    Vector4 operator/(T scalar) const
    {
        return { X / scalar, Y / scalar, Z / scalar, W / scalar };
    }
    
    void operator+=(const Vector4 &other)
    {
        X += other.X;
        Y += other.Y;
        Z += other.Z;
        W += other.W;
    }

    void operator-=(const Vector4 &other)
    {
        X -= other.X;
        Y -= other.Y;
        Z -= other.Z;
        W -= other.W;
    }

    void operator*=(const Vector4 &other)
    {
        X *= other.X;
        Y *= other.Y;
        Z *= other.Z;
        W *= other.W;
    }

    void operator*=(T scalar)
    {
        X *= scalar;
        Y *= scalar;
        Z *= scalar;
        W *= scalar;
    }

    void operator/=(const Vector4 &other)
    {
        X /= other.X;
        Y /= other.Y;
        Z /= other.Z;
        W /= other.W;
    }

    void operator/=(T scalar)
    {
        X /= scalar;
        Y /= scalar;
        Z /= scalar;
        W /= scalar;
    }
    
    Vector4 operator-() const
    {
        return { -X, -Y, -Z, -W };
    }
    
    T operator[](int index) const
    {
        return (&X)[index];
    }

    T &operator[](int index)
    {
        return (&X)[index];
    }
    
    NODISCARD FORCEINLINE T* Data() { return &X; }
    
    NODISCARD FORCEINLINE T LengthSquared() const
    {
        return X * X + Y * Y + Z * Z + W * W;
    }

    NODISCARD FORCEINLINE T Length() const
    {
        return sqrtf(LengthSquared());
    }

    FORCEINLINE void Normalize()
    {
        T length = Length();
        X /= length;
        Y /= length;
        Z /= length;
        W /= length;
    }

    NODISCARD FORCEINLINE Vector4 Normalized() const
    {
        Vector2 result = *this;
        result.Normalize();
        return result;
    }

    NODISCARD FORCEINLINE bool IsNormalized() const
    {
        return LengthSquared() - 1.0f < 0.0001f;
    }
    
    // Calculate the dot product between this vector and another.
    NODISCARD FORCEINLINE T Dot(const Vector4 &other, bool normalise = true) const
    {
        if (normalise)
            return Normalized().Dot(other.Normalized(), false);
        
        return (X * other.X)
            + (Y * other.Y)
            + (Z * other.Z)
            + (W * other.W);
    }
    
    FORCEINLINE T DistanceSquared(const Vector4 &other) const
    {
        return (*this - other).LengthSquared();
    }
    
    FORCEINLINE T Distance(const Vector4 &other) const
    {
        return (*this - other).Length();
    }

    FORCEINLINE void Zero()
    {
        X = 0;
        Y = 0;
        Z = 0;
        W = 0;
    }
    
    FORCEINLINE bool IsValid() const
    {
        if (X * 0.0f != X * 0.0f || std::isinf(X))
            return false;
        if (Y * 0.0f != Y * 0.0f || std::isinf(Y))
            return false;
        if (Z * 0.0f != Z * 0.0f || std::isinf(Z))
            return false;
        if (W * 0.0f != W * 0.0f || std::isinf(W))
            return false;
        
        return true;
    }

    // Casting operators
    template<typename OT>
    FORCEINLINE explicit operator Vector2<OT>() const
    {
        return { static_cast<OT>(X), static_cast<OT>(Y) };
    }

    template<typename OT>
    FORCEINLINE explicit operator Vector3<OT>() const
    {
        return { static_cast<OT>(X), static_cast<OT>(Y), static_cast<OT>(Z) };
    }
};

using Vector2I = Vector2<s32>;
using Vector2F = Vector2<f32>;
using Vector2D = Vector2<f64>;

using Vector3I = Vector3<s32>;
using Vector3F = Vector3<f32>;
using Vector3D = Vector3<f64>;

using Vector4I = Vector4<s32>;
using Vector4F = Vector4<f32>;
using Vector4D = Vector4<f64>;

// Hashing implementations.
namespace std
{
    template <>
    struct hash<Vector2I>
    {
        std::size_t operator()(const Vector2I &v) const noexcept
        {
            // Combine the two integers (x, y) into a single hash value.
            // Use a prime number multiplier for better distribution.
            size_t h1 = std::hash<int>{}(v.X);
            size_t h2 = std::hash<int>{}(v.Y);
            return h1 ^ (h2 << 1); // Combine the two hash values
        }
    };
}
