#pragma once
#include <stdint.h>
#include <stddef.h>
#include "../storage/storage.h"

int32_t put_file(const char* filename, const char* content, const size_t size, HashIndex* block_indices);
const char* get_file(const char* filename);
