#pragma once

#include <string>

namespace common
{

/*
    BaseException Class.

    All custom exceptions should inherit from this class.
*/
class BaseException : public std::exception
{
public:
    explicit BaseException(std::string errMessage) : _errMessage(errMessage) {}
    virtual ~BaseException() throw () {}

    virtual char const* what() const  throw ()
    {
        return _errMessage.c_str();
    }

protected:
    std::string _errMessage;
};


/*
    Macro to quickly implement a custom exception class (prefixed with a custom string).
*/
#define IMPLEMENT_CUSTOM_EXCEPTION(name, prefix)                                                  \
    class name : public common::BaseException                                                     \
    {                                                                                             \
    public:                                                                                       \
        name(const std::string& errMessage) :                                                     \
            common::BaseException(std::string(prefix) + std::string(": ") + errMessage) {}        \
    };                                                                                            \

} // namespace http_server
