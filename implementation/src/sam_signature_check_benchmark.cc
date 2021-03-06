#include <iostream>
#include "utils.h"
#include "crypto_utils.h"


#include "xdr/experiments.h"
#include <vector>
#include "sam_crypto_utils.h"

#include <xdrpp/srpc.h>
#include <thread>
#include <chrono>

#include <cstdint>

using namespace edce;

std::string hostname_from_idx(int idx) {
    return std::string("10.10.1.") + std::to_string(idx);
}

void
poll_node(int idx) {
    
    auto fd = xdr::tcp_connect(hostname_from_idx(idx).c_str(), SIGNATURECHECK_PORT);
    auto client = xdr::srpc_client<SignatureCheckV1>(fd.get());

    std::printf("printing hello world \n");
    client.print_hello_world();
}


int main(int argc, char const *argv[])
{
    if (argc != 4) {
        std::printf("usage: ./blah experiment_name block_number num_threads\n");
        return -1;
    }

    DeterministicKeyGenerator key_gen;
    
    ExperimentParameters params;

    std::string experiment_root = std::string("experiment_data/") + std::string(argv[1]);

    std::string params_filename = experiment_root + std::string("/params");

    if (load_xdr_from_file(params, params_filename.c_str())) {
        throw std::runtime_error("failed to load params file");
    }

    std::printf("num accounts: %d\n", params.num_accounts);

    AccountIDList account_id_list;

    std::string accounts_filename = experiment_root + std::string("/accounts");
    if (load_xdr_from_file(account_id_list, accounts_filename.c_str())) {
        throw std::runtime_error("failed to load accounts list " + accounts_filename);
    }

    std::vector<PublicKey> pks;
    pks.resize(account_id_list.size());

    for (size_t i = 0; i < 5; i++) {
        auto [_, pk] = key_gen.deterministic_key_gen(account_id_list[i]);
        pks[i] = pk;
        std::cout << pk.data() << std::endl;
    }

    ExperimentBlock block;

    std::string block_filename = experiment_root + std::string("/") + std::string(argv[2]) + std::string(".txs");

    if (load_xdr_from_file(block, block_filename.c_str())) {
        throw std::runtime_error("failed to load tx block");
    }

    SignedTransactionList tx_list;

    tx_list.insert(tx_list.end(), block.begin(), block.end());

    SerializedBlock serialized_block = xdr::xdr_to_opaque(tx_list);

    size_t num_threads = std::stoi(argv[3]);

    auto timestamp = init_time_measurement();


    /*
    poll_node(2);
    */

    float res = measure_time(timestamp);

    std::cout << res << std::endl;

    std::printf("checked %lu sigs in %lf with max %lu threads\n", tx_list.size(), res, num_threads);
    return 0;

}
