#pragma once
#include <stdint.h>
#include <stddef.h>

typedef int32_t HashIndex;
HashIndex process_file(const char*, const char*, size_t size);

