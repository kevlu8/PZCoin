#include "blockchain.hpp"

BlockChain::BlockChain() {
  chain.push_back(new_block("0")); // Create the genesis block
}

int BlockChain::new_transaction(char *a, char *b, uint64_t amount) {
  json_transaction transaction;
  transaction.sender = a;
  transaction.receiver = b;
  transaction.amount = amount;
  curr_transactions.push_back(transaction);
  return get_last_block().index + 1;
}

Block BlockChain::new_block(char *hash, char *prev_hash) {
  Block block;
  block.hash = hash;
  block.prev_hash = prev_hash;
  block.timestamp = time(NULL);
  return block;
}

Block BlockChain::get_last_block() {
  return chain.back();
}