#pragma once
#include <iostream>

namespace Server{
    class socket_description {
    public:
        socket_description(){}
        socket_description(int aSocket, std::string aIp, int aId)
        : socket	(aSocket)
        , ip		(aIp)
        , id		(aId)
        {
            std::cout << id << std::endl;
        }
        ~socket_description() = default;

        friend inline std::ostream& operator<<(std::ostream& os, const socket_description& sd);
        friend inline bool operator==(const socket_description& aLhs, const socket_description& aRhs);

        int socket;
        std::string ip;
        int id; 
        std::string message;
    };
    inline std::ostream& operator<<(std::ostream& os, const socket_description& sd)
    {
        os << "[ id:"		<< sd.id
            << " ip:"		<< sd.ip 
            << " socket:"	<< sd.socket
            << " message:"	<< sd.message 
            << " ]";
        return os;
    }

    inline bool operator==(const socket_description& aLhs, const socket_description& aRhs) {
        return aLhs.id == aRhs.id;
    }
}
