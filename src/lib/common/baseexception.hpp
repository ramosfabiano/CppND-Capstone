#pragma once

#include <string>

namespace common
{

/*
    BaseException

    Allows custom prefixed exception.
*/
class BaseException : public std::exception
{
public:
    BaseException(const char* prefix, const char* errMessage): _prefix(prefix), _message(errMessage) {}
    BaseException(const char* prefix, const std::string& errMessage):  _prefix(prefix), _message(errMessage) {}
    virtual ~BaseException() {}
    virtual const char* what ()
    {
        _message = "[" + _prefix + "]: " + _message;
        return _message.c_str();
    }
private:
    std::string _prefix;
    std::string _message;
};


#define IMPLEMENT_CUSTOM_EXCEPTION(name, prefix)                                                  \
    class name : public common::BaseException                                                     \
    {                                                                                             \
    public:                                                                                       \
        name(const char* errMessage) : common::BaseException(prefix, errMessage) {}               \
        name(const std::string& errMessage) : common::BaseException(prefix, errMessage) {}        \
    };                                                                                            \


} // namespace http_server