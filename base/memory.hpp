#pragma once
#include "base.hpp"

constexpr i64 KiB = 1024ll;
constexpr i64 MiB = 1024ll * KiB;
constexpr i64 GiB = 1024ll * MiB;
constexpr i64 TiB = 1024ll * GiB;
constexpr i64 PiB = 1024ll * TiB;
constexpr i64 EiB = 1024ll * PiB;

void* mem_set(void* p, u8 val, isize count);

void* mem_copy(void* dest, void const* source, isize count);

void* mem_copy_no_overlap(void* dest, void const* source, isize count);

i32 mem_compare(void* pa, void* pb, isize count);

enum struct AllocatorMode : u8 {
	Query         = 0,
	Alloc         = 1,
	Resize        = 2,
	Free          = 3,
	FreeAll       = 4,
	AllocNonZero  = 5,
	ResizeNonZero = 6,
};

// Bit set with the supported AllocatorMode's
enum struct AllocatorCapabilities : u8 {
	Query         = 1 << i32(AllocatorMode::Query),
	Alloc         = 1 << i32(AllocatorMode::Alloc),
	Resize        = 1 << i32(AllocatorMode::Resize),
	Free          = 1 << i32(AllocatorMode::Free),
	FreeAll       = 1 << i32(AllocatorMode::FreeAll),
	AllocNonZero  = 1 << i32(AllocatorMode::AllocNonZero),
	ResizeNonZero = 1 << i32(AllocatorMode::ResizeNonZero),
};

enum struct AllocatorError : u8 {
	None = 0,
	OutOfMemory,
	InvalidAlignment,
	BadArgument,
	UnsupportedMode,
};

using AllocatorFunc = Result<void*, AllocatorError>(*)(
	void* impl,
	AllocatorMode mode,
	isize size,
	isize align,
	void* old_ptr,
	isize old_size,
	AllocatorCapabilities* capability
);

struct Allocator {
	void* data;
	AllocatorFunc func;
};

[[nodiscard]]
Result<void*, AllocatorError> mem_alloc(Allocator allocator, isize count, isize align);

[[nodiscard]]
Result<void*, AllocatorError> mem_resize(Allocator allocator, isize new_size);

AllocatorCapabilities mem_query(Allocator allocator);

void mem_free(Allocator allocator, void* ptr, isize old_size);

void mem_free_all(Allocator allocator);
