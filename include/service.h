/*
 *  Description: This file defines NetworkFramework::Service
 *               and NetworkFramework::ServiceFactory.
 *
 *               You need to implements these classes to create a server.
 *
 *  Author(s):
 *      Nictheboy Li    <nictheboy@outlook.com>
 *
 *  License:
 *      MIT License, feel free to use and modify this file!
 *
 */

#pragma once
#include <memory>
#include "socket.h"

namespace NetworkFramework {

/// @brief A class that handles a connection.
class Service {
   public:
    virtual ~Service() = default;

    /// @brief Handle a connection.
    /// @param socket The socket of this connection.
    virtual void Execute(std::shared_ptr<Socket> socket) = 0;
};

/// @brief A factory for creating service instances.
class ServiceFactory {
   public:
    virtual ~ServiceFactory() = default;

    /// @brief Create a new service instance.
    /// @return A new service instance.
    virtual std::unique_ptr<Service> Create() = 0;
};

}  // namespace NetworkFramework
