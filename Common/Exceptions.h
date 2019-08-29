#pragma once
// TODO: delete this? Client have same excption file
#include <exception>
#include <iostream>

namespace Common {
    class SocketException: public std::exception
    {
    public:
        std::string& msg;
        explicit SocketException(std::string&& m) 
            : msg(m)
        {

        }
        virtual const char* what() const throw()
        {
            return msg.c_str();
        }
    };
    class CommunicationError: public std::exception
    {
    public:
        std::string& msg;
        explicit CommunicationError(std::string&& m) 
            : msg(m)
        {

        }
        virtual const char* what() const throw()
        {
            return msg.c_str();
        }
    };
    class AuthenticationFailedException: public std::exception
    {
    public:
        explicit AuthenticationFailedException() { }
        virtual const char* what() const throw()
        {
            return "Authentication failed";
        }
    };
    
}
