#pragma once
#include "base.hpp"

void* mem_set(void* p, u8 val, isize count);

void* mem_copy(void* dest, void const* source, isize count);

void* mem_copy_no_overlap(void* dest, void const* source, isize count);

i32 mem_compare(void* pa, void* pb, isize count);

struct Allocator {};

