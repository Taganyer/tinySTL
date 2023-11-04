//
// Created by 86152 on 2023/8/19.
//

#ifndef TINYSTL_WARNING_HPP
#define TINYSTL_WARNING_HPP

#include <exception>

namespace STD {
    class Exception : public std::exception {
    private:
        const char *message = nullptr;

    public:
        Exception() : message("Exception\n") {};

        explicit Exception(const char *note) : message(note) {};

        const char *what() const noexcept override { return message; };

        ~Exception() override = default;
    };

    struct runtimeError : public Exception {
        runtimeError() : Exception("runtimeError\n") {};

        using Exception::Exception;

        ~runtimeError() override = default;
    };

    struct logicError : public Exception {
        logicError() : Exception("logicError\n") {};

        using Exception::Exception;

        ~logicError() override = default;
    };

    struct badAlloc : public Exception {
        badAlloc() : Exception("badAlloc\n") {};

        using Exception::Exception;

        ~badAlloc() override = default;
    };

    struct badCast : public Exception {
        badCast() : Exception("badCast\n") {};

        using Exception::Exception;

        ~badCast() override = default;
    };

    struct outOfRange : public logicError {
        outOfRange() : logicError("outOfRange\n") {};

        using logicError::logicError;

        ~outOfRange() override = default;
    };

    struct errorType : public runtimeError {
        errorType() : runtimeError("errorType\n") {};

        using runtimeError::runtimeError;

        ~errorType() override = default;
    };

    struct overFLow : public runtimeError {
        overFLow() : runtimeError("overflow_error\n") {};

        using runtimeError::runtimeError;

        ~overFLow() override = default;
    };

    struct errorArgument : public runtimeError {
        errorArgument() : runtimeError("errorArgument\n") {};

        using runtimeError::runtimeError;

        ~errorArgument() override = default;
    };

}

#endif //TINYSTL_WARNING_HPP
