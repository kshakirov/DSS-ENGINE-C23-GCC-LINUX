#pragma once
#include <stdint.h>
#include <stddef.h>

int32_t put_file(const char* filename, const char* content, const size_t size);
const char* get_file(const char* filename);
