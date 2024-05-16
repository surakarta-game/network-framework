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
#include "sockpp/inet_address.h"

namespace NetworkFramework {

inline sockpp::inet_address ValidateAddress(const std::string& address, int port) {
    auto result = sockpp::inet_address::create(address, port);
    if (result.is_ok()) {
        return result.value();
    } else {
        throw InvalidAddressOrPortException(address, port);
    }
}

}  // namespace NetworkFramework
