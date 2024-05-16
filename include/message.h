// See: https://github.com/panjd123/Surakarta/blob/main/network/src/networkdata.h

#pragma once
#include <string>

namespace NetworkFramework {

/// @brief The different types of messages that can be sent.
typedef int Opcode;

/// @brief A message that can be sent over the network.
struct Message {
    Opcode opcode;
    std::string data1;
    std::string data2;
    std::string data3;

    Message(Opcode opcode = 0, const std::string& data1 = "", const std::string& data2 = "", const std::string& data3 = "")
        : opcode(opcode), data1(data1), data2(data2), data3(data3) {}

    bool operator==(const Message& other) const {
        return opcode == other.opcode && data1 == other.data1 && data2 == other.data2 && data3 == other.data3;
    }
};

}  // namespace NetworkFramework
