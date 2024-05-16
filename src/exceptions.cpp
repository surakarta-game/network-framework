/*
 *  Description: This file implements all the exceptions 
 *               defined in include/exceptions.h
 *
 *  Author(s):
 *      Nictheboy Li    <nictheboy@outlook.com>
 *
 *  License:
 *      MIT License, feel free to use and modify this file!
 *
 */

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
    const std::string& details)
    : what_("Broken pipe: details: " + details),
      details_(details) {}

const char* NetworkFramework::BrokenPipeException::what() const noexcept {
    return what_.c_str();
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

NetworkFramework::BindPortException::BindPortException(int port, const std::string& details)
    : port_(port),
      what_("Failed to bind port: " + std::to_string(port) + ", details: " + details),
      details_(details) {}

const char* NetworkFramework::BindPortException::what() const noexcept {
    return what_.c_str();
}

int NetworkFramework::BindPortException::Port() const noexcept {
    return port_;
}

std::string NetworkFramework::BindPortException::Details() const noexcept {
    return details_;
}
