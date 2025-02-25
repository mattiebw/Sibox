#pragma once

// MW @gotcha: This file should only be included in the PCH, or else the Linux build will fail due to redefinition
//             fmt overloads at the end of the file.

#include "UUID.h"
#include "spdlog/spdlog.h"
#include "spdlog/sinks/stdout_color_sinks.h"
#include "spdlog/fmt/ostr.h" // Needed for logging some types

extern std::shared_ptr<spdlog::logger> g_SiboxLogger;

void InitLog(const char *prefPath);
void AddSinkToLog(const spdlog::sink_ptr &sink);

#ifndef SIBOX_NO_LOG
	#define SIBOX_TRACE(format, ...)                  g_SiboxLogger->trace(format "\n" __VA_OPT__(,) __VA_ARGS__)
	#define SIBOX_INFO(format, ...)                   g_SiboxLogger->info(format "\n" __VA_OPT__(,) __VA_ARGS__)
	#define SIBOX_INFO(format, ...)                   g_SiboxLogger->info(format "\n" __VA_OPT__(,) __VA_ARGS__)
	#define SIBOX_WARN(format, ...)                   g_SiboxLogger->warn(format "\n" __VA_OPT__(,) __VA_ARGS__)
	#define SIBOX_ERROR(format, ...)                  g_SiboxLogger->error(format "\n" __VA_OPT__(,) __VA_ARGS__)
	#define SIBOX_CRITICAL(format, ...)                g_SiboxLogger->critical(format "\n" __VA_OPT__(,) __VA_ARGS__)

	#define SIBOX_TRACE_NO_NEWLINE(format, ...)        g_SiboxLogger->trace(format __VA_OPT__(,) __VA_ARGS__)
	#define SIBOX_INFO_NO_NEWLINE(format, ...)        g_SiboxLogger->info(format __VA_OPT__(,) __VA_ARGS__)
	#define SIBOX_WARN_NO_NEWLINE(format, ...)        g_SiboxLogger->warn(format __VA_OPT__(,) __VA_ARGS__)
	#define SIBOX_ERROR_NO_NEWLINE(format, ...)       g_SiboxLogger->error(format __VA_OPT__(,) __VA_ARGS__)
	#define SIBOX_CRITICAL_NO_NEWLINE(format, ...)     g_SiboxLogger->critical(format __VA_OPT__(,) __VA_ARGS__)
#else
	#define SIBOX_TRACE(...)      
	#define SIBOX_INFO(...)       
	#define SIBOX_WARN(...)       
	#define SIBOX_ERROR(...)      
	#define SIBOX_CRITICAL(...)

	#define SIBOX_TRACE_NO_NEWLINE(format, ...)     
	#define SIBOX_INFO_NO_NEWLINE(format, ...)      
	#define SIBOX_WARN_NO_NEWLINE(format, ...)      
	#define SIBOX_ERROR_NO_NEWLINE(format, ...)     
	#define SIBOX_CRITICAL_NO_NEWLINE(format, ...)  
#endif

template <>
class fmt::formatter<glm::ivec2>
{
public:
	constexpr auto parse(format_parse_context &ctx) { return ctx.begin(); }

	template <typename Context>
	constexpr auto format(const glm::ivec2 &vec, Context &ctx) const
	{
		return fmt::format_to(ctx.out(), "({}, {})", vec.x, vec.y);
	}
};

template <>
class fmt::formatter<glm::ivec3>
{
public:
	constexpr auto parse(format_parse_context &ctx) { return ctx.begin(); }

	template <typename Context>
	constexpr auto format(const glm::ivec3 &vec, Context &ctx) const
	{
		return fmt::format_to(ctx.out(), "({}, {}, {})", vec.x, vec.y, vec.z);
	}
};

template <>
class fmt::formatter<glm::vec2>
{
public:
	constexpr auto parse(format_parse_context &ctx) { return ctx.begin(); }

	template <typename Context>
	constexpr auto format(const glm::vec2 &vec, Context &ctx) const
	{
		return fmt::format_to(ctx.out(), "({}, {})", vec.x, vec.y);
	}
};

template <>
class fmt::formatter<glm::vec3>
{
public:
	constexpr auto parse(format_parse_context &ctx) { return ctx.begin(); }

	template <typename Context>
	constexpr auto format(const glm::vec3 &vec, Context &ctx) const
	{
		return fmt::format_to(ctx.out(), "({}, {}, {})", vec.x, vec.y, vec.z);
	}
};

template <>
class fmt::formatter<glm::vec4>
{
public:
	constexpr auto parse(format_parse_context &ctx) { return ctx.begin(); }

	template <typename Context>
	constexpr auto format(const glm::vec4 &vec, Context &ctx) const
	{
		return fmt::format_to(ctx.out(), "({}, {}, {}, {})", vec.x, vec.y, vec.z, vec.w);
	}
};

template <>
class fmt::formatter<UUID>
{
public:
	constexpr auto parse(format_parse_context &ctx) { return ctx.begin(); }

	template <typename Context>
	constexpr auto format(const UUID &uuid, Context &ctx) const
	{
		return fmt::format_to(ctx.out(), "({})", static_cast<uint32_t>(uuid));
	}
};
