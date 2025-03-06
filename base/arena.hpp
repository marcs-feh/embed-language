#pragma once

#include "base.hpp"

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

Arena arena_create();

void* arena_alloc(Arena* a, isize nbytes, isize align);

void arena_reset(Arena* a);

bool arena_resize(Arena* a, void* ptr, isize nbytes);

ArenaRegion arena_region_begin(Arena* a);

void arena_region_end(ArenaRegion reg);

template<typename T>
T* make(Arena* a){
	return (T*) arena_alloc(a, sizeof(T), alignof(T));
}

template<typename T>
Slice<T> make(isize n, Arena* a){
	ensure(n >= 0, "Cannot allocate negative length");
	void* ptr = arena_alloc(a, sizeof(T) * n, alignof(T));
	return Slice<T>((T*)ptr, n);
}
