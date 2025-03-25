#pragma once

// Templated square matrix class.
template <typename T, int Size>
	requires std::is_arithmetic_v<T>
struct Matrix
{
public:
	// ---- Constructors ----
	// Create an identity matrix.
	Matrix()
	{
		Identity();
	}

	// Create a matrix with the diagonal set to the given value.
	Matrix(T diagonal)
	{
		Zero();
		for (int i = 0; i < Size; i++)
		{
			m_Data[i][i] = diagonal;
		}
	}

	static Matrix<T, 4> MakePerspective(T fov, T aspect, T near, T far)
	{
		Matrix<T, 4> result;
		return result;
	}

	static Matrix<T, 4> MakeOrthographic(T left, T right, T bottom, T top, T near, T far)
	{
		Matrix<T, 4> result;
		return result;
	}

	template<typename OT>
	explicit operator Matrix<OT, Size>() const
	{
		Matrix<OT, Size> result;
		for (int i = 0; i < Size; i++)
			for (int j = 0; j < Size; j++)
				result[i][j] = static_cast<OT>(m_Data[i][j]);
		return result;
	}

	// ------- Arithmetic ------
	// Basic matrix addition.
	Matrix<T, Size> operator+(const Matrix<T, Size> &other) const
	{
		Matrix<T, Size> result = *this;

		for (int i = 0; i < Size; i++)
			for (int j = 0; j < Size; j++)
				result.m_Data[i][j] += other.m_Data[i][j];

		return result;
	}

	// Basic matrix subtraction.
	Matrix<T, Size> operator-(const Matrix<T, Size> &other) const
	{
		Matrix<T, Size> result = *this;

		for (int i = 0; i < Size; i++)
			for (int j = 0; j < Size; j++)
				result.m_Data[i][j] -= other.m_Data[i][j];

		return result;
	}

	// Basic matrix multiplication.
	Matrix<T, Size> operator*(const Matrix<T, Size> &other) const
	{
		Matrix<T, Size> result;

		for (int i = 0; i < Size; i++)
		{
			for (int j = 0; j < Size; j++)
			{
				result.m_Data[i][j] = 0;

				for (int k = 0; k < Size; k++)
					result.m_Data[i][j] += m_Data[i][k] * other.m_Data[k][j];
			}
		}

		// 2x2 matrix unwrap
		// result.m_Data[0][0] = m_Data[0][0] * other.m_Data[0][0] + m_Data[0][1] * other.m_Data[1][0];
		// result.m_Data[0][1] = m_Data[0][0] * other.m_Data[0][1] + m_Data[0][1] * other.m_Data[1][1];
		// result.m_Data[1][0] = m_Data[1][0] * other.m_Data[0][0] + m_Data[1][1] * other.m_Data[1][0];
		// result.m_Data[1][1] = m_Data[1][0] * other.m_Data[0][1] + m_Data[1][1] * other.m_Data[1][1];

		// 3x3 matrix unwrap
		// result.m_Data[0][0] = m_Data[0][0] * other.m_Data[0][0] + m_Data[0][1] * other.m_Data[1][0] + m_Data[0][2] * other.m_Data[2][0];
		// result.m_Data[0][1] = m_Data[0][0] * other.m_Data[0][1] + m_Data[0][1] * other.m_Data[1][1] + m_Data[0][2] * other.m_Data[2][1];
		// result.m_Data[0][2] = m_Data[0][0] * other.m_Data[0][2] + m_Data[0][1] * other.m_Data[1][2] + m_Data[0][2] * other.m_Data[2][2];
		// result.m_Data[1][0] = m_Data[1][0] * other.m_Data[0][0] + m_Data[1][1] * other.m_Data[1][0] + m_Data[1][2] * other.m_Data[2][0];
		// result.m_Data[1][1] = m_Data[1][0] * other.m_Data[0][1] + m_Data[1][1] * other.m_Data[1][1] + m_Data[1][2] * other.m_Data[2][1];
		// result.m_Data[1][2] = m_Data[1][0] * other.m_Data[0][2] + m_Data[1][1] * other.m_Data[1][2] + m_Data[1][2] * other.m_Data[2][2];
		// result.m_Data[2][0] = m_Data[2][0] * other.m_Data[0][0] + m_Data[2][1] * other.m_Data[1][0] + m_Data[2][2] * other.m_Data[2][0];
		// result.m_Data[2][1] = m_Data[2][0] * other.m_Data[0][1] + m_Data[2][1] * other.m_Data[1][1] + m_Data[2][2] * other.m_Data[2][1];
		// result.m_Data[2][2] = m_Data[2][0] * other.m_Data[0][2] + m_Data[2][1] * other.m_Data[1][2] + m_Data[2][2] * other.m_Data[2][2];

		return result;
	}

	void operator+=(const Matrix<T, Size> &other)
	{
		for (int i = 0; i < Size; i++)
			for (int j = 0; j < Size; j++)
				m_Data[i][j] += other.m_Data[i][j];
	}

	void operator-=(const Matrix<T, Size> &other)
	{
		for (int i = 0; i < Size; i++)
			for (int j = 0; j < Size; j++)
				m_Data[i][j] -= other.m_Data[i][j];
	}

	void operator*=(const Matrix<T, Size> &other)
	{
		*this = *this * other;
	}

	// Multiply a matrix by a 2-element vector. Only works when the matrix is 2x2.
	Vector2<T> operator*(const Vector2<T> &vector) const
	{
		static_assert(Size == 2, "Matrix must be 2x2 to multiply with a Vector2.");

		Vector2<T> result;

		result.X = m_Data[0][0] * vector.X + m_Data[0][1] * vector.Y;
		result.Y = m_Data[1][0] * vector.X + m_Data[1][1] * vector.Y;

		return result;
	}

	// Multiply a matrix by a 3-element vector. Only works when the matrix is 3x3.
	Vector3<T> operator*(const Vector3<T> &vector) const
	{
		static_assert(Size == 3, "Matrix must be 3x3 to multiply with a Vector3.");

		Vector3<T> result;

		result.X = m_Data[0][0] * vector.X + m_Data[0][1] * vector.Y + m_Data[0][2] * vector.Z;
		result.Y = m_Data[1][0] * vector.X + m_Data[1][1] * vector.Y + m_Data[1][2] * vector.Z;
		result.Z = m_Data[2][0] * vector.X + m_Data[2][1] * vector.Y + m_Data[2][2] * vector.Z;

		return result;
	}

	// Multiply a matrix by a 4-element vector. Only works when the matrix is 4x4.
	Vector4<T> operator*(const Vector4<T> &vector) const
	{
		static_assert(Size == 4, "Matrix must be 4x4 to multiply with a Vector4.");

		Vector4<T> result;

		result.X = m_Data[0][0] * vector.X + m_Data[0][1] * vector.Y + m_Data[0][2] * vector.Z + m_Data[0][3] * vector.
			W;
		result.Y = m_Data[1][0] * vector.X + m_Data[1][1] * vector.Y + m_Data[1][2] * vector.Z + m_Data[1][3] * vector.
			W;
		result.Z = m_Data[2][0] * vector.X + m_Data[2][1] * vector.Y + m_Data[2][2] * vector.Z + m_Data[2][3] * vector.
			W;
		result.W = m_Data[3][0] * vector.X + m_Data[3][1] * vector.Y + m_Data[3][2] * vector.Z + m_Data[3][3] * vector.
			W;

		return result;
	}

	// ------- Accessors -------
	FORCEINLINE T* operator[](int index)
	{
		return m_Data[index];
	}

	FORCEINLINE const T* operator[](int index) const
	{
		return m_Data[index];
	}

	NODISCARD FORCEINLINE const T* Data() const
	{
		return &m_Data[0][0];
	}

	// ------ Operations -------
	FORCEINLINE void Zero()
	{
		memset(m_Data, 0, sizeof(m_Data));
	}

	FORCEINLINE void Identity()
	{
		Zero();
		for (int i = 0; i < Size; i++)
		{
			m_Data[i][i] = 1;
		}
	}

	Matrix<T, Size> Transpose() const
	{
		Matrix<T, Size> result;

		for (int i = 0; i < Size; i++)
			for (int j              = 0; j < Size; j++)
				result.m_Data[i][j] = m_Data[j][i];

		return result;
	}

	void Invert()
	{
		
	}

	NODISCARD FORCEINLINE Matrix Inverse() const
	{
		Matrix result = *this;
		result.Invert();
		return std::move(result);
	}

	// ---- 2D Transformations ----

	// ---- 3D Transformations ----
	void Translate(Vector3<T> translation)
	{
		static_assert(Size == 4, "Matrix must be 4x4 to translate by a 3-element vector.");
	}

	void Rotate(Vector3<T> rotation)
	{
		static_assert(Size == 4, "Matrix must be 4x4 to rotate by a 3-element vector.");
	}

	void Scale(Vector3<T> scale)
	{
		static_assert(Size == 4, "Matrix must be 4x4 to scale by a 3-element vector.");
	}

private:
	T m_Data[Size][Size];
};

using Matrix2x2  = Matrix<f32, 2>;
using Matrix2x2I = Matrix<int, 2>;
using Matrix2x2F = Matrix<f32, 2>;
using Matrix2x2D = Matrix<f64, 2>;

using Matrix3x3  = Matrix<f32, 3>;
using Matrix3x3I = Matrix<int, 3>;
using Matrix3x3F = Matrix<f32, 3>;
using Matrix3x3D = Matrix<f64, 3>;

using Matrix4x4  = Matrix<f32, 4>;
using Matrix4x4I = Matrix<int, 4>;
using Matrix4x4F = Matrix<f32, 4>;
using Matrix4x4D = Matrix<f64, 4>;

// // MW @credit: Thanks to physics in a weekend book for recommending use of array of vectors instead of 2d array of T.
// template<typename T = f32>
// struct Matrix2x2
// {
//     // Create an identity matrix.
//     Matrix2x2()
//         : m_Data({{1, 0}, {0, 1}})
//     { }
//
//     Matrix2x2(T diagonal)
//         : m_Data({{diagonal, 0}, {0, diagonal}})
//     { }
//
//     Matrix2x2(Vector2<T> row1, Vector2<T> row2)
//         : m_Data({row1, row2})
//     { }
//
//     Matrix2x2 operator+(const Matrix2x2 &other) const
//     {
//         Matrix2x2 result = *this;
//
//         result.m_Data[0] += other.m_Data[0];
//         result.m_Data[1] += other.m_Data[1];
//         
//         return result;
//     }
//
//     Matrix2x2 operator-(const Matrix2x2 &other) const
//     {
//         Matrix2x2 result = *this;
//
//         result.m_Data[0] -= other.m_Data[0];
//         result.m_Data[1] -= other.m_Data[1];
//         
//         return result;
//     }
//
//     void operator+=(const Matrix2x2 &other)
//     {
//         m_Data[0] += other.m_Data[0];
//         m_Data[1] += other.m_Data[1];
//     }
//
//     void operator-=(const Matrix2x2 &other)
//     {
//         m_Data[0] -= other.m_Data[0];
//         m_Data[1] -= other.m_Data[1];
//     }
//
//     Matrix2x2 operator*(const Matrix2x2 &other) const
//     {
//         Matrix2x2 result;
//
//         // result[0][0] = m_Data[0][0] * other[0][0] + m_Data[0][1] * other[1][0];
//         // result[0][1] = m_Data[0][0] * other[0][1] + m_Data[0][1] * other[1][1];
//         // result[1][0] = m_Data[1][0] * other[0][0] + m_Data[1][1] * other[1][0];
//         // result[1][1] = m_Data[1][0] * other[0][1] + m_Data[1][1] * other[1][1];
//
//         for (int i = 0; i < 2; i++)
//         {
//             result[i][0] = m_Data[i][0] * other[0][0] + m_Data[i][1] * other[1][0];
//             result[i][1] = m_Data[i][0] * other[0][1] + m_Data[i][1] * other[1][1];
//         }
//         
//         return result;
//     }
//
//     Vector2<T> operator*(const Vector2<T> &vector) const
//     {
//         return Vector2<T>
//         {
//             m_Data[0].Dot(vector),
//             m_Data[1].Dot(vector)
//         };
//         
//         // return Vector2<T>
//         // {
//         //     (m_Data[0][0] * vector.X) + (m_Data[0][1] * vector.Y),
//         //     (m_Data[1][0] * vector.X) + (m_Data[1][1] * vector.Y)
//         // };
//     }
//
//     Matrix2x2 operator*(T scalar) const
//     {
//         Matrix2x2 result = *this;
//
//         result.m_Data[0] *= scalar;
//         result.m_Data[1] *= scalar;
//         
//         return result;
//     }
//
//     void operator*=(T scalar)
//     {
//         m_Data[0] *= scalar;
//         m_Data[1] *= scalar;
//     }
//
//     void operator*=(const Matrix2x2 &other)
//     {
//         *this = *this * other;
//     }
//     
//     FORCEINLINE Vector2<T> &operator[](int index)
//     {
//         return m_Data[index];
//     }
//
//     FORCEINLINE const Vector2<T> &operator[](int index) const
//     {
//         return m_Data[index];
//     }
//
//     FORCEINLINE void Set(int row, int column, T value)
//     {
//         m_Data[row][column] = value;
//     }
//
//     // MW @todo: Will the compiler optimise this to a memset?
//     FORCEINLINE void Zero()
//     {
//         m_Data[0].Zero();
//         m_Data[1].Zero();
//     }
//     
// private:
//     Vector2<T> m_Data[2];
// };
//
// typedef Matrix2x2<int> Matrix2x2i;
// typedef Matrix2x2<f32> Matrix2x2f;
// typedef Matrix2x2<f64> Matrix2x2d;
