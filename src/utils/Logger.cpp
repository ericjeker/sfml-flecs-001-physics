// Copyright (c) Eric Jeker 2025.

#include "core/utils/Logger.h"

#include <iostream>
#include <ostream>

namespace
{

std::string SeverityToText(const Logger::LogLevel level)
{
    switch (level)
    {
        case Logger::LogLevel::Trace:
            return "trace";
        case Logger::LogLevel::Debug:
            return "debug";
        case Logger::LogLevel::Info:
            return "info";
        case Logger::LogLevel::Warning:
            return "warn";
        case Logger::LogLevel::Error:
            return "error";
        case Logger::LogLevel::Fatal:
            return "fatal";
    }

    return "UNKNOWN";
}

void Log(const Logger::LogLevel level, const std::string& message)
{
    // Use std::cerr when the log level is below or equal Error
    auto& oStream = (level <= Logger::LogLevel::Error) ? std::cerr : std::cout;
    oStream << " (" << SeverityToText(level) << ") " << message << std::endl;
}

} // namespace

void Logger::Fatal(const std::string& message)
{
    Log(LogLevel::Fatal, message);
}

void Logger::Error(const std::string& message)
{
    Log(LogLevel::Error, message);
}

void Logger::Warn(const std::string& message)
{
    Log(LogLevel::Warning, message);
}

void Logger::Info(const std::string& message)
{
    Log(LogLevel::Info, message);
}

void Logger::Debug(const std::string& message)
{
    Log(LogLevel::Debug, message);
}

void Logger::Trace(const std::string& message)
{
    Log(LogLevel::Trace, message);
}
