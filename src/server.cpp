/*
 *  Description: This file implements NetworkFramework::Server
 *               defined in include/connect_to_server.h,
 *               using NetworkFramework::ServerImpl
 *               defined in src/private-include/server_impl.h
 *
 *  Author(s):
 *      Nictheboy Li    <nictheboy@outlook.com>
 *
 *  License:
 *      MIT License, feel free to use and modify this file!
 *
 */

#include "server.h"
#include "server_impl.h"

NetworkFramework::Server::Server(std::shared_ptr<Service> service, int listen_port) {
    impl_ = std::make_unique<ServerImpl>(service, listen_port);
}

NetworkFramework::Server::~Server() {
    Shutdown();
}

void NetworkFramework::Server::Shutdown() {
    impl_->Shutdown();
}
