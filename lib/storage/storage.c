#include <stddef.h>
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <xxhash.h>
typedef uint64_t BlockHash;
typedef void *BlockAddress;
typedef int32_t HashIndex;
static size_t stored_blocks;

#define  CAPACITY 10000
//about N it is fo the time being

static BlockHash blockHashTable[CAPACITY];
static BlockAddress blockAddressTable[CAPACITY];


//those functions will work within process_file 
static HashIndex find_index_hash_table (BlockHash hash){

  for(int i = 0; i< stored_blocks;i++){
    if(blockHashTable[i] == hash){
      return i;
    }
  }
  return  -1;
}
static HashIndex insert_index_hash_table( BlockHash hash){
  if(stored_blocks < CAPACITY){
    blockHashTable[stored_blocks] = hash;
    stored_blocks += 1;
    return stored_blocks -1 ;
  }
}
static bool sync_index_block_table(HashIndex, BlockAddress);





HashIndex process_file(const char* filename, const char* content){
  printf("Storage: I am processing the file %s with content %s \n", filename, content);
  BlockHash hash = XXH64(content, sizeof(content),0);
  auto idx =   find_index_hash_table(hash);
  if(idx > -1){
  }else {
    if(stored_blocks < CAPACITY){
      idx = insert_index_hash_table(hash);
    }else{
      // for the future malloc or realloc or whatever
    }
  }
  printf("Storage: Saving the file...\n");
  return idx; //for test only
}

