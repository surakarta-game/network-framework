#include "server.h"
#include "server_impl.h"

NetworkFramework::Server::Server(std::shared_ptr<ServiceFactory> service_factory, int listen_port, const std::string& listen_address) {
    impl_ = std::make_unique<ServerImpl>(service_factory, listen_port, listen_address);
}

NetworkFramework::Server::~Server() {
    Shutdown();
}

void NetworkFramework::Server::Shutdown() {
    impl_->Shutdown();
}
