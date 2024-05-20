/*
 *  Description: This file implements the abstract socket
 *               defined in include/socket.h, using the Sockpp library.
 *
 *  Author(s):
 *      Nictheboy Li    <nictheboy@outlook.com>
 *
 *  License:
 *      MIT License, feel free to use and modify this file!
 *
 */

#pragma once
#include <stdio.h>
#include <mutex>
#include <streambuf>
#include "exceptions.h"
#include "nlohmann/json.hpp"
#include "sockpp/inet_address.h"

namespace NetworkFramework {

class SockppSocket final : public Socket {
   private:
    std::string peer_address;
    int peer_port;
    std::unique_ptr<sockpp::socket> socket_read;
    std::unique_ptr<sockpp::socket> socket_write;
    std::string received_string;
    std::mutex mutex_read;
    std::mutex mutex_write;

   public:
    SockppSocket(std::unique_ptr<sockpp::socket> socket, std::string peer_address, int peer_port)
        : socket_read(std::make_unique<sockpp::socket>(socket->clone())),
          socket_write(std::make_unique<sockpp::socket>(socket->clone())),
          peer_address(peer_address),
          peer_port(peer_port) {}

    ~SockppSocket() override {
        Close();
    }

    void Send(Message message) override {
        // Send the message to the server
        std::lock_guard lk(mutex_write);
        nlohmann::json message_json = {
            {"op", static_cast<int>(message.opcode)},
            {"data1", message.data1},
            {"data2", message.data2},
            {"data3", message.data3},
        };
        std::string message_json_str = message_json.dump();
        assert(message_json_str.find('\n') == std::string::npos);  // Ensure that the message does not contain a newline character
        std::string message_str = message_json_str + "\n";
        auto result = socket_write->send(message_str);
        if (result.is_error()) {
            throw BrokenPipeException(result.error_message());
        }
    }

    std::optional<Message> Receive() override {
        // Receive the message from the server
        while (received_string.find('\n') == std::string::npos) {
            if (socket_read->is_open() == false) {
                return std::nullopt;
            }
            bool result = ReceiveOne();
            if (result == false) {
                return std::nullopt;
            }
        }
        auto newline_index = received_string.find('\n');
        std::string message_str = received_string.substr(0, newline_index);
        received_string = received_string.substr(newline_index + 1);
        nlohmann::json message_json;
        try {
            message_json = nlohmann::json::parse(message_str);
            Message message;
            if (message_json.contains("op") && message_json["op"].is_number_integer()) {
                message.opcode = static_cast<Opcode>(message_json["op"].get<int>());
            } else {
                throw InvalidMessageException(
                    message_str,
                    "Missing or invalid opcode");
            }
            if (message_json.contains("data1") && message_json["data1"].is_string()) {
                message.data1 = message_json["data1"].get<std::string>();
            } else {
                throw InvalidMessageException(
                    message_str,
                    "Missing or invalid data1");
            }
            if (message_json.contains("data2") && message_json["data2"].is_string()) {
                message.data2 = message_json["data2"].get<std::string>();
            } else {
                throw InvalidMessageException(
                    message_str,
                    "Missing or invalid data2");
            }
            if (message_json.contains("data3") && message_json["data3"].is_string()) {
                message.data3 = message_json["data3"].get<std::string>();
            } else {
                throw InvalidMessageException(
                    message_str,
                    "Missing or invalid data3");
            }
            return message;
        } catch (const nlohmann::json::parse_error& error) {
            throw InvalidMessageException(
                message_str,
                error.what());
        }
    }

    void Close() override {
        // Close the connection
        socket_write->shutdown();
        socket_write->close();
        socket_read->shutdown();
        socket_read->close();
    }

    std::string PeerAddress() const override {
        return peer_address;
    }

    int PeerPort() const override {
        return peer_port;
    }

   private:
    bool ReceiveOne() {
        std::lock_guard lk(mutex_read);
        if (socket_read->is_open() == false) {
            return false;
        }
        char buffer[1024 + 1];
        auto result = socket_read->recv(buffer, sizeof(buffer) - 1);
        if (result.is_error()) {
            throw BrokenPipeException(result.error_message());
        }
        int length = result.value();
        if (length == 0) {
            return false;
        }
        assert(length < sizeof(buffer));
        buffer[length] = '\0';
        if (length > 0 && buffer[length - 1] != '\n') {
            printf("Received message without newline character, this may cause bugs in other implementations of the protocol");
        }
        received_string += buffer;
        return true;
    }
};

}  // namespace NetworkFramework
