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
    /// New connections will be handled by the given service factory, in a new thread.
    /// @param service_factory A factory that creates services which will be used to handle incoming connections.
    /// @param listen_port The port to listen on.
    /// @param listen_address The address to listen on, use :: for all network interfaces.
    Server(std::shared_ptr<ServiceFactory> service_factory,
           int listen_port,
           const std::string& listen_address = "::");
    ~Server();

    /// @brief Stop listening and close all connections.
    void Shutdown();

   private:
    std::unique_ptr<ServerImpl> impl_;
};

}  // namespace NetworkFramework
