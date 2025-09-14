// Copyright (c) Eric Jeker. All Rights Reserved.

#pragma once

#include <format>
#include <string>

#ifndef LOG_LEVEL
#if defined(DEBUG) || defined(_DEBUG)
#define LOG_LEVEL 4
#else
#define LOG_LEVEL 3
#endif
#endif

/**
 * Logger is a utility made for easy logging of messages in the game.
 * It can log messages at different levels such as Error, Warning, Info, and Debug.
 * It can be used in any part of the game to log messages.
 */
namespace Logger
{

enum struct LogLevel
{
	Fatal = 0,
	Error = 1,
	Warning = 2,
	Info = 3,
	Debug = 4,
	Trace = 5
};

// Existing non-templated overloads (definitions remain in Logger.cpp)
void Fatal(const std::string& message);
void Error(const std::string& message);
void Warn(const std::string& message);
void Info(const std::string& message);
void Debug(const std::string& message);
void Trace(const std::string& message);

// New templated overloads that call the non-templated versions after formatting
template <typename... Args>
void Fatal(std::format_string<Args...> fmt, Args&&... args)
{
    Fatal(std::format(fmt, std::forward<Args>(args)...));
}

template <typename... Args>
void Error(std::format_string<Args...> fmt, Args&&... args)
{
    Error(std::format(fmt, std::forward<Args>(args)...));
}

template <typename... Args>
void Warn(std::format_string<Args...> fmt, Args&&... args)
{
    Warn(std::format(fmt, std::forward<Args>(args)...));
}

template <typename... Args>
void Info(std::format_string<Args...> fmt, Args&&... args)
{
    Info(std::format(fmt, std::forward<Args>(args)...));
}

template <typename... Args>
void Debug(std::format_string<Args...> fmt, Args&&... args)
{
    Debug(std::format(fmt, std::forward<Args>(args)...));
}

template <typename... Args>
void Trace(std::format_string<Args...> fmt, Args&&... args)
{
    Trace(std::format(fmt, std::forward<Args>(args)...));
}

}; // namespace Logger

#define LOG_FATAL(...) Logger::Fatal(__VA_ARGS__)

#if LOG_LEVEL >= 1
#define LOG_ERROR(...) Logger::Error(__VA_ARGS__)
#else
#define LOG_ERROR(...)
#endif

#if LOG_LEVEL >= 2
#define LOG_WARN(...) Logger::Warn(__VA_ARGS__)
#else
#define LOG_WARN(...)
#endif

#if LOG_LEVEL >= 3
#define LOG_INFO(...) Logger::Info(__VA_ARGS__)
#else
#define LOG_INFO(...)
#endif

#if LOG_LEVEL >= 4
#define LOG_DEBUG(...) Logger::Debug(__VA_ARGS__)
#else
#define LOG_DEBUG(...)
#endif

#if LOG_LEVEL >= 5
#define LOG_TRACE(...) Logger::Trace(__VA_ARGS__)
#else
#define LOG_TRACE(...)
#endif

