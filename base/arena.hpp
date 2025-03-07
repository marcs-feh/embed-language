#pragma once

#include "base.hpp"
#include "memory.hpp"

struct Arena {
	byte* data;
	void* last_allocation;
	isize capacity;
	isize offset;
	isize region_count;
};

// Arena temporary region
struct ArenaRegion {
	Arena* arena;
	isize  offset;
};

Arena arena_create(Slice<u8> buf);

void* arena_alloc(Arena* a, isize nbytes, isize align);

void arena_reset(Arena* a);

bool arena_resize(Arena* a, void* ptr, isize nbytes);

[[nodiscard]]
ArenaRegion arena_region_begin(Arena* a);

void arena_region_end(ArenaRegion reg);

Allocator arena_allocator(Arena* a);

template<typename T> [[nodiscard]]
T* make(Allocator a){
	return (T*) mem_alloc(a, sizeof(T), alignof(T)).value;
}

template<typename T> [[nodiscard]]
Slice<T> make(isize n, Allocator a){
	ensure(n >= 0, "Cannot allocate negative length");
	auto res = mem_alloc(a, sizeof(T) * n, alignof(T));
	return Slice<T>((T*)res.value, ok(res) ? n : 0);
}

template<typename T>
void destroy(Slice<T> s, Allocator a){
	mem_free(a, s.data, len(s) * sizeof(T));
}

template<typename T>
void destroy(T* p, Allocator a){
	mem_free(a, p, sizeof(T));
}
