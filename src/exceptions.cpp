#include "exceptions.h"

NetworkFramework::BrokenPipeException::BrokenPipeException(
    const std::string& address_remote,
    int port_remote,
    const std::string& address_local,
    int port_local)
    : address_remote_(address_remote),
      port_remote_(port_remote),
      address_local_(address_local),
      port_local_(port_local),
      what_("Broken pipe: remote address: " + address_remote + ":" + std::to_string(port_remote) +
            ", local address: " + address_local + ":" + std::to_string(port_local)) {}

const char* NetworkFramework::BrokenPipeException::what() const noexcept {
    return what_.c_str();
}

std::string NetworkFramework::BrokenPipeException::RemoteAddress() const noexcept {
    return address_remote_;
}

int NetworkFramework::BrokenPipeException::RemotePort() const noexcept {
    return port_remote_;
}

std::string NetworkFramework::BrokenPipeException::LocalAddress() const noexcept {
    return address_local_;
}

int NetworkFramework::BrokenPipeException::LocalPort() const noexcept {
    return port_local_;
}

NetworkFramework::ConnectionEstablishmentException::ConnectionEstablishmentException(
    const std::string& address_remote,
    int port_remote,
    const std::string& address_local,
    int port_local)
    : address_remote_(address_remote),
      port_remote_(port_remote),
      address_local_(address_local),
      port_local_(port_local),
      what_("Connection establishment failed: remote address: " + address_remote + ":" + std::to_string(port_remote) +
            ", local address: " + address_local + ":" + std::to_string(port_local)) {}

const char* NetworkFramework::ConnectionEstablishmentException::what() const noexcept {
    return what_.c_str();
}

std::string NetworkFramework::ConnectionEstablishmentException::RemoteAddress() const noexcept {
    return address_remote_;
}

int NetworkFramework::ConnectionEstablishmentException::RemotePort() const noexcept {
    return port_remote_;
}

std::string NetworkFramework::ConnectionEstablishmentException::LocalAddress() const noexcept {
    return address_local_;
}

int NetworkFramework::ConnectionEstablishmentException::LocalPort() const noexcept {
    return port_local_;
}
