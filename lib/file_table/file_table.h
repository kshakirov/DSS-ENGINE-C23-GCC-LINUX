#pragma once
#include <stdint.h>
#include <stdlib.h>
#include <stddef.h>
#include "../storage/storage.h"

typedef  struct {
  bool found;
  size_t content_size;
  size_t block_count;
  const HashIndex* block_indices;
  
}FILE_VIEW;

int32_t put_file(const char* filename,  const size_t size, HashIndex* block_indices,size_t block_count);
FILE_VIEW get_file(const char* filename);
