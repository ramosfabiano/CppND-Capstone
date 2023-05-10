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
    explicit BaseException(std::string errMessage) : _errMessage(errMessage) {}
    virtual ~BaseException() throw () {}

    virtual char const* what() const  throw ()
    {
        return _errMessage.c_str();
    }

protected:
    std::string _errMessage;
};


#define IMPLEMENT_CUSTOM_EXCEPTION(name, prefix)                                                  \
    class name : public common::BaseException                                                     \
    {                                                                                             \
    public:                                                                                       \
        name(const std::string& errMessage) :                                                     \
            common::BaseException(std::string(prefix) + std::string(": ") + errMessage) {}        \
    };                                                                                            \


} // namespace http_server
