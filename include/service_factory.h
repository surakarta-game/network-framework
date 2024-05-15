#pragma once
#include <memory>
#include "service.h"

namespace NetworkFramework {

/// @brief A factory for creating service instances.
class ServiceFactory {
   public:
    virtual ~ServiceFactory() = default;

    /// @brief Create a new service instance.
    /// @return A new service instance.
    virtual std::unique_ptr<Service> Create() = 0;
};

}  // namespace NetworkFramework
