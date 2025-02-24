#pragma once

// Non-owning raw buffer class
// Adapted from https://github.com/TheCherno/Hazel/blob/1feb70572fa87fa1c4ba784a2cfeada5b4a500db/Hazel/src/Hazel/Core/Buffer.h

struct Buffer
{
	uint8_t *Data = nullptr;
	size_t   Size = 0;

	Buffer() = default;

	Buffer(uint8_t *data, size_t size)
	{
		this->Data = data;
		this->Size = size;
	}

	Buffer(size_t size)
	{
		Allocate(Size);
	}

	Buffer(const Buffer &other) = default;

	Buffer(Buffer &&other) noexcept
	{
		Data       = other.Data;
		Size       = other.Size;
		other.Data = nullptr;
		other.Size = 0;
	}

	Buffer& operator=(const Buffer &other) = default;

	Buffer& operator=(Buffer &&other) noexcept
	{
		Data       = other.Data;
		Size       = other.Size;
		other.Data = nullptr;
		other.Size = 0;
		return *this;
	};

	~Buffer() = default; // The buffer is non-owning, so we don't want to free on deconstruct.

	FORCEINLINE_DEBUGGABLE void Allocate(size_t size)
	{
		Release();
		Data = new uint8_t[size];
		Size = size;
	}

	FORCEINLINE_DEBUGGABLE void Release()
	{
		delete[] Data; // It's okay if data is nullptr.
		Data = nullptr;
		Size = 0;
	}

	NODISCARD FORCEINLINE_DEBUGGABLE Buffer Copy() const
	{
		Buffer result(Size);
		memcpy(result.Data, Data, Size);
		return result;
	}

	NODISCARD FORCEINLINE_DEBUGGABLE static Buffer Copy(Buffer buffer)
	{
		return buffer.Copy();
	}

	template <typename T>
	T* As()
	{
		return static_cast<T*>(Data);
	}

	operator bool() const
	{
		return Data != nullptr;
	}
};

// And also an owning scoped buffer:

struct ScopedBuffer
{
	ScopedBuffer(const Buffer &buffer)
		: m_Buffer(buffer)
	{
	}

	ScopedBuffer(Buffer &&buffer)
		: m_Buffer(std::move(buffer))
	{
	}

	ScopedBuffer(size_t size)
		: m_Buffer(size)
	{
	}

	~ScopedBuffer()
	{
		m_Buffer.Release();
	}

	NODISCARD FORCEINLINE uint8_t* Data() { return m_Buffer.Data; }
	NODISCARD FORCEINLINE size_t   Size() { return m_Buffer.Size; }

	template <typename T>
	NODISCARD FORCEINLINE T* As()
	{
		return m_Buffer.As<T>();
	}

	operator bool() const { return m_Buffer; }

private:
	Buffer m_Buffer;
};
