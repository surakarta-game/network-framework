#pragma once
#include <string>
#include "sockpp/connector.h"

namespace NetworkFramework {

inline sockpp::inet6_address ValidateAddress(const std::string& address, int port) {
    try {
        return sockpp::inet6_address(address, port);
    } catch (const std::exception&) {
        try {
            auto address4 = sockpp::inet_address(address, port);
            auto address6 = in6_addr();
            address6.__in6_u.__u6_addr16[0] = 0x0000;
            address6.__in6_u.__u6_addr16[1] = 0x0000;
            address6.__in6_u.__u6_addr16[2] = 0x0000;
            address6.__in6_u.__u6_addr16[4] = 0x0000;
            address6.__in6_u.__u6_addr16[5] = 0xffff;
            address6.__in6_u.__u6_addr8[12] = address4[3];
            address6.__in6_u.__u6_addr8[13] = address4[2];
            address6.__in6_u.__u6_addr8[14] = address4[1];
            address6.__in6_u.__u6_addr8[15] = address4[0];
            return sockpp::inet6_address(address6, port);
        } catch (const std::exception&) {
            throw InvalidAddressOrPortException(address, port);
        }
    }
}

}  // namespace NetworkFramework
