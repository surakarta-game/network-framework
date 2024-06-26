/*
 *  Description: This file defines NetworkFramework::Server, which is a class
 *               that represents a server.
 *
 *  Author(s):
 *      Nictheboy Li    <nictheboy@outlook.com>
 *
 *  License:
 *      MIT License, feel free to use and modify this file!
 *
 */

#pragma once
#include <string>
#include "service.h"

namespace NetworkFramework {

class ServerImpl;

/// @brief A class that represents a server.
class Server {
   public:
    /// @brief
    /// Start listening on the given address and port.
    /// New connections will be handled by a service object, in a new thread.
    /// @param service A service object which will be used to handle incoming connections.
    /// @param listen_port The port to listen on.
    Server(std::shared_ptr<Service> service,
           int listen_port);
    ~Server();

    /// @brief Stop listening and close all connections.
    void Shutdown();

   private:
    std::unique_ptr<ServerImpl> impl_;
};

}  // namespace NetworkFramework
