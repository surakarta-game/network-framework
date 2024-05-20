/*
 *  Description: This file implements the server, using
 *               the Sockpp library and NetworkFramework::SockppSocket.
 *
 *  Author(s):
 *      Nictheboy Li    <nictheboy@outlook.com>
 *
 *  License:
 *      MIT License, feel free to use and modify this file!
 *
 */

#pragma once

#pragma once
#include <string>
#include <thread>
#include "connect_to_server.h"
#include "service.h"
#include "sockpp/tcp_acceptor.h"
#include "sockpp_socket.h"
#include "validate_address.h"

namespace NetworkFramework {

class ServerImpl {
   private:
    class Daemon {
       public:
        Daemon(std::unique_ptr<sockpp::tcp_acceptor> acceptor, std::shared_ptr<ServiceFactory> service_factory)
            : acceptor_(std::move(acceptor)), service_factory_(service_factory) {}

        void operator()() {
            while (true) {
                sockpp::inet_address peer_address;
                auto result = acceptor_->accept(&peer_address);
                if (acceptor_->is_open() == false) {
                    break;
                }
                if (result.is_error()) {
                    break;
                }
                std::string peer_address_port = peer_address.to_string();
                std::string peer_address_str = peer_address_port.substr(0, peer_address_port.find(':'));
                int peer_port = std::stoi(peer_address_port.substr(peer_address_port.find(':') + 1));
                auto wrapped_socket = std::make_shared<SockppSocket>(
                    std::make_unique<sockpp::tcp_socket>(result.value().clone()), peer_address_str, peer_port);
                sockets_.push_back(wrapped_socket);
                std::shared_ptr<Service> service = service_factory_->Create();
                auto thread = std::make_unique<std::thread>([service, wrapped_socket]() {
                    service->Execute(wrapped_socket);
                });
                threads_.push_back(std::move(thread));
            }
        }

        void Shutdown() {
            if (acceptor_->is_open()) {
                acceptor_->shutdown();
                acceptor_->close();
            }
            for (auto socket : sockets_) {
                socket->Close();
            }
            for (auto& thread : threads_) {
                thread->join();
            }
        }

       private:
        std::unique_ptr<sockpp::tcp_acceptor> acceptor_;
        std::shared_ptr<ServiceFactory> service_factory_;
        std::vector<std::shared_ptr<SockppSocket>> sockets_;
        std::vector<std::unique_ptr<std::thread>> threads_;
    };

   private:
    int port_;
    std::shared_ptr<Daemon> daemon_;
    std::unique_ptr<std::thread> daemon_thread_;

   public:
    ServerImpl(std::shared_ptr<ServiceFactory> service_factory,
               int listen_port)
        : port_(listen_port) {
        sockpp::initialize();
        sockpp::error_code acceptor_error_code;
        std::unique_ptr<sockpp::tcp_acceptor> acceptor = std::make_unique<sockpp::tcp_acceptor>(listen_port, 5, acceptor_error_code);
        if (acceptor_error_code)
            throw BindPortException(listen_port, acceptor_error_code.message());
        daemon_ = std::make_unique<Daemon>(std::move(acceptor), service_factory);
        auto daemon_ptr_copy = daemon_;
        daemon_thread_ = std::make_unique<std::thread>([daemon_ptr_copy]() {
            (*daemon_ptr_copy)();
        });
    }

    ~ServerImpl() {
        Shutdown();
    }

    void Shutdown() {
        if (daemon_thread_ && daemon_thread_->joinable()) {
            daemon_->Shutdown();
            try {
                auto temp_client = ConnectToServer("localhost", port_);  // Connect to server to unblock acceptor_->accept()
            } catch (...) {
                // Ignore the exception
            }
            daemon_thread_->join();
        }
    }
};

}  // namespace NetworkFramework
