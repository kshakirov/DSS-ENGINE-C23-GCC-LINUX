#include "file_table.h"
#include "../../debug.h"


#define CAPACITY 10000
typedef uint32_t FileIndex ;

typedef uint32_t FileNameHash ;

typedef  struct {
  const char* filename;
  const size_t size;
  const FileIndex descr;
  const FileNameHash fileNameHash;
  //dont' now how make Block address available her
}FileMetadata;

static FileNameHash fileNameHashTable[CAPACITY];

static FileMetadata fileMetadataTable[CAPACITY];

FileIndex put_file(const char* content, const char* filename){
  DEBUG_LOG("Saving the file");
  return 1;
}
const char* get_file(const char* filename){
  return "just for test";
}
