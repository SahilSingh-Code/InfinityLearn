//-------------------------------------------------------------------------------
// Logging.cpp
//
// This is the implementation of InfinityLearn's logging system.
// ------------------------------------------------------------------------------

// Includes
#include "Logging.h"
#include <fstream>   // For std::ofstream
#include <iostream>  // For std::cout and std::cerr

namespace InfinityLearn
{


void Log(LogLevel level, const std::string& message)
{
    // Performance build shows only PERFORMANCE, WARNING, ERROR
    if (IFLEARN_ACTIVE_LOG_LEVEL == PERFORMANCE)
    {
        if (!(level == PERFORMANCE || level == WARNING || level == ERROR))
        {
            return;
        }
    }
    else
    {
        // Normal threshold filtering
        if (level < IFLEARN_ACTIVE_LOG_LEVEL)
        {
            return;
        }
    }

    std::string logLevel = "";

    switch (level)
    {
        case InfinityLearn::PERFORMANCE:
            logLevel = "PERFORMANCE";
            break;
        case InfinityLearn::DEBUG:
            logLevel = "DEBUG";
            break;
        case InfinityLearn::INFO:
            logLevel = "INFO";
            break;
        case InfinityLearn::WARNING:
            logLevel = "WARNING";
            break;
        case InfinityLearn::ERROR:
            logLevel = "ERROR";
            break;
        default:
            return;
    }

    std::cout << logLevel << ": " << message << "\n";
}


void LogToFile(const std::string& message, const std::string& filename)
{
    // Open the file in append mode
    std::ofstream file(filename, std::ios::app);
    if (file.is_open())
    {
        file << message << std::endl;  // Write the message to the file
        file.close();                  // Close the file
    }
    else
    {
        std::cerr << "Error: Could not open file " << filename
                  << " for logging." << std::endl;
    }
}

}  // namespace InfinityLearn
