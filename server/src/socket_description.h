#pragma once
#include <iostream>
#include "Common.h"

namespace Server{
    class socket_description {
    public:
        socket_description(){}
        socket_description(int aSocket, std::string aIp, int aId)
        : socket	        (aSocket)
        , ip		        (aIp)
        , id		        (aId)
        , isAuthenticated   (false)
        {
        }
        
        ~socket_description() {
        };

        friend inline std::ostream& operator<<(std::ostream& os, const socket_description& sd);
        friend inline bool operator==(const socket_description& aLhs, const socket_description& aRhs);

        int         socket;
        std::string ip;
        int         id; 
        bool        isAuthenticated;
    };
    inline std::ostream& operator<<(std::ostream& os, const socket_description& sd)
    {
        os << "[ id:"		<< sd.id
            << " ip:"		<< sd.ip 
            << " socket:"	<< sd.socket
            << " ]";
        return os;
    }

    inline bool operator==(const socket_description& aLhs, const socket_description& aRhs) {
        return aLhs.id == aRhs.id;
    }
}
