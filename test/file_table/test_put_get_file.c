#include "../../lib/file_table/file_table.h"
#include <stdio.h>
#include <assert.h>
#include <string.h>


int main(void){
  printf("Testing file table ...\n");
  const char* filename = "file to save";
  const char* content = "the content of the file is almost empty";
  auto found = get_file(filename);
  assert(strcmp(found, "not found"));
  auto idx  = put_file(filename, content);
  assert(idx > -1);
}
