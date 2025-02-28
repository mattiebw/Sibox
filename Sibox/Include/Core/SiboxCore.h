#pragma once

// Preprocessor Definitions
#define PROP(x) x // Usage PROP()
#define FUNC(x) x // Usage: FUNC(glm::vec2 GetPositionOfEntity(UUID entityID));
// Usage: RPC(Server, SendMovement, glm::vec2 position, glm::vec2 velocity);
#define RPC(type, name, ...) void name(__VA_ARGS__);\
	void name##_Implementation(__VA_ARGS__)

#define BIT(x) (1 << (x))

// MSVC compiler intrinsics
// MW @todo: Add intrinsics for other compilers (gcc/clang)
#ifdef SIBOX_PLATFORM_WINDOWS
	#define UNALIGNED __unaligned
	#define FORCEINLINE __forceinline
#else
	#define UNALIGNED
	#define FORCEINLINE inline
#endif

#ifdef SIBOX_DEBUG
	#define FORCEINLINE_DEBUGGABLE 
#else // ifdef SIBOX_DEBUG
	#define FORCEINLINE_DEBUGGABLE FORCEINLINE
#endif // elifdef SIBOX_DEBUG

#if __cplusplus >= 201703L or (defined(_MSVC_LANG) and _MSVC_LANG >= 201703L)
	#define NODISCARD [[nodiscard]]
#else
	#define NODISCARD
#endif

// -----------------------------------------------------------------------------------------------
// Array size macro
// From winnt.h:
//
// RtlpNumberOf is a function that takes a reference to an array of N Ts.
//
// typedef T array_of_T[N];
// typedef array_of_T &reference_to_array_of_T;
//
// RtlpNumberOf returns a pointer to an array of N chars.
// We could return a reference instead of a pointer but older compilers do not accept that.
//
// typedef char array_of_char[N];
// typedef array_of_char *pointer_to_array_of_char;
//
// sizeof(array_of_char) == N
// sizeof(*pointer_to_array_of_char) == N
//
// pointer_to_array_of_char RtlpNumberOf(reference_to_array_of_T);
//
// We never even call RtlpNumberOf, we just take the size of dereferencing its return type.
// We do not even implement RtlpNumberOf, we just declare it.
//
// Attempts to pass pointers instead of arrays to this macro result in compile time errors.
// That is the point.
//
extern "C++" // templates cannot be declared to have 'C' linkage
template <typename T, size_t N>
char (* RtlpNumberOf(UNALIGNED T (&)[N]))[N];
#define SIBOX_ARRAY_SIZE(A) (sizeof(*RtlpNumberOf(A)))
// -----------------------------------------------------------------------------------------------

// Basic swap macro
#define SIBOX_SWAP(IntermediateType, a, b) do { IntermediateType t = a; (a) = b; (b) = t; } while (0)

template <typename T>
using Scope = std::unique_ptr<T>;

template <typename T, typename... Args>
constexpr Scope<T> CreateScope(Args &&... args)
{
	return std::make_unique<T>(std::forward<Args>(args)...);
}

template <typename T>
using Ref = std::shared_ptr<T>;

template <typename T, typename... Args>
constexpr Ref<T> CreateRef(Args &&... args)
{
	return std::make_shared<T>(std::forward<Args>(args)...);
}

#define SIBOX_ASSERT SDL_assert

struct SemVer
{
	short Major;
	short Minor;
	short Patch;

	SemVer() : Major(0), Minor(0), Patch(0)
	{
	}

	explicit SemVer(const short major) : Major(major), Minor(0), Patch(0)
	{
	}

	SemVer(const short major, const short minor, const short patch) : Major(major), Minor(minor), Patch(patch)
	{
	}

	// SemVer comparison operators
	bool operator==(const SemVer &other) const
	{
		return Major == other.Major && Minor == other.Minor && Patch == other.Patch;
	}

	bool operator!=(const SemVer &other) const
	{
		return !(*this == other);
	}

	bool operator<(const SemVer &other) const
	{
		if (Major < other.Major)
			return true;
		if (Major > other.Major)
			return false;

		if (Minor < other.Minor)
			return true;
		if (Minor > other.Minor)
			return false;

		return Patch < other.Patch;
	}
};

// Generate CRC lookup table
template <unsigned c, int k = 8>
struct crcf : crcf<((c & 1) ? 0xedb88320 : 0) ^ (c >> 1), k - 1>
{
};

template <unsigned c>
struct crcf<c, 0>
{
	enum { value = c };
};

#define CRCA(x) CRCB(x) CRCB(x + 128)
#define CRCB(x) CRCc(x) CRCc(x +  64)
#define CRCc(x) CRCD(x) CRCD(x +  32)
#define CRCD(x) CRCE(x) CRCE(x +  16)
#define CRCE(x) CRCF(x) CRCF(x +   8)
#define CRCF(x) CRCG(x) CRCG(x +   4)
#define CRCG(x) CRCH(x) CRCH(x +   2)
#define CRCH(x) CRCI(x) CRCI(x +   1)
#define CRCI(x) crcf<x>::value ,

constexpr unsigned crc_table[] = {CRCA(0)}; // Rider doesn't like this, but it compiles fine!

constexpr uint32_t crc32(std::string_view str)
{
	uint32_t crc = 0xffffffff;
	for (auto c : str)
		crc = (crc >> 8) ^ crc_table[(crc ^ c) & 0xff];
	return crc ^ 0xffffffff;
}

constexpr uint16_t crc16(std::string_view str)
{
	uint16_t crc = 0xffff;
	for (auto c : str)
		crc = (crc >> 8) ^ crc_table[(crc ^ c) & 0xff];
	return crc ^ 0xffff;
}

// namespace std
// {
// 	template <>
// 	struct hash<glm::ivec2>
// 	{
// 		std::size_t operator()(const glm::ivec2 &v) const
// 		{
// 			// Combine the two integers (x, y) into a single hash value.
// 			// Use a prime number multiplier for better distribution.
// 			size_t h1 = std::hash<int>{}(v.x);
// 			size_t h2 = std::hash<int>{}(v.y);
// 			return h1 ^ (h2 << 1); // Combine the two hash values
// 		}
// 	};
// }
