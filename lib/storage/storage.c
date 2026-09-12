#include "storage.h"
#include <string.h>
#include <xxhash.h>
#include <stdlib.h>
#include <stdio.h>
#include "../../debug.h"

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

  for(size_t i = 0; i< stored_blocks;i++){
    if(blockHashTable[i] == hash){
      return i;
    }
  }
  return  -1;
}
static HashIndex insert_index_hash_table( BlockHash hash){

    blockHashTable[stored_blocks] = hash;
    stored_blocks += 1;
    return stored_blocks -1 ;

}
static void sync_index_block_table(HashIndex idx, BlockAddress address){
  //not checking the capacity must be synced with Hash tabel
 
  blockAddressTable[idx] = address;

}





HashIndex process_file(const char* filename, const char* content, size_t size){
  printf("Storage: I am processing the file %s with content %s \n", filename, content);
  BlockHash hash = XXH64(content, size,0);
  HashIndex idx =   find_index_hash_table(hash);
  if(idx > -1){
    DEBUG_LOG("I  found the index by  hash %lu\n", hash); 
  }else {
    if(stored_blocks < CAPACITY){
      idx = insert_index_hash_table(hash);
      DEBUG_LOG("Not found the index, creating  the index by  hash %lu\n", hash);
      BlockAddress address = malloc(size);
      if(address == nullptr){
	//do something the policy of treating errors 
	DEBUG_LOG("Can't allocate the memory ");
	exit(1);
      }
      memcpy(address, content, size);
      sync_index_block_table(idx, address);
      DEBUG_LOG("Synced  BlockAddress Table \n");
      
    }else{
      // TODO make ti grow and its parallel blocck address 
    }
  }
  DEBUG_LOG("Storage: Saving the file...\n");
  return idx; //for test only
}

