#include "sockpp/tcp_connector.h"

void test() {
    auto conn = sockpp::tcp_connector();
    conn.connect("localhost", 8080);
}
