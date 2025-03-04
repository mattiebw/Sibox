﻿#pragma once

template<typename T>
struct Vector2
{
public:
    T X, Y;

    Vector2(T scalar)
        : X(scalar), Y(scalar)
    { }

    Vector2(T x, T y)
        : X(x), Y(y)
    { }

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

    Vector2 operator/(const Vector2 &other) const
    {
        return { X / other.X, Y / other.Y };
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

    void operator/=(const Vector2 &other)
    {
        X /= other.X;
        Y /= other.Y;
    }

    NODISCARD FORCEINLINE float LengthSquared() const
    {
        return X * X + Y * Y;
    }

    NODISCARD FORCEINLINE float Length() const
    {
        return sqrtf(LengthSquared());
    }

    FORCEINLINE void Normalize()
    {
        this /= Length();
    }

    NODISCARD FORCEINLINE Vector2 Normalized() const
    {
        return *this / Length();
    }
    
    // Calculate the dot product between this vector and another. We assume that both vectors are normalised.
    NODISCARD FORCEINLINE float Dot(const Vector2 &other) const
    {
        return (X * other.X)
            + (Y * other.Y);
    }
};

template<typename T>
struct Vector3
{
public:
    T X, Y, Z;

    Vector3(T scalar)
        : X(scalar), Y(scalar), Z(scalar)
    { }

    Vector3(T x, T y, T z)
        : X(x), Y(y), Z(z)
    { }

    Vector3(Vector2<T> xy, T z)
        : X(xy.X), Y(xy.Y), Z(z)
    { }

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

    Vector3 operator/(const Vector3 &other) const
    {
        return { X / other.X, Y / other.Y, Z / other.Z };
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

    void operator/=(const Vector3 &other)
    {
        X /= other.X;
        Y /= other.Y;
        Z /= other.Z;
    }
    
    NODISCARD FORCEINLINE float LengthSquared() const
    {
        return X * X + Y * Y + Z * Z;
    }

    NODISCARD FORCEINLINE float Length() const
    {
        return sqrtf(LengthSquared());
    }

    FORCEINLINE void Normalize()
    {
        this /= Length();
    }

    NODISCARD FORCEINLINE Vector3 Normalized() const
    {
        return *this / Length();
    }
    
    // Calculate the dot product between this vector and another. We assume that both vectors are normalised.
    NODISCARD FORCEINLINE float Dot(const Vector3 &other) const
    {
        return (X * other.X)
            + (Y * other.Y)
            + (Z * other.Z);
    }
};

template<typename T>
struct Vector4
{
public:
    T X, Y, Z, W;

    Vector4(T scalar)
        : X(scalar), Y(scalar), Z(scalar), W(scalar)
    { }

    Vector4(T x, T y, T z, T w)
        : X(x), Y(y), Z(z), W(w)
    { }
    
    Vector4(Vector2<T> xy, T z, T w)
        : X(xy.X), Y(xy.Y), Z(z), W(w)
    { }

    Vector4(Vector2<T> xy, Vector2<T> zw)
        : X(xy.X), Y(xy.Y), Z(zw.X), W(zw.Y)
    { }
    
    Vector4(Vector3<T> xyz, T w)
        : X(xyz.X), Y(xyz.Y), Z(xyz.Z), W(w)
    { }

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

    Vector4 operator/(const Vector4 &other) const
    {
        return { X / other.X, Y / other.Y, Z / other.Z, W / other.W };
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

    void operator/=(const Vector4 &other)
    {
        X /= other.X;
        Y /= other.Y;
        Z /= other.Z;
        W /= other.W;
    }
    
    NODISCARD FORCEINLINE float LengthSquared() const
    {
        return X * X + Y * Y + Z * Z + W * W;
    }

    NODISCARD FORCEINLINE float Length() const
    {
        return sqrtf(LengthSquared());
    }

    FORCEINLINE void Normalize()
    {
        this /= Length();
    }

    NODISCARD FORCEINLINE Vector4 Normalized() const
    {
        return *this / Length();
    }

    // Calculate the dot product between this vector and another. We assume that both vectors are normalised.
    NODISCARD FORCEINLINE float Dot(const Vector4 &other) const
    {
        return (X * other.X)
            + (Y * other.Y)
            + (Z * other.Z)
            + (W * other.W);
    }
};

typedef Vector2<int> Vector2i;
typedef Vector2<float> Vector2f;
typedef Vector2<double> Vector2d;

typedef Vector3<int> Vector3i;
typedef Vector3<float> Vector3f;
typedef Vector3<double> Vector3d;

typedef Vector4<int> Vector4i;
typedef Vector4<float> Vector4f;
typedef Vector4<double> Vector4d;
