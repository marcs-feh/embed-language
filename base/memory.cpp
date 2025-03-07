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

Result<void*, AllocatorError> mem_alloc(Allocator allocator, isize count, isize align){
	return allocator.func(allocator.data, AllocatorMode::Alloc, count, align, nullptr, 0, nullptr);
}

Result<void*, AllocatorError> mem_resize(Allocator allocator, void* ptr, isize old_size, isize new_size){
	return allocator.func(allocator.data, AllocatorMode::Resize, new_size, 0, ptr, old_size, nullptr);
}

AllocatorCapabilities mem_query(Allocator allocator){
	AllocatorCapabilities cap{0};
	allocator.func(allocator.data, AllocatorMode::Query, 0, 0, nullptr, 0, &cap);
	return cap;
}

void mem_free(Allocator allocator, void* ptr, isize old_size){
	allocator.func(allocator.data, AllocatorMode::Free, 0, 0, ptr, old_size, nullptr);
}

void mem_free_all(Allocator allocator){
	allocator.func(allocator.data, AllocatorMode::FreeAll, 0, 0, nullptr, 0, nullptr);
}

Result<void*, AllocatorError> mem_alloc_non_zero(Allocator allocator, isize count, isize align){
	return allocator.func(allocator.data, AllocatorMode::AllocNonZero, count, align, nullptr, 0, nullptr);
}

Result<void*, AllocatorError> mem_resize_non_zero(Allocator allocator, void* ptr, isize old_size, isize new_size){
	return allocator.func(allocator.data, AllocatorMode::ResizeNonZero, new_size, 0, ptr, old_size, nullptr);
}
