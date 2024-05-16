#pragma once
#include <string>
#include "sockpp/connector.h"

namespace NetworkFramework {

inline std::string IPv4ToIPv6(const std::string& ipv4Address) {
    std::stringstream ss(ipv4Address);
    std::string segment;
    std::string ipv6Address = "::ffff:";  // IPv4-mapped IPv6 address prefix
    int i = 0;
    while (i < 2 && std::getline(ss, segment, '.')) {
        int decimal = std::stoi(segment);
        std::stringstream hexStream;
        hexStream << std::hex << std::setw(2) << std::setfill('0') << decimal;
        ipv6Address += hexStream.str();
        i++;
    }
    ipv6Address += ":";
    while (std::getline(ss, segment, '.')) {
        int decimal = std::stoi(segment);
        std::stringstream hexStream;
        hexStream << std::hex << std::setw(2) << std::setfill('0') << decimal;
        ipv6Address += hexStream.str();
    }
    // printf("convert %s to %s\n", ipv4Address.c_str(), ipv6Address.c_str());
    return ipv6Address;
}

inline sockpp::inet6_address ValidateAddress(const std::string& address, int port) {
    try {
        return sockpp::inet6_address(address, port);
    } catch (const std::exception&) {
        try {
            // auto address4 = sockpp::inet_address(address, port);
            // auto address6 = in6_addr();
            // address6.__in6_u.__u6_addr16[0] = 0x0000;
            // address6.__in6_u.__u6_addr16[1] = 0x0000;
            // address6.__in6_u.__u6_addr16[2] = 0x0000;
            // address6.__in6_u.__u6_addr16[4] = 0x0000;
            // address6.__in6_u.__u6_addr16[5] = 0xffff;
            // address6.__in6_u.__u6_addr8[12] = address4[3];
            // address6.__in6_u.__u6_addr8[13] = address4[2];
            // address6.__in6_u.__u6_addr8[14] = address4[1];
            // address6.__in6_u.__u6_addr8[15] = address4[0];
            // return sockpp::inet6_address(address6, port);
            return sockpp::inet6_address(IPv4ToIPv6(address), port);
        } catch (const std::exception&) {
            throw InvalidAddressOrPortException(address, port);
        }
    }
}

}  // namespace NetworkFramework
