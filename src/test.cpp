/*
 *  Description: This file is a test program for the network framework.
 *
 *               However, it's also a good example of how to use the network framework.
 *
 *  Author(s):
 *      Nictheboy Li    <nictheboy@outlook.com>
 *
 *  License:
 *      MIT License, feel free to use and modify this file!
 *
 */

#include <condition_variable>
#include <mutex>
#include "network_framework.h"

// Define the opcodes
enum MyOpcode : NetworkFramework::Opcode {
    OpError = -1,
    OpExit = 0,
    Op1 = 1,
    Op2 = 2,
    Op3 = 3,
    Op4 = 4,
};

// Define the service class that handles all connections.
class RelayService : public NetworkFramework::Service {
   private:
    // Define the data that is shared among the service threads
    // which are created every time a new connection is established.

    // This class is shared among threads, so we need to use mutex to protect it.
    std::mutex mutex;

    // This condition variable is used to notify the first service thread,
    // when the second service thread is created.
    std::condition_variable condition_variable;

    // The socket that is created when the first connection is established.
    std::shared_ptr<NetworkFramework::Socket> first_socket;

    // The socket that is created when the second connection is established.
    std::shared_ptr<NetworkFramework::Socket> second_socket;

   public:
    void Execute(std::shared_ptr<NetworkFramework::Socket> socket) override {
        std::shared_ptr<NetworkFramework::Socket> peer_socket;
        {  // use lock to protect shared_data, when accessing it
            std::unique_lock<std::mutex> lock(mutex);
            if (first_socket == nullptr) {
                first_socket = socket;

                // wait until the second socket is created
                condition_variable.wait(lock, [&] {
                    return second_socket != nullptr;
                });

                peer_socket = second_socket;
            } else if (second_socket == nullptr) {  // && first_socket != nullptr
                second_socket = socket;

                // notify the first socket that the second socket is created
                condition_variable.notify_one();

                peer_socket = first_socket;
            } else {
                socket->Send(NetworkFramework::Message(OpError, "Server is full."));
                socket->Close();
                return;
            }
        }
        try {
            while (true) {
                auto message = socket->Receive();  // Wait for a message from the client
                if (message.has_value() == false || message.value().opcode == OpExit) {
                    break;
                }
                peer_socket->Send(message.value());  // Send the message to the peer
            }
        } catch (const NetworkFramework::BrokenPipeException&) {
            // Unexpected close from the client side may cause BrokenPipeException.
            // Just ignore it.
        }
        std::lock_guard<std::mutex> lock(mutex);
        if (first_socket) {
            first_socket->Send(NetworkFramework::Message(OpExit));
            first_socket->Close();
            first_socket = nullptr;
        }
        if (second_socket) {
            second_socket->Send(NetworkFramework::Message(OpExit));
            second_socket->Close();
            second_socket = nullptr;
        }
    }
};

// System assert() may not work in some cases, so we use our own one.
void Assert(bool value) {
    if (!value) {
        printf("Assert failed!\n");
        exit(-1);
    }
}

int main() {
    constexpr int port = 7777;

    // When this object is created, the server starts listening on the given port.
    NetworkFramework::Server server(std::make_unique<RelayService>(), port);

    // Connect to the server
    auto client1 = NetworkFramework::ConnectToServer("localhost", port);
    auto client2 = NetworkFramework::ConnectToServer("127.0.0.1", port);
    auto client3 = NetworkFramework::ConnectToServer("127.0.0.1", port);

    Assert(client3->Receive().value().opcode == OpError);
    Assert(client3->Receive().has_value() == false);

    client1->Send(NetworkFramework::Message(Op1, "Hello, client2!"));
    client2->Send(NetworkFramework::Message(Op2, "Hello, client1!"));
    Assert(client1->Receive().value() == NetworkFramework::Message(Op2, "Hello, client1!"));
    Assert(client2->Receive().value() == NetworkFramework::Message(Op1, "Hello, client2!"));

    client1->Send(NetworkFramework::Message(Op3, "Goodbye, client2!"));
    client2->Send(NetworkFramework::Message(Op4, "Goodbye, client1!"));
    Assert(client1->Receive().value() == NetworkFramework::Message(Op4, "Goodbye, client1!"));
    Assert(client2->Receive().value() == NetworkFramework::Message(Op3, "Goodbye, client2!"));

    client1->Close();
    Assert(client1->Receive().has_value() == false);
    Assert(client2->Receive().value().opcode == OpExit);
    Assert(client2->Receive().has_value() == false);

    // Manual shutdown is not necessary, since Shutdown() is called in the destructor.
    // However, shutdown manually is also supported.
    server.Shutdown();
    return 0;
}
