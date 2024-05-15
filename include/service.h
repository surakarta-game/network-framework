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

}  // namespace NetworkFramework
