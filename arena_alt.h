#ifndef _ARENA_H_INCLUDE_
#define _ARENA_H_INCLUDE_

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdalign.h>

typedef struct Arena Arena;

struct Arena {
	void*  data;
	size_t offset;
	size_t capacity;
	void*  last_allocation;
};

#define arena_make(A, Type, Count) \
	((Type *)arena_alloc((A), sizeof(Type) * (Count), alignof(Type)))

Arena arena_create(uint8_t* buf, size_t buf_size);

void* arena_alloc(Arena* arena, size_t size, size_t align);

bool arena_resize(Arena* arena, void* ptr, size_t size);

void arena_reset(Arena* arena);

#ifdef ARENA_IMPLEMENTATION
#include <stdio.h>

#ifndef ARENA_ASSERT
	#define ensure(Pred, Msg) do { \
		if(!(Pred)){ \
			fprintf(stderr, "(%s:%d) Assertion failure: %s\n", __FILE__, __LINE__, (Msg));\
			__builtin_trap(); \
		} \
	} while(0)
#else
	#define ensure(Pred, Msg) ARENA_ASSERT(Pred, Msg)
#endif


static inline
bool arena_valid_alignment(size_t align){
	return align && ((align & (align - 1)) == 0);
}

static inline
uintptr_t arena_align_forward_ptr(uintptr_t p, uintptr_t a){
	ensure(arena_valid_alignment(a), "Alignment must be a power of 2 greater than 0");
	uintptr_t mod = p & (a - 1); /* Fast modulo for powers of 2 */
	if(mod > 0){
		p += (a - mod);
	}
	return p;
}

Arena arena_create(uint8_t* buf, size_t buf_size){
	return (Arena){
		.data = buf,
		.offset = 0,
		.capacity = buf_size,
		.last_allocation = NULL,
	};
}

void* arena_alloc(Arena* a, size_t size, size_t align){
	uintptr_t base = (uintptr_t)a->data;
	uintptr_t current = base + (uintptr_t)a->offset;

	size_t available = a->capacity - (current - base);

	uintptr_t aligned  = arena_align_forward_ptr(current, align);
	uintptr_t padding  = aligned - current;
	uintptr_t required = padding + size;

	if(required > available){
		return NULL; /* Out of memory */
	}

	a->offset += required;
	void* allocation = (void*)aligned;
	a->last_allocation = allocation;
	__builtin_memset(allocation, 0, size);

	return allocation;
}

bool arena_resize(Arena* a, void* ptr, size_t new_size){
	uintptr_t base    = (uintptr_t)a->data;
	uintptr_t current = base + (uintptr_t)a->offset;
	uintptr_t limit   = base + a->capacity;

	ensure((uintptr_t)ptr >= base && (uintptr_t)ptr < limit, "Pointer is not owned by arena");

	if(ptr == a->last_allocation){
		size_t last_allocation_size = current - (uintptr_t)a->last_allocation;
		if(((current - last_allocation_size) + new_size) > limit){
			return false; /* No space left */
		}

		a->offset += new_size - last_allocation_size;
		return true;
	}

	return false;
}

void arena_reset(Arena* arena){
	arena->offset = 0;
	arena->last_allocation = NULL;
}

#undef ensure
#endif /* Implementation */

#endif /* Header guard */
