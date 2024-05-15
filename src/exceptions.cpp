#include "exceptions.h"

NetworkFramework::InvalidAddressOrPortException::InvalidAddressOrPortException(const std::string& address,
                                                                               int port)
    : address_(address),
      port_(port),
      what_("Invalid address: " + address + ":" + std::to_string(port)) {}

const char* NetworkFramework::InvalidAddressOrPortException::what() const noexcept {
    return what_.c_str();
}

std::string NetworkFramework::InvalidAddressOrPortException::Address() const noexcept {
    return address_;
}

int NetworkFramework::InvalidAddressOrPortException::Port() const noexcept {
    return port_;
}

NetworkFramework::BrokenPipeException::BrokenPipeException(
    const std::string& address_remote,
    int port_remote,
    const std::string& address_local,
    int port_local,
    const std::string& details)
    : address_remote_(address_remote),
      port_remote_(port_remote),
      address_local_(address_local),
      port_local_(port_local),
      what_("Broken pipe: remote address: " + address_remote + ":" + std::to_string(port_remote) +
            ", local address: " + address_local + ":" + std::to_string(port_local) + ", details: " + details),
      details_(details) {}

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

std::string NetworkFramework::BrokenPipeException::Details() const noexcept {
    return details_;
}

NetworkFramework::ConnectionEstablishmentException::ConnectionEstablishmentException(
    const std::string& address_remote,
    int port_remote,
    const std::string& details)
    : address_remote_(address_remote),
      port_remote_(port_remote),
      what_("Connection establishment failed: remote address: " + address_remote + ":" + std::to_string(port_remote) +
            ", details: " + details),
      details_(details) {}

const char* NetworkFramework::ConnectionEstablishmentException::what() const noexcept {
    return what_.c_str();
}

std::string NetworkFramework::ConnectionEstablishmentException::RemoteAddress() const noexcept {
    return address_remote_;
}

int NetworkFramework::ConnectionEstablishmentException::RemotePort() const noexcept {
    return port_remote_;
}

std::string NetworkFramework::ConnectionEstablishmentException::Details() const noexcept {
    return details_;
}

NetworkFramework::InvalidMessageException::InvalidMessageException(const std::string& message, const std::string& details)
    : message_(message),
      what_("Invalid message: message: " + message + ", details: " + details),
      details_(details) {}

const char* NetworkFramework::InvalidMessageException::what() const noexcept {
    return what_.c_str();
}

std::string NetworkFramework::InvalidMessageException::Message() const noexcept {
    return message_;
}

std::string NetworkFramework::InvalidMessageException::Details() const noexcept {
    return details_;
}

NetworkFramework::BindPortException::BindPortException(const std::string& address, int port, const std::string& details)
    : address_(address),
      port_(port),
      what_("Failed to bind port: address: " + address + ":" + std::to_string(port) + ", details: " + details),
      details_(details) {}

const char* NetworkFramework::BindPortException::what() const noexcept {
    return what_.c_str();
}

std::string NetworkFramework::BindPortException::Address() const noexcept {
    return address_;
}

int NetworkFramework::BindPortException::Port() const noexcept {
    return port_;
}

std::string NetworkFramework::BindPortException::Details() const noexcept {
    return details_;
}
