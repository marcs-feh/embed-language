#include "base/base.hpp"
#include "base/arena.hpp"

#include <iostream>
template<typename T>
void print(T v){
	std::cout << v << '\n';
}
template<typename T, typename... Rest>
void print(T v, Rest&&... rest){
	std::cout << v << ' ';
	print(rest...);
}

int main(){
	static byte memory[8 * MiB];
	Arena arena = arena_create(Slice<byte>(memory, 8*MiB));
	defer(arena_reset(&arena));
	{
		auto restore = arena_region_begin(&arena);
		defer(arena_region_end(restore));

		void * p0 = arena_alloc(&arena, 11, 8);
		print(arena.offset);
		void * p1 = arena_alloc(&arena, 13, 4);
		print(arena.offset);
	}
	print(arena.offset);

	
	return 0;
}
