#include "memory.hpp"

void* mem_set(void* p, u8 val, isize count){
	return __builtin_memset(p, val, count);
}

void* mem_copy(void* dest, void const* source, isize count){
	return __builtin_memmove(dest, source, count);
}

void* mem_copy_no_overlap(void* dest, void const* source, isize count){
	return __builtin_memcpy(dest, source, count);
}

i32 mem_compare(void* pa, void* pb, isize count){
	return __builtin_memcmp(pa, pb, count);
}


