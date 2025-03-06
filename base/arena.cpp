#include "arena.hpp"

static inline
bool arena_valid_alignment(isize align){
	return (align > 0) && ((align & (align - 1)) == 0);
}

static inline
uintptr arena_align_forward_ptr(uintptr p, uintptr a){
	ensure(arena_valid_alignment(a), "Alignment must be a power of 2 greater than 0");
	uintptr mod = p & (a - 1); /* Fast modulo for powers of 2 */
	if(mod > 0){
		p += (a - mod);
	}
	return p;
}

void* arena_alloc(Arena* a, isize size, isize align){
	uintptr base = (uintptr)a->data;
	uintptr current = base + (uintptr)a->offset;

	size_t available = a->capacity - (current - base);

	uintptr aligned  = arena_align_forward_ptr(current, align);
	uintptr padding  = aligned - current;
	uintptr required = padding + size;

	if(required > available){
		return NULL; /* Out of memory */
	}

	a->offset += required;
	void* allocation = (void*)aligned;
	a->last_allocation = allocation;
	mem_set(allocation, 0, size);

	return allocation;
}

Arena arena_create(Slice<u8> buf){
	Arena a;
	a.capacity = len(buf);
	a.data = buf.data;
	a.offset = 0;
	a.region_count = 0;
	a.last_allocation = nullptr;
	return a;
}

bool arena_resize(Arena* a, void* ptr, isize new_size){
	uintptr base    = (uintptr)a->data;
	uintptr current = base + (uintptr)a->offset;
	uintptr limit   = base + a->capacity;

	ensure((uintptr)ptr >= base && (uintptr)ptr < limit, "Pointer is not owned by arena");

	if(ptr == a->last_allocation){
		usize last_allocation_size = current - (uintptr)a->last_allocation;
		if(((current - last_allocation_size) + new_size) > limit){
			return false; /* No space left */
		}

		a->offset += new_size - last_allocation_size;
		return true;
	}

	return false;
}

void arena_reset(Arena* a){
	ensure(a->region_count == 0, "Dangling regions are still present in arena");
	a->offset = 0;
}

ArenaRegion arena_region_begin(Arena* a){
	ArenaRegion reg = {a, a->offset};
	a->region_count += 1;
	return reg;
}

void arena_region_end(ArenaRegion reg){
	reg.arena->offset = reg.offset;
	reg.arena->region_count -= 1;
}

