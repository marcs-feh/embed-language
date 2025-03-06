#include "arena.hpp"

void* arena_alloc(Arena* a, isize nbytes, isize align){}

void arena_reset(Arena* a){}

bool arena_resize(Arena* a, void* ptr, isize nbytes){}

ArenaRegion arena_region_begin(Arena* a){}

void arena_region_end(ArenaRegion reg){}

