#pragma once

#pragma once
#include <string>
#include <thread>
#include "service.h"
#include "sockpp/acceptor.h"
#include "sockpp_socket.h"
#include "validate_address.h"

namespace NetworkFramework {

class ServerImpl {
   private:
    class Daemon {
       public:
        Daemon(sockpp::acceptor& acceptor, std::shared_ptr<ServiceFactory> service_factory)
            : acceptor_(acceptor), service_factory_(service_factory) {}

        void operator()() {
            while (true) {
                auto socket = acceptor_.accept();
                if (socket.is_error()) {
                    break;
                }
                auto wrapped_socket = std::make_shared<SockppSocket>(socket.value().clone());
                sockets_.push_back(wrapped_socket);
                auto thread = std::make_unique<std::thread>([this, wrapped_socket]() {
                    auto service = service_factory_->Create();
                    service->Execute(wrapped_socket);
                });
                threads_.push_back(std::move(thread));
            }
            Shutdown();
        }

       private:
        sockpp::acceptor& acceptor_;
        std::shared_ptr<ServiceFactory> service_factory_;
        std::vector<std::shared_ptr<SockppSocket>> sockets_;
        std::vector<std::unique_ptr<std::thread>> threads_;

        void Shutdown() {
            for (auto socket : sockets_) {
                socket->Close();
            }
            for (auto& thread : threads_) {
                thread->join();
            }
        }
    };

   private:
    sockpp::acceptor acceptor_;
    std::unique_ptr<std::thread> daemon_thread_;

   public:
    ServerImpl(std::shared_ptr<ServiceFactory> service_factory,
               int listen_port,
               const std::string& listen_address = "::") {
        auto address = ValidateAddress(listen_address, listen_port);
        try {
            acceptor_.bind(address);
        } catch (const std::exception& e) {
            throw BindPortException(listen_address, listen_port, e.what());
        }
        acceptor_.listen();
        daemon_thread_ = std::make_unique<std::thread>(Daemon(acceptor_, service_factory));
    }

    ~ServerImpl() {
        Shutdown();
    }

    void Shutdown() {
        acceptor_.close();
        if (daemon_thread_ && daemon_thread_->joinable()) {
            daemon_thread_->join();
        }
    }
};

}  // namespace NetworkFramework
