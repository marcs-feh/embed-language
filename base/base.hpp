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

template<typename A, typename B = A>
struct Pair {
	A a;
	B b;
};

namespace impl_defer {
	template<typename F>
	struct Deferred {
		F f;
		explicit Deferred(F&& f) : f(static_cast<F&&>(f)){}
		~Deferred(){ f(); }
	};
	template<typename F>
	auto make_deferred(F&& f){
		return Deferred<F>(static_cast<F&&>(f));
	}

	#define _impl_defer_concat0(X, Y) X##Y
	#define _impl_defer_concat1(X, Y) _impl_defer_concat0(X, Y)
	#define _impl_defer_concat_counter(X) _impl_defer_concat1(X, __COUNTER__)
	#define defer(Stmt) auto _impl_defer_concat_counter(_defer_) = ::impl_defer::make_deferred([&](){ do { Stmt ; } while(0); return; })
}

// #define ContainerOf(Ptr, Type, Member) \
// 	((Type *)(((void *)(Ptr)) - offsetof(Type, Member)))

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

#ifndef NO_STDIO
extern "C" { int printf(const char* restrict, ...); }
#endif

struct String {
	u8 const* data;
	isize     length;

	byte operator[](isize idx);
	String operator[](Pair<isize> range);

	String() : data{nullptr}, length{0} {}
	String(u8 const* data, isize n) : data{data}, length{n}{}
	String(char const* cstr){
		data = (u8 const*)cstr;
		for(length = 0; cstr[length] != 0; length++){}
	}
};

static inline
isize len(String s){
	return s.length;
}


#define string_printf_fmt(S) ((int)len((S))), ((char const*)((S).data))

static inline
void bounds_check(bool predicate, String msg){
	#ifndef DISABLE_BOUNDS_CHECK
	[[unlikely]] if(!predicate){
		#ifndef NO_STDIO
		printf("Bounds check failure: %.*s\n", string_printf_fmt(msg));
		#endif
		__builtin_trap();
	}
	#endif
}

inline
byte String::operator[](isize idx){
	bounds_check(idx >= 0 && idx <= length, "Out of bounds access to string");
	return data[idx];
}

inline
String String::operator[](Pair<isize> range){
	bounds_check(range.a >= range.b && range.a < length && range.b <= length && range.a >= 0 && range.b >= 0, "Invalid range to substring");
	String res = { &data[range.a], range.b - range.a };
	return res;
}

static inline
void ensure(bool predicate, String msg){
	#ifndef DISABLE_ASSERT
	[[unlikely]] if(!predicate){
		#ifndef NO_STDIO
		printf("Assertion failure: %.*s\n", string_printf_fmt(msg));
		#endif
		__builtin_trap();
	}
	#endif
}

[[noreturn]] static inline
void panic(String msg){
	#ifndef NO_STDIO
	printf("Assertion failure: %.*s\n", string_printf_fmt(msg));
	#endif
	__builtin_trap();
}

template<typename T>
struct Slice {
	T*    data;
	isize length;

	T& operator[](isize idx){
		bounds_check(idx >= 0 && idx <= length, "Out of bounds access to slice");
		return data[idx];
	}

	T const & operator[](isize idx) const {
		bounds_check(idx >= 0 && idx <= length, "Out of bounds access to slice");
		return data[idx];
	}

	Slice<T> operator[](Pair<isize> range) const {
		bounds_check(range.a >= range.b && range.a < length && range.b <= length && range.a >= 0 && range.b >= 0, "Invalid range to sub-slice");
	}

	Slice() : data{nullptr}, length{0}{}
	Slice(T* ptr, isize n) : data{ptr}, length{n} {}
};

template<typename T>
isize len(Slice<T> s){
	return s.length;
}

