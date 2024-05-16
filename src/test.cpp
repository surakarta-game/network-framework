#include <condition_variable>
#include <mutex>
#include "network_framework.h"

#define TEST_PORT 7777

enum MyOpcode : NetworkFramework::Opcode {
    OpError = -1,
    OpExit = 0,
    Op1 = 1,
    Op2 = 2,
    Op3 = 3,
    Op4 = 4,
};

struct SharedData {
    std::mutex mutex;
    std::condition_variable condition_variable;
    std::shared_ptr<NetworkFramework::Socket> first_socket;
    std::shared_ptr<NetworkFramework::Socket> second_socket;
};

class RelayService : public NetworkFramework::Service {
   private:
    std::shared_ptr<SharedData> shared_data;

   public:
    RelayService(std::shared_ptr<SharedData> shared_data)
        : shared_data(shared_data) {}

    void Execute(std::shared_ptr<NetworkFramework::Socket> socket) override {
        std::shared_ptr<NetworkFramework::Socket> peer_socket;
        {
            std::unique_lock<std::mutex> lock(shared_data->mutex);
            if (shared_data->first_socket == nullptr) {
                shared_data->first_socket = socket;
                shared_data->condition_variable.wait(lock, [&] {
                    return shared_data->second_socket != nullptr;
                });
                peer_socket = shared_data->second_socket;
            } else if (shared_data->second_socket == nullptr) {  // && shared_data->first_socket != nullptr
                shared_data->second_socket = socket;
                shared_data->condition_variable.notify_one();
                peer_socket = shared_data->first_socket;
            } else {
                socket->Send(NetworkFramework::Message(OpError, "Server is full."));
                socket->Close();
                return;
            }
        }
        while (true) {
            auto message = socket->Receive();
            if (message.has_value() == false || message.value().opcode == OpExit) {
                break;
            }
            peer_socket->Send(message.value());
        }
        peer_socket->Send(NetworkFramework::Message(OpExit));
        peer_socket->Close();
    }
};

class ServiceFactory : public NetworkFramework::ServiceFactory {
   private:
    std::shared_ptr<SharedData> shared_data;

   public:
    ServiceFactory()
        : shared_data(std::make_shared<SharedData>()) {}

    std::unique_ptr<NetworkFramework::Service> Create() override {
        return std::make_unique<RelayService>(shared_data);
    }
};

// assert() is not always asserted
void Assert(bool value) {
    if (!value) {
        printf("Assert failed!\n");
        exit(-1);
    }
}

int main() {
    NetworkFramework::Server server(std::make_shared<ServiceFactory>(), TEST_PORT);

    auto client1 = NetworkFramework::ConnectToServer("127.0.0.1", TEST_PORT);
    auto client2 = NetworkFramework::ConnectToServer("localhost", TEST_PORT);
    auto client3 = NetworkFramework::ConnectToServer("::1", TEST_PORT);

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

    client1->Send(NetworkFramework::Message(OpExit));
    Assert(client2->Receive().value().opcode == OpExit);
    Assert(client2->Receive().has_value() == false);

    client2->Close();
    Assert(client1->Receive().value().opcode == OpExit);
    Assert(client1->Receive().has_value() == false);

    server.Shutdown();
    return 0;
}
