#pragma once

#pragma once
#include <string>
#include <thread>
#include "connect_to_server.h"
#include "service.h"
#include "sockpp/tcp6_acceptor.h"
#include "sockpp_socket.h"
#include "validate_address.h"

namespace NetworkFramework {

class ServerImpl {
   private:
    class Daemon {
       public:
        Daemon(sockpp::tcp6_acceptor& acceptor, std::shared_ptr<ServiceFactory> service_factory)
            : acceptor_(acceptor), service_factory_(service_factory) {}

        void operator()() {
            while (true) {
                auto result = acceptor_.accept();
                if (acceptor_.is_open() == false) {
                    break;
                }
                if (result.is_error()) {
                    break;
                }
                auto wrapped_socket = std::make_shared<SockppSocket>(
                    std::make_unique<sockpp::tcp6_socket>(result.value().clone()));
                sockets_.push_back(wrapped_socket);
                std::shared_ptr<Service> service = service_factory_->Create();
                auto thread = std::make_unique<std::thread>([service, wrapped_socket]() {
                    try {
                        service->Execute(wrapped_socket);
                    } catch (const std::exception& e) {
                        std::cerr << "Exception in service: " << e.what() << std::endl;
                    }
                    // wrapped_socket->Close();
                });
                threads_.push_back(std::move(thread));
            }
            Shutdown();
        }

       private:
        sockpp::tcp6_acceptor& acceptor_;
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
    int port_;
    sockpp::tcp6_acceptor acceptor_;
    sockpp::error_code acceptor_error_code_;
    std::shared_ptr<Daemon> daemon_;
    std::unique_ptr<std::thread> daemon_thread_;

   public:
    ServerImpl(std::shared_ptr<ServiceFactory> service_factory,
               int listen_port)
        : port_(listen_port), acceptor_(listen_port, 5, acceptor_error_code_) {
        sockpp::initialize();
        if (acceptor_error_code_)
            throw BindPortException(listen_port, acceptor_error_code_.message());
        daemon_ = std::make_unique<Daemon>(acceptor_, service_factory);
        auto daemon_ptr_copy = daemon_;
        daemon_thread_ = std::make_unique<std::thread>([daemon_ptr_copy]() {
            (*daemon_ptr_copy)();
        });
    }

    ~ServerImpl() {
        Shutdown();
    }

    void Shutdown() {
        if (acceptor_.is_open()) {
            acceptor_.shutdown();
            acceptor_.close();
        }
        if (daemon_thread_ && daemon_thread_->joinable()) {
            try {
                auto temp_client = ConnectToServer("localhost", port_);  // Connect to server to unblock acceptor_.accept()
            } catch (...) {
                // Ignore the exception
            }
            daemon_thread_->join();
        }
    }
};

}  // namespace NetworkFramework
