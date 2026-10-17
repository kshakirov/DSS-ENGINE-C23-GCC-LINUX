#include "file_table.h"
#include "../../debug.h"
#include <xxhash.h>
#include <string.h>
#define CAPACITY 10000
#define FILENAME_SIZE 256
typedef int32_t FileIndex ;

typedef uint64_t FileNameHash ;

typedef  struct {
  char filename[FILENAME_SIZE];
  size_t size;
  FileNameHash fileNameHash;
  size_t block_count;
  //dont' now how make Block address available her
  HashIndex* block_indices;
}FileMetadata;




static size_t stored_files;
static FileNameHash fileNameHashTable[CAPACITY];

static FileMetadata fileMetadataTable[CAPACITY];

static FileIndex find_index_by_hash_file_table (FileNameHash hash){
  for(uint32_t i = 0; i< stored_files ; i++){
    if(fileNameHashTable[i] == hash){
      return i;
    }
  }
  return  -1 ;
}


static void sync_file_metadata(FileIndex idx, FileNameHash  hash,  const char*filename, const size_t content_size, HashIndex* block_indices, size_t block_count){
  
  FileMetadata metadata;//for the time being only
  strcpy(metadata.filename, filename);
  metadata.fileNameHash = hash;
  metadata.size = content_size;
  metadata.block_indices = block_indices;
  metadata.block_count = block_count;
  
  fileMetadataTable[idx] = metadata;
}
static void free_metadata(FileIndex idx){
  FileMetadata metadata = fileMetadataTable[idx];
  free(metadata.block_indices);

}


FileIndex put_file(const char* filename, const size_t content_size, HashIndex* block_indices, size_t block_count){
  DEBUG_LOG("Saving the file\n");
  size_t filename_len = strlen(filename);
  FileNameHash f_hash = XXH64(filename, filename_len,0);
  FileIndex idx = find_index_by_hash_file_table(f_hash);
  if(idx > -1){
    free_metadata(idx);
    sync_file_metadata(idx, f_hash, filename, content_size, block_indices, block_count);
    return idx;
  }else{
    fileNameHashTable[stored_files] = f_hash;
    sync_file_metadata(stored_files, f_hash, filename, content_size, block_indices, block_count);
    stored_files += 1;
    return stored_files - 1;

  }

}


FILE_VIEW  get_file(const char* filename){
  DEBUG_LOG("Getting  the file\n");
  size_t filename_len = strlen(filename);
  FileNameHash f_hash = XXH64(filename, filename_len,0);
  FileIndex idx = find_index_by_hash_file_table(f_hash);
  FILE_VIEW fv;
  if(idx > -1){
    FileMetadata metadata = fileMetadataTable[idx];
    fv.found = true;
    fv.block_count = metadata.block_count;
    fv.block_indices = metadata.block_indices;
    fv.content_size = metadata.size;

    return fv;
  }else {
    fv.found = false;
    return fv;
  }
  

}
