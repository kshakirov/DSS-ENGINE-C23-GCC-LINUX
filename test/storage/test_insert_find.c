#include <stdio.h>
#include <assert.h>
#include <string.h>
#include "../../debug.h"
#include "../../lib/storage/storage.h"
int main(void){
  auto content = "tell me why you lied to me";
  long size = strlen(content);
  printf("the size of content is %ld\n", size);

  auto filename = "file.txt";
  printf("Testing insert and find hashed\n");
  auto idx = process_file(filename, content, size );
  printf("Getting index from storage %d \n", idx);
  auto another_idx = process_file(filename, content, size);

  printf("Getting another index from storage %d \n", another_idx);
  assert(another_idx == idx);
  printf("All asserts are good, bro\n");
}
