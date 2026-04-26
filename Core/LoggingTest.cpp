//-------------------------------------------------------------------------------
// LoggingTest.cpp
//
// This is a test program for InfinityLearn's logging system.
// This is not a thorough test, but it demonstrates the basic functionality of
// the logging system. A sanity check to ensure that the logging system is
// working as expected with different build configurations.
//-------------------------------------------------------------------------------

// Includes
#include "Logging.h"  // For Log and LogLevel

// Use the InfinityLearn namespace to avoid having to prefix Log and LogLevel
using namespace InfinityLearn;

// Print one of each type of log message to the console to verify that the
// logging system is working
int main()
{
    Log(PERFORMANCE, "This is a PERFORMANCE log");
    Log(DEBUG, "This is a DEBUG log");
    Log(INFO, "This is an INFO log");
    Log(WARNING, "This is a WARNING log");
    Log(ERROR, "This is an ERROR log");

    return 0;
}
