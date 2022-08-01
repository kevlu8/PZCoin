#include "blockchain.hpp"

BlockChain::BlockChain() {
  chain.push_back(new_block("0"));
}

BlockChain::new_transaction(char *a, char *b, uint64_t amount) {
  json transaction = {
    {"sender", a},
    {"receiver", b},
    {"amount", amount}
  };
  chain.back().transactions.push_back(transaction);
  return chain.size() - 1;
}