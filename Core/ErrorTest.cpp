//-------------------------------------------------------------------------------
// ErrorTest.cpp
//
// This is a test program for InfinityLearn's error handling system.
// This is not a thorough test, but it demonstrates the basic functionality of
// the error handling system. A sanity check to ensure that the error handling
// system is working as expected with different build configurations.
//-------------------------------------------------------------------------------

// Includes
#include "Error.h"  // For HandleCheckFailure and HandleUnreachable

#include <exception>
#include <iostream>

// Macros from Error.h are globally visible; no namespace import needed.

namespace
{

void PrintResult(const char* testName, bool success)
{
    std::cout << testName << ": "
              << (success ? "works as expected" : "unexpected behavior")
              << std::endl;
}

}  // namespace

// Print a few sanity-check results to the console to verify that the
// error handling system is working
int main()
{
    std::cout << "Running error handling sanity checks..." << std::endl;

    IL_ASSERT(true, "IL_ASSERT should not fail for a true condition");
    PrintResult("IL_ASSERT(true)", true);

    IL_ASSERT_DEBUG(true,
                    "IL_ASSERT_DEBUG should not fail for a true condition");
    PrintResult("IL_ASSERT_DEBUG(true)", true);

    IL_VERIFY(true, "IL_VERIFY should not fail for a true condition");
    PrintResult("IL_VERIFY(true)", true);

    IL_REQUIRE(true, "IL_REQUIRE should not fail for a true condition");
    PrintResult("IL_REQUIRE(true)", true);

    IL_ENSURE(true, "IL_ENSURE should not fail for a true condition");
    PrintResult("IL_ENSURE(true)", true);

    IL_CHECK(true, "IL_CHECK should not fail for a true condition");
    PrintResult("IL_CHECK(true)", true);

    bool checkFailureThrows = false;
    try
    {
        IL_CHECK(false, "Intentional IL_CHECK failure for sanity test");
    }
    catch (const std::exception& exception)
    {
        checkFailureThrows = true;
        std::cout << "IL_CHECK(false): works as expected" << std::endl;
        std::cout << "Caught expected exception:" << std::endl;
        std::cout << exception.what() << std::endl;
    }

    if (!checkFailureThrows)
    {
        PrintResult("IL_CHECK(false)", false);
    }

    std::cout << "Skipping IL_ASSERT(false), IL_ASSERT_DEBUG(false), "
              << "IL_VERIFY(false), IL_UNREACHABLE(...), and "
              << "IL_NOT_IMPLEMENTED(...) because they intentionally abort "
              << "the program." << std::endl;

    return 0;
}
