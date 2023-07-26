#pragma once
#include <cassert>
#include <cstdint>

namespace UN::DI
{
    //! \brief Represents an error result of a function call within the library.
    enum class ErrorCode : uint32_t
    {
        Fail,               //!< Operation failed.
        Abort,              //!< Operation aborted.
        NotImplemented,     //!< Operation not implemented.
        InvalidOperation,   //!< Operation was invalid.
        InvalidArguments,   //!< One or more arguments were invalid.
        NotFound,           //!< One or more arguments were not found.
        CircularDependency, //!< Service activation failed because of circular dependencies.
        AccessDenied,       //!< General access denied error occurred.
        Timeout,            //!< Operation timed out.
        OutOfMemory         //!< Not enough memory to complete the operation.
    };

    //! \brief Get a constant string that represents an ErrorCode enum value.
    inline const char* ErrorCodeToString(ErrorCode result)
    {
        switch (result)
        {
            // clang-format off
        case ErrorCode::Fail: return "ErrorCode::Fail";
        case ErrorCode::Abort: return "ErrorCode::Abort";
        case ErrorCode::NotImplemented: return "ErrorCode::NotImplemented";
        case ErrorCode::InvalidOperation: return "ErrorCode::InvalidOperation";
        case ErrorCode::InvalidArguments: return "ErrorCode::InvalidArguments";
        case ErrorCode::NotFound: return "ErrorCode::NotFound";
        case ErrorCode::CircularDependency: return "ErrorCode::CircularDependency";
        case ErrorCode::AccessDenied: return "ErrorCode::AccessDenied";
        case ErrorCode::Timeout: return "ErrorCode::Timeout";
        case ErrorCode::OutOfMemory: return "ErrorCode::OutOfMemory";
            // clang-format on
        default:
            assert(false && "ErrorCode was unknown");
            return "ErrorCode::<Unknown>";
        }
    }
} // namespace UN::DI
