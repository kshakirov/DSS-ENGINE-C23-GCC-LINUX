#include "../../lib/file_table/file_table.h"
#include <stdio.h>
#include <assert.h>
#include <string.h>


int main(void){
  printf("Testing file table ...\n");
  const char* filename = "file to save";
  const char* content = "the content of the file is almost empty";
  auto found = get_file(filename);
  printf("%s\n",found);
  assert(strcmp(found, "not found")==0);
  auto idx  = put_file(filename, content, strlen(content));
  printf("%u\n",idx);
  assert(idx >=0);
}
