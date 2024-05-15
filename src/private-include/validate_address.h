#pragma once
#include <string>
#include "sockpp/connector.h"

namespace NetworkFramework {

inline sockpp::inet6_address ValidateAddress(const std::string& address, int port) {
    try {
        return sockpp::inet6_address(address, port);
    } catch (const std::exception&) {
        throw InvalidAddressOrPortException(address, port);
    }
}

}  // namespace NetworkFramework
