//------------------------------------------------------------------------------
// This is InfinityLearn's error handling header file.
// It defines some macros and functions for error handling and checks in the
// InfinityLearn project.
//------------------------------------------------------------------------------

#pragma once

#include <sstream>
#include <string>

namespace InfinityLearn
{

enum class ErrorFailureMode
{
    Abort,
    Throw
};

[[noreturn]] void HandleCheckFailure(const char* expression, const char* file, int line, const char* function,
                                     const std::string& message, ErrorFailureMode mode);

[[noreturn]] void HandleUnreachable(const char* file, int line, const char* function, const std::string& message);

// -----------------------------------------------------------------------------
// Error logging suppression
// -----------------------------------------------------------------------------
//
// These utilities suppress the printing/logging side of error failures, but they
// do not suppress the actual failure behavior. Checks still throw or abort based
// on their selected ErrorFailureMode.

void PushErrorLoggingSuppression();
void PopErrorLoggingSuppression();
bool IsErrorLoggingSuppressed();

class ScopedErrorLoggingSuppression
{
   public:
    ScopedErrorLoggingSuppression()
    {
        PushErrorLoggingSuppression();
    }

    ~ScopedErrorLoggingSuppression()
    {
        PopErrorLoggingSuppression();
    }

    ScopedErrorLoggingSuppression(const ScopedErrorLoggingSuppression&) = delete;
    ScopedErrorLoggingSuppression& operator=(const ScopedErrorLoggingSuppression&) = delete;

    ScopedErrorLoggingSuppression(ScopedErrorLoggingSuppression&&) = delete;
    ScopedErrorLoggingSuppression& operator=(ScopedErrorLoggingSuppression&&) = delete;
};

} // namespace InfinityLearn

// -----------------------------------------------------------------------------
// Default compile definitions
// -----------------------------------------------------------------------------
//
// These are normally supplied by CMake through InfinityLearnConfig.
// These fallbacks make the header robust if included outside normal CMake
// setup.

#ifndef IL_BUILD_TYPE
#define IL_BUILD_TYPE "Unknown"
#endif

#ifndef IL_ENABLE_ASSERTS
#define IL_ENABLE_ASSERTS 1
#endif

#ifndef IL_ENABLE_DEBUG_ASSERTS
#define IL_ENABLE_DEBUG_ASSERTS 0
#endif

// -----------------------------------------------------------------------------
// Internal macro helpers
// -----------------------------------------------------------------------------

#define IL_MAKE_ERROR_MESSAGE(message_expr)  \
    (                                        \
        [&]() -> std::string                 \
        {                                    \
            std::ostringstream il_error_oss; \
            il_error_oss << message_expr;    \
            return il_error_oss.str();       \
        }())

#define IL_HANDLE_CHECK_FAILURE(condition, message_expr, failure_mode)                          \
    do                                                                                          \
    {                                                                                           \
        ::InfinityLearn::HandleCheckFailure(#condition, __FILE__, __LINE__, __func__,           \
                                            IL_MAKE_ERROR_MESSAGE(message_expr), failure_mode); \
    } while (false)

// -----------------------------------------------------------------------------
// Public macros
// -----------------------------------------------------------------------------

#if IL_ENABLE_ASSERTS

#define IL_ASSERT(condition, message_expr)                                                              \
    do                                                                                                  \
    {                                                                                                   \
        if (!(condition))                                                                               \
        {                                                                                               \
            IL_HANDLE_CHECK_FAILURE(condition, message_expr, ::InfinityLearn::ErrorFailureMode::Abort); \
        }                                                                                               \
    } while (false)

#else

#define IL_ASSERT(condition, message_expr) \
    do                                     \
    {                                      \
        (void)sizeof(condition);           \
    } while (false)

#endif

#if IL_ENABLE_DEBUG_ASSERTS

#define IL_ASSERT_DEBUG(condition, message_expr)                                                        \
    do                                                                                                  \
    {                                                                                                   \
        if (!(condition))                                                                               \
        {                                                                                               \
            IL_HANDLE_CHECK_FAILURE(condition, message_expr, ::InfinityLearn::ErrorFailureMode::Abort); \
        }                                                                                               \
    } while (false)

#else

#define IL_ASSERT_DEBUG(condition, message_expr) \
    do                                           \
    {                                            \
        (void)sizeof(condition);                 \
    } while (false)

#endif

#define IL_CHECK(condition, message_expr)                                                               \
    do                                                                                                  \
    {                                                                                                   \
        if (!(condition))                                                                               \
        {                                                                                               \
            IL_HANDLE_CHECK_FAILURE(condition, message_expr, ::InfinityLearn::ErrorFailureMode::Throw); \
        }                                                                                               \
    } while (false)

#define IL_VERIFY(condition, message_expr)                                                              \
    do                                                                                                  \
    {                                                                                                   \
        const bool il_verify_result = static_cast<bool>(condition);                                     \
        if (!il_verify_result)                                                                          \
        {                                                                                               \
            IL_HANDLE_CHECK_FAILURE(condition, message_expr, ::InfinityLearn::ErrorFailureMode::Abort); \
        }                                                                                               \
    } while (false)

#define IL_REQUIRE(condition, message_expr) IL_ASSERT(condition, "Precondition failed: " << message_expr)

#define IL_ENSURE(condition, message_expr) IL_ASSERT(condition, "Postcondition failed: " << message_expr)

#define IL_UNREACHABLE(message_expr)                                                                           \
    do                                                                                                         \
    {                                                                                                          \
        ::InfinityLearn::HandleUnreachable(__FILE__, __LINE__, __func__, IL_MAKE_ERROR_MESSAGE(message_expr)); \
    } while (false)

#define IL_NOT_IMPLEMENTED(message_expr) IL_UNREACHABLE("Not implemented: " << message_expr)

// -----------------------------------------------------------------------------
// Error logging suppression macros
// -----------------------------------------------------------------------------

#define IL_CONCAT_IMPL(a, b) a##b
#define IL_CONCAT(a, b) IL_CONCAT_IMPL(a, b)

#define IL_SUPPRESS_ERROR_LOGGING_SCOPE() \
    const ::InfinityLearn::ScopedErrorLoggingSuppression IL_CONCAT(il_error_logging_suppression_, __LINE__)

#define IL_SUPPRESS_ERROR_LOGGING_FOR_THIS_FILE()                                    \
    namespace                                                                        \
    {                                                                                \
    [[maybe_unused]] const ::InfinityLearn::ScopedErrorLoggingSuppression IL_CONCAT( \
        il_error_logging_suppression_for_file_, __LINE__);                           \
    }
