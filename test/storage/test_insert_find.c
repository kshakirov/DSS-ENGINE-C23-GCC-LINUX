#include <stdio.h>
#include <assert.h>
#include "../../lib/storage/storage.h"
int main(int argc, char** argv){
  auto content = "tell me why you lied to me";
  auto filename = "file.txt";
  printf("Testing insert and find hashed\n");
  auto idx = process_file(filename, content);
  printf("Getting index from storage %d \n", idx);
  auto another_idx = process_file(filename, content);

  printf("Getting another index from storage %d \n", idx);
  assert(another_idx == idx);
}
