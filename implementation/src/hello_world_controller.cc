#include "xdr/experiments.h"
#include "xdr/hello_world_api.h"
#include "rpc/rpcconfig.h"
#include <xdrpp/srpc.h>
#include <thread>
#include <chrono>

#include "utils.h"

#include <cstdint>
#include <vector>

using namespace edce;

std::string hostname_from_idx(int idx) {
    return std::string("10.10.1.") + std::to_string(idx);
}

void
poll_node(int idx) {
    
    auto fd = xdr::tcp_connect(hostname_from_idx(idx).c_str(), HELLOWORLD_PORT);
    auto client = xdr::srpc_client<MyProgV1>(fd.get());

    std::printf("printing hello world \n");
    client.print_hello_world();
}


int main(int argc, char const *argv[]) {

    if (argc != 1) {
        std::printf("usage: ./hello_world_controller \n");
        return 0;
    }

    if (argc == 1) {
        poll_node(2);
    }

    return 0;

}
