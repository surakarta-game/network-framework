/*
 *  Description: This file defines an abstract socket for
 *               sending and receiving messages.
 *
 *  Author(s):
 *      Nictheboy Li    <nictheboy@outlook.com>
 *
 *  License:
 *      MIT License, feel free to use and modify this file!
 *
 */

#pragma once
#include <optional>
#include "exceptions.h"
#include "message.h"

namespace NetworkFramework {

/// @brief An abstract socket for sending and receiving messages.
class Socket {
   public:
    virtual ~Socket() = default;

    /// @brief Send a message.
    /// @param message The message to send.
    virtual void Send(Message message) = 0;

    /// @brief Receive a message.
    /// @return The received message, or std::nullopt if the connection was closed.
    virtual std::optional<Message> Receive() = 0;

    /// @brief Close the socket.
    virtual void Close() = 0;

    /// @brief Get the address of the peer.
    /// @return The address of the peer.
    virtual std::string PeerAddress() const = 0;

    /// @brief Get the port of the peer.
    /// @return The port of the peer.
    virtual int PeerPort() const = 0;
};

}  // namespace NetworkFramework
