#pragma once
#include <stdexcept>
#include <string>

namespace NetworkFramework {

/// @brief Base class for all exceptions thrown by the network framework.
class BaseException : public std::exception {};

/// @brief An exception that is thrown when a socket is closed unexpectedly.
class BrokenPipeException final : public BaseException {
   public:
    BrokenPipeException(const std::string& address_remote,
                        int port_remote,
                        const std::string& address_local,
                        int port_local,
                        const std::string& details);

    const char* what() const noexcept override;

    std::string RemoteAddress() const noexcept;
    int RemotePort() const noexcept;
    std::string LocalAddress() const noexcept;
    int LocalPort() const noexcept;
    std::string Details() const noexcept;

   private:
    std::string what_;
    std::string address_remote_;
    int port_remote_;
    std::string address_local_;
    int port_local_;
    std::string details_;
};

/// @brief An exception that is thrown when a connection could not be established.
class ConnectionEstablishmentException final : public BaseException {
   public:
    ConnectionEstablishmentException(const std::string& address_remote,
                                     int port_remote,
                                     const std::string& address_local,
                                     int port_local,
                                     const std::string& details);

    const char* what() const noexcept override;

    std::string RemoteAddress() const noexcept;
    int RemotePort() const noexcept;
    std::string LocalAddress() const noexcept;
    int LocalPort() const noexcept;
    std::string Details() const noexcept;

   private:
    std::string what_;
    std::string address_remote_;
    int port_remote_;
    std::string address_local_;
    int port_local_;
    std::string details_;
};

}  // namespace NetworkFramework
