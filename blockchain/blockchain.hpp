#include "include.hpp"

typedef struct Block {
  char *hash, *prev_hash;
  uint64_t timestamp;
  json transactions;
};

class BlockChain {
  std::vector<Block> chain;
  
  BlockChain();
  
  /**
 * @brief Appends a new transaction to the chain
 * 
 * @param a Wallet address of the sender
 * @param n Wallet address of the receiver
 * @param amount Amount of coins to send
 * @return The index of the block containing the transaction
 */
  int new_transaction(char *a, char *b, uint64_t amount);

  /**
   * @brief Creates a new block and appends it to the chain
   * 
   * @param hash The hash of the block
   * @param prev_hash (Optional) The hash of the previous block
   * @return The block
   */
  Block new_block(char *hash, char *prev_hash=NULL);

  /**
   * @brief Returns the JSON data of the last block
   * 
   * @return The block
   */
  Block get_last_block();


};
