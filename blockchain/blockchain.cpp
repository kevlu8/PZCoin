#include "blockchain.hpp"

BlockChain::BlockChain() {
	new_block("0");
	first = latest;
}

int BlockChain::new_transaction(char *a, char *b, uint64_t amount) {
	json_transaction transaction;
	transaction.sender = a;
	transaction.receiver = b;
	transaction.amount = amount;
	curr_transactions.push_back(transaction);
	return get_last_block().index + 1;
}

void BlockChain::new_block(char *hash, char *prev_hash = NULL) {
	Block block;
	block.next = NULL;
	block.prev = latest;
	latest->next = &block;
	block.index = latest->index + 1;
	latest = &block;
	block.hash = hash;
	block.prev_hash = prev_hash;
	block.timestamp = time(NULL);
	block.transactions = curr_transactions; // wait for transactions to be added
}

Block BlockChain::get_last_block() { return *latest; }
