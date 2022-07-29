#include <include.hpp>

class Block {
  char *hash;
}

class BlockChain {
  std::vector<Block> blockchain;
  
  BlockChain();
  ~BlockChain();
  
  void new_block();
  
}
