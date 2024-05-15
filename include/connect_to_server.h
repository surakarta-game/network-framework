#pragma once
#include <memory>
#include "socket.h"

namespace NetworkFramework {

/// @brief Connect to a server.
/// @param address_remote The IP address of the server.
/// @param port_remote The port number of the server.
/// @return A socket that is connected to the server.
/// @throw ConnectionEstablishmentException if the connection could not be established.
std::unique_ptr<Socket> ConnectToServer(const std::string& address_remote, int port_remote);

}  // namespace NetworkFramework