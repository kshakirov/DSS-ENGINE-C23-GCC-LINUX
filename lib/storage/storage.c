#include <stddef.h>
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
typedef uint64_t BlockHash;
typedef void *BlockAddress;
typedef int32_t HashIndex;
static size_t stored_blocks;

#define  CAPACITY 10000
//about N it is fo the time being

static BlockHash blockHashTable[CAPACITY];
static BlockAddress blockAddressTable[CAPACITY];


//those functions will work within process_file 
static HashIndex find_index_hash_table (BlockHash);
static HashIndex insert_index_hash_table( BlockHash );
static bool sync_index_block_table(HashIndex, BlockAddress);


void process_file(const char* filename, const char* content){
  printf("Storage: I am processing the file %s with content %s \n", filename, content);
  printf("Storage: Saving the file...");
}


