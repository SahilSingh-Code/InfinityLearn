//-----------------------------------------------------------------------------
// Logging.h
//
// This is Infinity Learn's logging system. It provides a simple interface for
// logging messages to the console and to files.
//
// Which level of messages are printed depends on the build type, which can be
// set at compile time by using the flag -DCMAKE_BUILD_TYPE=<BuildType>
//
// Here are the default log levels for each build type:
// - DEBUG: DEBUG, INFO, WARNING, ERROR
// - DEVELOP: INFO, WARNING, ERROR
// - RELEASE: INFO, WARNING, ERROR
// - PERFORMANCE: PERFORMANCE, WARNING, ERROR
//-----------------------------------------------------------------------------

#pragma once

// Default the active log level to INFO if it is not defined
#ifndef IFLEARN_ACTIVE_LOG_LEVEL
#define IFLEARN_ACTIVE_LOG_LEVEL INFO
#endif

// Includes
#include <string>  //  For std::string


namespace InfinityLearn
{

// The log level enumeration defining the log severity
enum LogLevel
{
    PERFORMANCE,
    DEBUG,
    INFO,
    WARNING,
    ERROR
};

/// <summary>
/// Logs a message with the specified logging level.
/// </summary>
/// <param name="level">The severity level of the log message.</param>
/// <param name="message">The message to log.</param>
void Log(LogLevel level, const std::string& message);

/// <summary>
/// Writes a message to a specified file.
/// </summary>
/// <param name="message">The message to write to the file.</param>
/// <param name="filename">The name of the file to write to.</param>
void LogToFile(const std::string& message, const std::string& filename);

}  // namespace InfinityLearn
