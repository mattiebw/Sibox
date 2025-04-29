#pragma once

// Templated square matrix class.
template <typename T, s32 Size>
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
		for (s32 i = 0; i < Size; i++)
		{
			m_Data[i][i] = diagonal;
		}
	}

	Matrix(Vector4<T> a, Vector4<T> b, Vector4<T> c, Vector4<T> d)
		: m_Data{{a.X, a.Y, a.Z, a.W}, {b.X, b.Y, b.Z, b.W}, {c.X, c.Y, c.Z, c.W}, {d.X, d.Y, d.Z, d.W}}
	{
		static_assert(Size == 4);
	}

	static Matrix<T, 4> MakePerspective(T verticalFov, T aspect, T near, T far)
	{
		Matrix<T, 4> result;
		T const tanOfHalfVerticalFov = tan(verticalFov / static_cast<T>(2));
		result[0][0] = static_cast<T>(1) / (aspect * tanOfHalfVerticalFov);
		result[1][1] = static_cast<T>(1) / (tanOfHalfVerticalFov);
		result[2][2] = far / (far - near);
		result[2][3] = static_cast<T>(1);
		result[3][2] = -(far * near) / (far - near);
		return result;
	}

	static Matrix<T, 4> MakeOrthographic(T left, T right, T bottom, T top, T near, T far)
	{
		// Thanks to https://en.wikipedia.org/wiki/Orthographic_projection
		Matrix<T, 4> result;
		result[0][0] = 2 / (right - left);
		result[1][1] = 2 / (top - bottom);
		result[2][2] = -2 / (far - near);
		result[3][0] = -((right + left) / (right - left));
		result[3][1] = -((top + bottom) / (top - bottom));
		result[3][2] = -((far + near) / (far - near));
		return result;
	}

	template <typename OT>
	explicit operator Matrix<OT, Size>() const
	{
		Matrix<OT, Size> result;
		for (s32 i = 0; i < Size; i++)
			for (s32 j       = 0; j < Size; j++)
				result[i][j] = static_cast<OT>(m_Data[i][j]);
		return result;
	}

	// ------- Arithmetic ------
	// Basic matrix addition.
	Matrix<T, Size> operator+(const Matrix<T, Size> &other) const
	{
		Matrix<T, Size> result = *this;

		for (s32 i = 0; i < Size; i++)
			for (s32 j = 0; j < Size; j++)
				result.m_Data[i][j] += other.m_Data[i][j];

		return result;
	}

	// Basic matrix subtraction.
	Matrix<T, Size> operator-(const Matrix<T, Size> &other) const
	{
		Matrix<T, Size> result = *this;

		for (s32 i = 0; i < Size; i++)
			for (s32 j = 0; j < Size; j++)
				result.m_Data[i][j] -= other.m_Data[i][j];

		return result;
	}

	// Basic matrix multiplication.
	Matrix<T, Size> operator*(const Matrix<T, Size> &other) const
	{
		Matrix<T, Size> result;

		if constexpr (Size == 2)
		{
			float a00 = m_Data[0][0];
			float a01 = m_Data[0][1];

			float a10 = m_Data[1][0];
			float a11 = m_Data[1][1];

			float b00 = other.m_Data[0][0];
			float b01 = other.m_Data[0][1];

			float b10 = other.m_Data[1][0];
			float b11 = other.m_Data[1][1];
			
			result[0][0] = (a00 * b00) + (a10 * b01);
			result[0][1] = (a01 * b00) + (a11 * b01);
			
			result[1][0] = (a00 * b10) + (a10 * b11);
			result[1][1] = (a01 * b10) + (a11 * b11);
		}
		else if constexpr (Size == 3)
		{
			float a00 = m_Data[0][0];
			float a01 = m_Data[0][1];
			float a02 = m_Data[0][2];

			float a10 = m_Data[1][0];
			float a11 = m_Data[1][1];
			float a12 = m_Data[1][2];

			float a20 = m_Data[2][0];
			float a21 = m_Data[2][1];
			float a22 = m_Data[2][2];
			
			float b00 = other.m_Data[0][0];
			float b01 = other.m_Data[0][1];
			float b02 = other.m_Data[0][2];

			float b10 = other.m_Data[1][0];
			float b11 = other.m_Data[1][1];
			float b12 = other.m_Data[1][2];

			float b20 = other.m_Data[2][0];
			float b21 = other.m_Data[2][1];
			float b22 = other.m_Data[2][2];
			
			result[0][0] = (a00 * b00) + (a10 * b01) + (a20 * b02);
			result[0][1] = (a01 * b00) + (a11 * b01) + (a21 * b02);
			result[0][2] = (a02 * b00) + (a12 * b01) + (a22 * b02);
			
			result[1][0] = (a00 * b10) + (a10 * b11) + (a20 * b12);
			result[1][1] = (a01 * b10) + (a11 * b11) + (a21 * b12);
			result[1][2] = (a02 * b10) + (a12 * b11) + (a22 * b12);
			
			result[2][0] = (a00 * b20) + (a10 * b21) + (a20 * b22);
			result[2][1] = (a01 * b20) + (a11 * b21) + (a21 * b22);
			result[2][2] = (a02 * b20) + (a12 * b21) + (a22 * b22);
		}
		else if constexpr (Size == 4)
		{
			float a00 = m_Data[0][0];
			float a01 = m_Data[0][1];
			float a02 = m_Data[0][2];
			float a03 = m_Data[0][3];

			float a10 = m_Data[1][0];
			float a11 = m_Data[1][1];
			float a12 = m_Data[1][2];
			float a13 = m_Data[1][3];

			float a20 = m_Data[2][0];
			float a21 = m_Data[2][1];
			float a22 = m_Data[2][2];
			float a23 = m_Data[2][3];

			float a30 = m_Data[3][0];
			float a31 = m_Data[3][1];
			float a32 = m_Data[3][2];
			float a33 = m_Data[3][3];

			float b00 = other.m_Data[0][0];
			float b01 = other.m_Data[0][1];
			float b02 = other.m_Data[0][2];
			float b03 = other.m_Data[0][3];

			float b10 = other.m_Data[1][0];
			float b11 = other.m_Data[1][1];
			float b12 = other.m_Data[1][2];
			float b13 = other.m_Data[1][3];

			float b20 = other.m_Data[2][0];
			float b21 = other.m_Data[2][1];
			float b22 = other.m_Data[2][2];
			float b23 = other.m_Data[2][3];

			float b30 = other.m_Data[3][0];
			float b31 = other.m_Data[3][1];
			float b32 = other.m_Data[3][2];
			float b33 = other.m_Data[3][3];

			result[0][0] = (a00 * b00) + (a10 * b01) + (a20 * b02) + (a30 * b03);
			result[0][1] = (a01 * b00) + (a11 * b01) + (a21 * b02) + (a31 * b03);
			result[0][2] = (a02 * b00) + (a12 * b01) + (a22 * b02) + (a32 * b03);
			result[0][3] = (a03 * b00) + (a13 * b01) + (a23 * b02) + (a33 * b03);

			result[1][0] = (a00 * b10) + (a10 * b11) + (a20 * b12) + (a30 * b13);
			result[1][1] = (a01 * b10) + (a11 * b11) + (a21 * b12) + (a31 * b13);
			result[1][2] = (a02 * b10) + (a12 * b11) + (a22 * b12) + (a32 * b13);
			result[1][3] = (a03 * b10) + (a13 * b11) + (a23 * b12) + (a33 * b13);

			result[2][0] = (a00 * b20) + (a10 * b21) + (a20 * b22) + (a30 * b23);
			result[2][1] = (a01 * b20) + (a11 * b21) + (a21 * b22) + (a31 * b23);
			result[2][2] = (a02 * b20) + (a12 * b21) + (a22 * b22) + (a32 * b23);
			result[2][3] = (a03 * b20) + (a13 * b21) + (a23 * b22) + (a33 * b23);

			result[3][0] = (a00 * b30) + (a10 * b31) + (a20 * b32) + (a30 * b33);
			result[3][1] = (a01 * b30) + (a11 * b31) + (a21 * b32) + (a31 * b33);
			result[3][2] = (a02 * b30) + (a12 * b31) + (a22 * b32) + (a32 * b33);
			result[3][3] = (a03 * b30) + (a13 * b31) + (a23 * b32) + (a33 * b33);
		}

		return result;
	}

	void operator+=(const Matrix<T, Size> &other)
	{
		for (s32 i = 0; i < Size; i++)
			for (s32 j = 0; j < Size; j++)
				m_Data[i][j] += other.m_Data[i][j];
	}

	void operator-=(const Matrix<T, Size> &other)
	{
		for (s32 i = 0; i < Size; i++)
			for (s32 j = 0; j < Size; j++)
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

		result.X = m_Data[0][0] * vector.X + m_Data[1][0] * vector.Y;
		result.Y = m_Data[0][1] * vector.X + m_Data[1][1] * vector.Y;

		return result;
	}

	// Multiply a matrix by a 3-element vector. Only works when the matrix is 3x3.
	Vector3<T> operator*(const Vector3<T> &vector) const
	{
		static_assert(Size == 3, "Matrix must be 3x3 to multiply with a Vector3.");
		
		return {
			m_Data[0][0] * vector.X + m_Data[1][0] * vector.Y + m_Data[2][0] * vector.Z,
			m_Data[0][1] * vector.X + m_Data[1][1] * vector.Y + m_Data[2][1] * vector.Z,
			m_Data[0][2] * vector.X + m_Data[1][2] * vector.Y + m_Data[2][2] * vector.Z,
		};
	}

	// Multiply a matrix by a 4-element vector. Only works when the matrix is 4x4.
	Vector4<T> operator*(const Vector4<T> &vector) const
	{
		static_assert(Size == 4, "Matrix must be 4x4 to multiply with a Vector4.");

		return {
			m_Data[0][0] * vector.X + m_Data[1][0] * vector.Y + m_Data[2][0] * vector.Z + m_Data[3][0] * vector.W,
			m_Data[0][1] * vector.X + m_Data[1][1] * vector.Y + m_Data[2][1] * vector.Z + m_Data[3][1] * vector.W,
			m_Data[0][2] * vector.X + m_Data[1][2] * vector.Y + m_Data[2][2] * vector.Z + m_Data[3][2] * vector.W,
			m_Data[0][3] * vector.X + m_Data[1][3] * vector.Y + m_Data[2][3] * vector.Z + m_Data[3][3] * vector.W
		};
	}

	template <typename MT>
		requires std::is_floating_point_v<MT>
	Matrix<T, Size> operator*(MT other) const
	{
		Matrix<T, Size> out = *this;
		for (s32 i = 0; i < Size; i++)
			for (s32 j = 0; j < Size; j++)
				out.m_Data[i][j] *= other;
		return out;
	}

	// ------- Accessors -------
	FORCEINLINE T* operator[](s32 index)
	{
		return m_Data[index];
	}

	FORCEINLINE const T* operator[](s32 index) const
	{
		return m_Data[index];
	}

	FORCEINLINE T Get(s32 row, s32 column) const
	{
		return m_Data[column][row];
	}

	FORCEINLINE void Set(s32 row, s32 column, T value)
	{
		m_Data[column][row] = value;
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
		for (s32 i = 0; i < Size; i++)
		{
			m_Data[i][i] = 1;
		}
	}

	Matrix<T, Size> Transpose() const
	{
		Matrix<T, Size> result;

		for (s32 i = 0; i < Size; i++)
			for (s32 j              = 0; j < Size; j++)
				result.m_Data[i][j] = m_Data[j][i];

		return result;
	}

	void Invert()
	{
		*this = Inverse();
	}

	NODISCARD FORCEINLINE Matrix Inverse() const
	{
		static_assert(Size == 4, "Matrix must be 4x4 to invert.");

		T Coef00 = m_Data[2][2] * m_Data[3][3] - m_Data[3][2] * m_Data[2][3];
		T Coef02 = m_Data[1][2] * m_Data[3][3] - m_Data[3][2] * m_Data[1][3];
		T Coef03 = m_Data[1][2] * m_Data[2][3] - m_Data[2][2] * m_Data[1][3];

		T Coef04 = m_Data[2][1] * m_Data[3][3] - m_Data[3][1] * m_Data[2][3];
		T Coef06 = m_Data[1][1] * m_Data[3][3] - m_Data[3][1] * m_Data[1][3];
		T Coef07 = m_Data[1][1] * m_Data[2][3] - m_Data[2][1] * m_Data[1][3];

		T Coef08 = m_Data[2][1] * m_Data[3][2] - m_Data[3][1] * m_Data[2][2];
		T Coef10 = m_Data[1][1] * m_Data[3][2] - m_Data[3][1] * m_Data[1][2];
		T Coef11 = m_Data[1][1] * m_Data[2][2] - m_Data[2][1] * m_Data[1][2];

		T Coef12 = m_Data[2][0] * m_Data[3][3] - m_Data[3][0] * m_Data[2][3];
		T Coef14 = m_Data[1][0] * m_Data[3][3] - m_Data[3][0] * m_Data[1][3];
		T Coef15 = m_Data[1][0] * m_Data[2][3] - m_Data[2][0] * m_Data[1][3];

		T Coef16 = m_Data[2][0] * m_Data[3][2] - m_Data[3][0] * m_Data[2][2];
		T Coef18 = m_Data[1][0] * m_Data[3][2] - m_Data[3][0] * m_Data[1][2];
		T Coef19 = m_Data[1][0] * m_Data[2][2] - m_Data[2][0] * m_Data[1][2];

		T Coef20 = m_Data[2][0] * m_Data[3][1] - m_Data[3][0] * m_Data[2][1];
		T Coef22 = m_Data[1][0] * m_Data[3][1] - m_Data[3][0] * m_Data[1][1];
		T Coef23 = m_Data[1][0] * m_Data[2][1] - m_Data[2][0] * m_Data[1][1];

		Vector4<T> Fac0(Coef00, Coef00, Coef02, Coef03);
		Vector4<T> Fac1(Coef04, Coef04, Coef06, Coef07);
		Vector4<T> Fac2(Coef08, Coef08, Coef10, Coef11);
		Vector4<T> Fac3(Coef12, Coef12, Coef14, Coef15);
		Vector4<T> Fac4(Coef16, Coef16, Coef18, Coef19);
		Vector4<T> Fac5(Coef20, Coef20, Coef22, Coef23);

		Vector4<T> Vec0(m_Data[1][0], m_Data[0][0], m_Data[0][0], m_Data[0][0]);
		Vector4<T> Vec1(m_Data[1][1], m_Data[0][1], m_Data[0][1], m_Data[0][1]);
		Vector4<T> Vec2(m_Data[1][2], m_Data[0][2], m_Data[0][2], m_Data[0][2]);
		Vector4<T> Vec3(m_Data[1][3], m_Data[0][3], m_Data[0][3], m_Data[0][3]);

		Vector4<T> Inv0(Vec1 * Fac0 - Vec2 * Fac1 + Vec3 * Fac2);
		Vector4<T> Inv1(Vec0 * Fac0 - Vec2 * Fac3 + Vec3 * Fac4);
		Vector4<T> Inv2(Vec0 * Fac1 - Vec1 * Fac3 + Vec3 * Fac5);
		Vector4<T> Inv3(Vec0 * Fac2 - Vec1 * Fac4 + Vec2 * Fac5);

		Vector4<T>   SignA(+1, -1, +1, -1);
		Vector4<T>   SignB(-1, +1, -1, +1);
		Matrix<T, 4> Inverse(Inv0 * SignA, Inv1 * SignB, Inv2 * SignA, Inv3 * SignB);

		Vector4<T> Row0(Inverse[0][0], Inverse[1][0], Inverse[2][0], Inverse[3][0]);

		Vector4<T> Dot0 = Vector4<T>(m_Data[0][0], m_Data[0][1], m_Data[0][2], m_Data[0][3]) * Row0;
		T          Dot1 = (Dot0.X + Dot0.Y) + (Dot0.Z + Dot0.W);

		T OneOverDeterminant = static_cast<T>(1) / Dot1;

		return Inverse * OneOverDeterminant;
	}

	// ---- 2D Transformations ----
	void Translate(Vector2<T> translation)
	{
		static_assert(Size == 4, "Matrix must be 4x4 to receive transformations.");

		Matrix<T, 4> translationMatrix;
		translationMatrix[3][0] = translation.X;
		translationMatrix[3][1] = translation.Y;
		*this *= translationMatrix;
	}

	// ---- 3D Transformations ----
	void Translate(Vector3<T> translation)
	{
		static_assert(Size == 4, "Matrix must be 4x4 to receive transformations.");

		Matrix<T, 4> translationMatrix;
		translationMatrix[3][0] = translation.X;
		translationMatrix[3][1] = translation.Y;
		translationMatrix[3][2] = translation.Z;

		// Originally, in all these functions, I had "*this *= translationMatrix", but that was wrong, as of course
		// that is the wrong way around for matrix multiplication! (The equivalent of *this = *this * translationMatrix,
		// which is in effect transforming the translationMatrix by the current matrix, rather than the other way around.)
		// I stepped through both this code and the same code using GLM, and it was clear that all the individual translation
		// matrices were correct, but there was an issue with how they were being applied.
		*this = translationMatrix * (*this);
	}

	void SetTranslation(Vector3<T> translation)
	{
		static_assert(Size == 4, "Matrix must be 4x4 to receive transformations.");

		m_Data[3][0] = translation.X;
		m_Data[3][1] = translation.Y;
		m_Data[3][2] = translation.Z;
	}

	NODISCARD static Matrix<T, 4> MakeYawRotationMatrix(T angleRadians)
	{
		T angleCos = cos(angleRadians);
		T angleSin = sin(angleRadians);

		Matrix<T, 4> rotationMatrix;
		rotationMatrix[0][0] = angleCos;
		rotationMatrix[0][2] = -angleSin;
		rotationMatrix[2][0] = angleSin;
		rotationMatrix[2][2] = angleCos;
		return rotationMatrix;
	}

	void RotateYaw(T angleRadians)
	{
		static_assert(Size == 4, "Matrix must be 4x4 to receive transformations.");

		*this *= MakeYawRotationMatrix(angleRadians);
	}

	NODISCARD static Matrix<T, 4> MakePitchRotationMatrix(T angleRadians)
	{
		T angleCos = cos(angleRadians);
		T angleSin = sin(angleRadians);

		Matrix<T, 4> rotationMatrix;
		rotationMatrix[1][1] = angleCos;
		rotationMatrix[1][2] = angleSin;
		rotationMatrix[2][1] = -angleSin;
		rotationMatrix[2][2] = angleCos;
		return rotationMatrix;
	}

	void RotatePitch(T angleRadians)
	{
		static_assert(Size == 4, "Matrix must be 4x4 to receive transformations.");

		*this *= MakePitchRotationMatrix(angleRadians);
	}

	NODISCARD static Matrix<T, 4> MakeRollRotationMatrix(T angleRadians)
	{
		T angleCos = cos(angleRadians);
		T angleSin = sin(angleRadians);

		Matrix<T, 4> rotationMatrix;
		rotationMatrix[0][0] = angleCos;
		rotationMatrix[0][1] = angleSin;
		rotationMatrix[1][0] = -angleSin;
		rotationMatrix[1][1] = angleCos;
		return rotationMatrix;
	}

	FORCEINLINE void RotateRoll(T angleRadians)
	{
		static_assert(Size == 4, "Matrix must be 4x4 to receive transformations.");

		*this *= MakeRollRotationMatrix(angleRadians);
	}

	static Matrix<T, 4> MakeRotationMatrixSlow(T yawRadians, T pitchRadians, T rollRadians)
	{
		return MakeYawRotationMatrix(yawRadians) * MakePitchRotationMatrix(pitchRadians) *
			MakeRollRotationMatrix(rollRadians);
	}

	FORCEINLINE void RotateSlow(T yawRadians, T pitchRadians, T rollRadians)
	{
		static_assert(Size == 4, "Matrix must be 4x4 to receive transformations.");

		*this = MakeRotationMatrixSlow(yawRadians, pitchRadians, rollRadians) * (*this);
	}

	// This is a faster version of the MakeRotationMatrix function, as it "unrolls" the creation and multiplication of
	// the individual rotation matricies (yaw, pitch, roll) into a single matrix.
	NODISCARD static Matrix<T, 4> MakeRotationMatrix(T yawRadians, T pitchRadians, T rollRadians)
	{
		Matrix<T, 4> rotationMatrix;

		T yawCos   = cos(yawRadians);
		T yawSin   = sin(yawRadians);
		T pitchCos = cos(pitchRadians);
		T pitchSin = sin(pitchRadians);
		T rollCos  = cos(rollRadians);
		T rollSin  = sin(rollRadians);

		rotationMatrix[0][0] = yawCos * rollCos + yawSin * pitchSin * rollSin;
		rotationMatrix[0][1] = rollSin * pitchCos;
		rotationMatrix[0][2] = -yawSin * rollCos + yawCos * pitchSin * rollSin;
		rotationMatrix[0][3] = static_cast<T>(0);
		rotationMatrix[1][0] = -yawCos * rollSin + yawSin * pitchSin * rollCos;
		rotationMatrix[1][1] = rollCos * pitchCos;
		rotationMatrix[1][2] = rollSin * yawSin + yawCos * pitchSin * rollCos;
		rotationMatrix[1][3] = static_cast<T>(0);
		rotationMatrix[2][0] = yawSin * pitchCos;
		rotationMatrix[2][1] = -pitchSin;
		rotationMatrix[2][2] = yawCos * pitchCos;
		rotationMatrix[2][3] = static_cast<T>(0);
		rotationMatrix[3][0] = static_cast<T>(0);
		rotationMatrix[3][1] = static_cast<T>(0);
		rotationMatrix[3][2] = static_cast<T>(0);
		rotationMatrix[3][3] = static_cast<T>(1);

		return rotationMatrix;
	}

	FORCEINLINE void Rotate(T yawRadians, T pitchRadians, T rollRadians)
	{
		static_assert(Size == 4, "Matrix must be 4x4 to receive transformations.");

		*this = MakeRotationMatrix(yawRadians, pitchRadians, rollRadians) * (*this);
	}

	FORCEINLINE void Rotate(const Rotator<T> &rotator)
	{
		static_assert(Size == 4, "Matrix must be 4x4 to receive transformations.");

		Rotate(rotator.GetYawRadians(), rotator.GetPitchRadians(), rotator.GetRollRadians());
	}

	void Scale(Vector3<T> scale)
	{
		static_assert(Size == 4, "Matrix must be 4x4 to receive transformations.");

		Matrix<T, 4> scaleMatrix;
		scaleMatrix[0][0] = scale.X;
		scaleMatrix[1][1] = scale.Y;
		scaleMatrix[2][2] = scale.Z;

		*this = scaleMatrix * (*this);
	}

private:
	T m_Data[Size][Size];
};

using Matrix2x2  = Matrix<f32, 2>;
using Matrix2x2I = Matrix<s32, 2>;
using Matrix2x2F = Matrix<f32, 2>;
using Matrix2x2D = Matrix<f64, 2>;

using Matrix3x3  = Matrix<f32, 3>;
using Matrix3x3I = Matrix<s32, 3>;
using Matrix3x3F = Matrix<f32, 3>;
using Matrix3x3D = Matrix<f64, 3>;

using Matrix4x4  = Matrix<f32, 4>;
using Matrix4x4I = Matrix<s32, 4>;
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
//         for (s32 i = 0; i < 2; i++)
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
//     FORCEINLINE Vector2<T> &operator[](s32 index)
//     {
//         return m_Data[index];
//     }
//
//     FORCEINLINE const Vector2<T> &operator[](s32 index) const
//     {
//         return m_Data[index];
//     }
//
//     FORCEINLINE void Set(s32 row, s32 column, T value)
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
// typedef Matrix2x2<s32> Matrix2x2i;
// typedef Matrix2x2<f32> Matrix2x2f;
// typedef Matrix2x2<f64> Matrix2x2d;
