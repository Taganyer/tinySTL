//
// Created by 86152 on 2023/8/19.
//

#ifndef TINYSTL_WARNING_HPP
#define TINYSTL_WARNING_HPP

#include <iostream>

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

    class runtimeError : public Exception {
    public:
        runtimeError() : Exception("runtimeError\n") {};

        using Exception::Exception;

        ~runtimeError() override = default;
    };

    class logicError : public Exception {
    public:
        logicError() : Exception("logicError\n") {};

        using Exception::Exception;

        ~logicError() override = default;
    };

    class badAlloc : public Exception {
    public:
        badAlloc() : Exception("badAlloc\n") {};

        using Exception::Exception;

        ~badAlloc() override = default;
    };

    class badCast : public Exception {
    public:
        badCast() : Exception("badCast\n") {};

        using Exception::Exception;

        ~badCast() override = default;
    };

    class outOfRange : public logicError {
    public:
        outOfRange() : logicError("outOfRange\n") {};

        using logicError::logicError;

        ~outOfRange() override = default;
    };
}

#endif //TINYSTL_WARNING_HPP
