/*
 *  Description: This file implements NetworkFramework::ConnectToServer(),
 *               which is defined in include/connect_to_server.h.
 *
 *  Author(s):
 *      Nictheboy Li    <nictheboy@outlook.com>
 *
 *  License:
 *      MIT License, feel free to use and modify this file!
 *
 */

#include <thread>
#include "connect_to_server.h"
#include "exceptions.h"
#include "sockpp/tcp_connector.h"
#include "sockpp_socket.h"
#include "validate_address.h"

std::unique_ptr<NetworkFramework::Socket>
NetworkFramework::ConnectToServer(const std::string& address_remote, int port_remote, int retry_count) {
    sockpp::initialize();
    try {
        auto addr = ValidateAddress(address_remote, port_remote);
        auto connector = std::make_unique<sockpp::tcp_connector>();
        sockpp::result<> result;
        do {
            result = connector->connect(addr);
            retry_count--;
            if (result.is_error() && retry_count > 0) {
                std::this_thread::sleep_for(std::chrono::seconds(1));
            }
        } while (result.is_error() && retry_count > 0);
        if (result.is_error()) {
            throw ConnectionEstablishmentException(
                address_remote,
                port_remote,
                result.error_message());
        }
        auto peer_address_port = addr.to_string();
        auto peer_address = peer_address_port.substr(0, peer_address_port.find(':'));
        auto peer_port = std::stoi(peer_address_port.substr(peer_address_port.find(':') + 1));
        return std::make_unique<SockppSocket>(std::move(connector), peer_address, peer_port);
    } catch (const std::system_error& error) {
        throw ConnectionEstablishmentException(
            address_remote,
            port_remote,
            error.what());
    } catch (const std::exception& error) {
        throw ConnectionEstablishmentException(
            address_remote,
            port_remote,
            error.what());
    } catch (...) {
        throw ConnectionEstablishmentException(
            address_remote,
            port_remote,
            "Unknown error");
    }
}
