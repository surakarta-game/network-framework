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
