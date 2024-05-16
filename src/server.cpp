#include "server.h"
#include "server_impl.h"

NetworkFramework::Server::Server(std::shared_ptr<ServiceFactory> service_factory, int listen_port) {
    impl_ = std::make_unique<ServerImpl>(service_factory, listen_port);
}

NetworkFramework::Server::~Server() {
    Shutdown();
}

void NetworkFramework::Server::Shutdown() {
    impl_->Shutdown();
}
