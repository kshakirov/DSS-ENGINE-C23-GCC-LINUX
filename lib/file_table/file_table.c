#include "file_table.h"
#include "../../debug.h"
#include <xxhash.h>
#include <string.h>
#define CAPACITY 10000
#define FILENAME_SIZE 256
typedef uint32_t FileIndex ;

typedef uint32_t FileNameHash ;

typedef  struct {
  char filename[FILENAME_SIZE];
  size_t size;
  FileIndex descr;
  FileNameHash fileNameHash;
  size_t block_count;
  //dont' now how make Block address available her
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


static void sync_file_metadata(FileIndex idx, FileNameHash  hash,  const char*filename, const size_t content_size){
  
  FileMetadata metadata;//for the time being only
  strcpy(metadata.filename, filename);
  metadata.fileNameHash = hash;
  metadata.size = content_size;
  fileMetadataTable[idx] = metadata;
}

FileIndex put_file(const char* content, const char* filename, const size_t content_size){
  DEBUG_LOG("Saving the file\n");
  size_t filename_len = strlen(filename);
  FileNameHash f_hash = XXH64(filename, filename_len,0);
  FileIndex idx = find_index_by_hash_file_table(f_hash);
  if(idx > -1){
    return idx;
  }else{
    fileNameHashTable[stored_files] = f_hash;
    sync_file_metadata(stored_files, f_hash, filename, content_size);
    stored_files += 1;
    return stored_files - 1;

  }

}


const char* get_file(const char* filename){
  DEBUG_LOG("Getting  the file\n");
  size_t filename_len = strlen(filename);
  FileNameHash f_hash = XXH64(filename, filename_len,0);
  for (size_t i = 0; i < stored_files;i++){
    if(fileNameHashTable[i] == f_hash){
      FileMetadata metadata = fileMetadataTable[i];
      DEBUG_LOG("Found file and its metadata, returning for the time being its stored name \n");
      return "found";
    }
  }
  return "not found"; //for the time being

}
