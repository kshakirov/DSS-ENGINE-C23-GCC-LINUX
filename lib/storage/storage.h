#pragma once
#include <stdint.h>
#include <stddef.h>

typedef int32_t HashIndex;

HashIndex process_file(const char*, const char*, size_t size);

void const *get_block(HashIndex idx);

