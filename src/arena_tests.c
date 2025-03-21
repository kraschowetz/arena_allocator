#include "arena.h"
#include <stdio.h>

void alloc_string_test(void) {
	set_arena_error_handling_method(ARENA_THROW_ERR);
	Arena arena = create_arena(32);
	
	size_t size = sizeof("hello, world!\n");
	char *ptr = arena_alloc(&arena, size);
	arena_copy(&arena, ptr, "hello, world!\n", size);
	
	printf("%s\n", ptr);

	dbg_blob(&arena, ptr);
	dbg_arena(&arena);

	delete_arena(&arena);
}

int main(void) {	
	alloc_string_test();
	return 0;
}
