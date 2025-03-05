#pragma once

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdarg.h>
#include <stdalign.h>
#include <limits.h>
#include <atomic>

using i8 = int8_t;
using i16 = int16_t;
using i32 = int32_t;
using i64 = int64_t;

using u8 = uint8_t;
using u16 = uint16_t;
using u32 = uint32_t;
using u64 = uint64_t;

typedef ptrdiff_t isize;
typedef size_t usize;
typedef uintptr_t uintptr;

typedef u8  byte;
typedef u32 rune;

typedef float  f32;
typedef double f64;

template<typename T>
T abs(T x){
	return x < T{0} ? -x : x;
}

template<typename T>
T min(T a, T b){
	return a < b ? a : b;
}

template<typename T>
T max(T a, T b){
	return a > b ? a : b;
}

template<typename T>
T clamp(T lo, T x, T hi){
	return min(max(lo, x), hi);
}

#define ContainerOf(Ptr, Type, Member) \
	((Type *)(((void *)(Ptr)) - offsetof(Type, Member)))

// TODO: Change this to 202311L in the future, many versions of gcc for whatever
// reason haven't updated the number
#if (__STDC_VERSION__ < 202000L)
#define typeof(X) __typeof__(X)
#endif

static_assert(sizeof(f32) == 4 && sizeof(f64) == 8, "Bad float size");
static_assert(sizeof(void(*)(void)) == sizeof(void*), "Function pointers and data pointers must be of the same width");
static_assert(sizeof(void(*)(void)) == sizeof(uintptr), "Mismatched pointer types");
static_assert(sizeof(isize) == sizeof(usize), "Mismatched size");
static_assert(CHAR_BIT == 8, "Invalid char size");

static inline
void bounds_check(bool predicate){
	if(!predicate){
		__builtin_trap();
	}
}

struct String {
	u8 const* data;
	isize     length;

	String() : data{nullptr}, length{0} {}
	String(char const* cstr){
		data = (u8 const*)cstr;
		for(length = 0; cstr[length] != 0; length++){}
	}
};

static inline
isize len(String s){
	return s.length;
}

