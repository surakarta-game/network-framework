/*
 *  Description: This file implements a function, ValidateAddress(),
 *               which converts an address string and a port into
 *               a Sockpp address.
 *
 *  Author(s):
 *      Nictheboy Li    <nictheboy@outlook.com>
 *
 *  License:
 *      MIT License, feel free to use and modify this file!
 *
 */

#pragma once
#include <iostream>
#include <sstream>
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
    auto ipv6address = address.find('.') == std::string::npos ? address : IPv4ToIPv6(address);
    auto result = sockpp::inet6_address::create(ipv6address, port);
    if (result.is_ok()) {
        return result.value();
    } else {
        throw InvalidAddressOrPortException(address, port);
    }
}

}  // namespace NetworkFramework
