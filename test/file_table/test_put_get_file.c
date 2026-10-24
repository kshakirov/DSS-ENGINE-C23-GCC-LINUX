#include "../../lib/file_table/file_table.h"
#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <string.h>


int main(void){
  printf("Testing file table ...\n");
  const char* filename = "file to save";
  const char* content = "the content of the file is almost empty";
  auto view = get_file(filename);
  
  printf("%s\n","found");
  assert(view.found ==true);
  HashIndex* block_indices = malloc(sizeof *block_indices);
  assert(block_indices != nullptr);
  block_indices[0] = 1;
  assert(view.found);
  assert(view.content_size == strlen(content));
  assert(view.block_count == 2);
  assert(view.block_indices[0] == 2);
  assert(view.block_indices[1] == 3);
  auto idx  = put_file(filename, strlen(content), block_indices, 1);
  printf("%u\n",idx);
  assert(idx >=0);

  HashIndex* updated_block_indices = malloc(2 * sizeof *updated_block_indices);
  assert(updated_block_indices != nullptr);
  updated_block_indices[0] = 2;
  updated_block_indices[1] = 3;

  auto updated_idx = put_file(filename, strlen(content), updated_block_indices, 2);
  assert(updated_idx == idx);
}
