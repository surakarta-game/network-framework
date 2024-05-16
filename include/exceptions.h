#pragma once
#include <stdexcept>
#include <string>

namespace NetworkFramework {

/// @brief Base class for all exceptions thrown by the network framework.
class BaseException : public std::exception {};

/// @brief An exception that is thrown when an invalid address is used.
class InvalidAddressOrPortException final : public BaseException {
   public:
    InvalidAddressOrPortException(const std::string& address, int port);

    const char* what() const noexcept override;

    std::string Address() const noexcept;
    int Port() const noexcept;

   private:
    std::string what_;
    std::string address_;
    int port_;
};

/// @brief An exception that is thrown when a socket is closed unexpectedly.
class BrokenPipeException final : public BaseException {
   public:
    BrokenPipeException(const std::string& details);

    const char* what() const noexcept override;

    std::string Details() const noexcept;

   private:
    std::string what_;
    std::string details_;
};

/// @brief An exception that is thrown when a connection could not be established.
class ConnectionEstablishmentException final : public BaseException {
   public:
    ConnectionEstablishmentException(const std::string& address_remote,
                                     int port_remote,
                                     const std::string& details);

    const char* what() const noexcept override;

    std::string RemoteAddress() const noexcept;
    int RemotePort() const noexcept;
    std::string Details() const noexcept;

   private:
    std::string what_;
    std::string address_remote_;
    int port_remote_;
    std::string details_;
};

/// @brief An exception that is thrown when an invalid message is received.
class InvalidMessageException final : public BaseException {
   public:
    InvalidMessageException(const std::string& message,
                            const std::string& details);

    const char* what() const noexcept override;

    std::string Message() const noexcept;
    std::string Details() const noexcept;

   private:
    std::string what_;
    std::string message_;
    std::string details_;
};

class BindPortException final : public BaseException {
   public:
    BindPortException(int port, const std::string& details);

    const char* what() const noexcept override;

    int Port() const noexcept;
    std::string Details() const noexcept;

   private:
    std::string what_;
    int port_;
    std::string details_;
};

}  // namespace NetworkFramework
