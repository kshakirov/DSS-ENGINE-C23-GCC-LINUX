#include "file_table.h"
#include "../../debug.h"

typedef uint32_t FileIndex ;

FileIndex put_file(const char* content, const char* filename){
  DEBUG_LOG("Saving the file");
  return 1;
}
const char* get_file(const char* filename){
  return "just for test";
}
