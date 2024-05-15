#pragma once
#include <stdio.h>
#include <streambuf>
#include "exceptions.h"
#include "nlohmann/json.hpp"
#include "sockpp/tcp6_connector.h"

namespace NetworkFramework {

class SockppSocket final : public Socket {
   private:
    sockpp::tcp6_connector socket;
    std::string received_string;

   public:
    SockppSocket(sockpp::tcp6_connector&& socket)
        : socket(std::move(socket)) {}

    ~SockppSocket() override {
        Close();
    }

    void Send(Message message) override {
        // Send the message to the server
        nlohmann::json message_json = {
            {"op", static_cast<int>(message.opcode)},
            {"data1", message.data1},
            {"data2", message.data2},
            {"data3", message.data3},
        };
        std::string message_json_str = message_json.dump();
        assert(message_json_str.find('\n') == std::string::npos);  // Ensure that the message does not contain a newline character
        std::string message_str = message_json_str + "\n";
        socket.send(message_str);
    }

    std::optional<Message> Receive() override {
        // Receive the message from the server
        while (received_string.find('\n') == std::string::npos) {
            ReceiveOne();
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
        socket.close();
    }

   private:
    void ReceiveOne() {
        char buffer[1024 + 1];
        auto result = socket.recv(buffer, sizeof(buffer) - 1);
        if (result.is_error()) {
            const auto peer_address_string = socket.peer_address().to_string();
            const auto index = peer_address_string.find_last_of(':');
            const auto address_remote = peer_address_string.substr(0, index);
            const auto port_remote = std::stoi(peer_address_string.substr(index + 1));
            const auto local_address_string = socket.address().to_string();
            const auto local_index = local_address_string.find_last_of(':');
            const auto address_local = local_address_string.substr(0, local_index);
            const auto port_local = std::stoi(local_address_string.substr(local_index + 1));
            throw BrokenPipeException(
                address_remote,
                port_remote,
                address_local,
                port_local,
                result.error_message());
        }
        int length = result.value();
        assert(length < sizeof(buffer));
        buffer[length] = '\0';
        if (length > 0 && buffer[length - 1] != '\n') {
            printf("Received message without newline character, this may cause bugs in other implementations of the protocol");
        }
        received_string += buffer;
    }
};

}  // namespace NetworkFramework
