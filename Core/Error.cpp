#include "Error.h"
#include <cstdlib>
#include <iostream>
#include <sstream>
#include <stdexcept>

#if defined(_MSC_VER)
#include <intrin.h>
#endif

namespace InfinityLearn
{

namespace
{

std::string BuildCheckFailureMessage(const char* expression, const char* file,
                                     int line, const char* function,
                                     const std::string& message)
{
    std::ostringstream oss;

    oss << "InfinityLearn error check failed\n";
    oss << "  Expression: " << expression << "\n";
    oss << "  Message:    " << message << "\n";
    oss << "  Function:   " << function << "\n";
    oss << "  Location:   " << file << ":" << line << "\n";
    oss << "  Build:      " << IL_BUILD_TYPE << "\n";

    return oss.str();
}

[[noreturn]] void AbortProgram()
{
#if defined(_MSC_VER)
    __debugbreak();
#endif

    std::abort();
}

}  // namespace

[[noreturn]] void HandleCheckFailure(const char* expression, const char* file,
                                     int line, const char* function,
                                     const std::string& message,
                                     ErrorFailureMode mode)
{
    const std::string fullMessage =
        BuildCheckFailureMessage(expression, file, line, function, message);

    std::cerr << fullMessage << std::endl;

    if (mode == ErrorFailureMode::Throw)
    {
        throw std::runtime_error(fullMessage);
    }

    AbortProgram();
}

[[noreturn]] void HandleUnreachable(const char* file, int line,
                                    const char* function,
                                    const std::string& message)
{
    HandleCheckFailure("IL_UNREACHABLE", file, line, function, message,
                       ErrorFailureMode::Abort);
}

}  // namespace InfinityLearn
