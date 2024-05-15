// See: https://github.com/panjd123/Surakarta/blob/main/network/src/networkdata.h

#pragma once
#include <string>

namespace NetworkFramework {

/// @brief The different types of messages that can be sent.
enum class Opcode : int {
    READY_OP = 200000,
    MOVE_OP,
    RESIGN_OP,
    REJECT_OP,
    LEAVE_OP,
    CHAT_OP,
    END_OP,
};

/// @brief A message that can be sent over the network.
struct Message {
    Opcode opcode;
    std::string data1;
    std::string data2;
    std::string data3;
};

}  // namespace NetworkFramework
