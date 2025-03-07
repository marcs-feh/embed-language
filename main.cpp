#include "base/base.hpp"
#include "base/arena.hpp"

int main(){
	static byte memory[8 * MiB];
	Arena arena = arena_create(Slice<byte>(memory, 8*MiB));
	return 0;
}
