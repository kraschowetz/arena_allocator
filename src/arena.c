#include "arena.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

ArenaErrorHandle error_handle_method = ARENA_NO_ERR;

Blob *get_blob(Arena *restrict self, void *ptr) {
	Blob *b = self->ll_root;
	while(b) {
		if(b->mem == ptr) {
			return b;
		}
		b = b->next;
	}
	return NULL;
}

Arena create_arena(size_t size) {
	if(size == 0) {
		HANDLE_ERR("cant create arena with size 0!\n");
		return (Arena) {
			.size = 0,
			.mem = NULL,
			.ll_root = NULL
		};
	}

	Arena self = (Arena) {
		.size = size,
		.mem = (u8*) malloc(size),
	};

	self.ll_root = (Blob*) malloc(sizeof(Blob));
	*self.ll_root = (Blob) {
		.size = size > STD_BLOB_SIZE ? STD_BLOB_SIZE : size,
		.mem = self.mem,
		.used = 0
	};
	size -= self.ll_root->size;
	
	u64 last_byte = self.ll_root->size;
	Blob *last_blob = self.ll_root;
	u64 nblobs = size % 8 == 0 ? size / 8 : size / 8 + 1;
	
	for(u64 i = 0; i < nblobs; i++) {
		Blob *b = (Blob*) malloc(sizeof(Blob));
		*b = (Blob) {
			.size = size - STD_BLOB_SIZE < size ? STD_BLOB_SIZE : size, // check for unsigned overflow
			.mem = &self.mem[last_byte],
			.next = NULL,
			.used = 0
		};
		last_blob->next = b;
		last_blob = b;
		size -= b->size;
		last_byte += b->size;
	}

	return self;
}

void *arena_alloc(Arena *self, size_t size) {
	Blob *blob = self->ll_root;
	size_t last_byte = 0;
	while(blob->used && blob->next) {
		last_byte += blob->size;
		blob = blob->next;
	}
	if(!blob->next && blob->used) {
		HANDLE_ERR("arena full!\n");
		return NULL;
	}


	if(size > blob->size) {
		u64 free_size = 0;
		Blob *of_blob = blob;
		u32 free_list_size = 0;

		while(of_blob) {
			free_size += of_blob->size;
			
			if(of_blob->used) {
				free_size = 0;
				free_list_size = 0;
				of_blob = of_blob->next;
				blob = of_blob;
				continue;
			}
			
			if(free_size >= size) {
				break;
			}
			of_blob = of_blob->next;
			free_list_size++;

		}
		if(free_size < size) {
			HANDLE_ERRF("failed to alloc %zu bytes!\n", size);
			return NULL;
		}

		blob->size = free_size;

		Blob *free_blob = blob->next;
		for(u32 i = 0; i < free_list_size; i++) {
			Blob * tmp = free_blob->next;
			free(free_blob);
			free_blob = tmp;
		}
		blob->next = free_blob;

	}

	if(blob->size > size) {
		size_t prev_size = blob->size;
		blob->size = size;
		Blob *new_blob = (Blob*) malloc(sizeof(Blob));
		*new_blob = (Blob) {
			.size = prev_size - size,
			.mem = &self->mem[last_byte + size],
			.next = blob->next,
			.used = 0
		};
		blob->next = new_blob;
	}

	blob->used = 1;
	return blob->mem;
}

void arena_free(Arena *self, void *ptr) {
	Blob *b = self->ll_root;
	while(b->mem != ptr) {
		b = b->next;
		if(b == NULL) {
			HANDLE_ERRF("blob with ptr %p not found!\n", ptr);
			return;
		}
	}

	b->used = 0;
}

void arena_copy(Arena *self, void *restrict dest, void *src, size_t n) {
	Blob *b = get_blob(self, dest);
	if(!b) {
		HANDLE_ERRF("blob with ptr %p not found!\n", dest);
		return;
	}
	if(n > b->size) {
		HANDLE_ERR("blob overflow!\n");
		return;
	}
	memcpy(dest, src, n);
}

void delete_arena(Arena *self) {
	free(self->mem);
	Blob *b = self->ll_root;
	while(b) {
		Blob *tmp = b->next;
		free(b);
		b = tmp;
	}
}

void dbg_arena(Arena* self) {
	Blob *b = self->ll_root;
	u32 i = 0;
	printf("state of arena @ %p:\n", self);
	while(b) {
		printf("blob %u:\n", i);
		printf("\tsize: %zu\n", b->size);
		printf("\tnext: %p\n", b->next);
		printf("\tused: %i\n", b->used);
		b = b->next;
		i++;
	}
}

void dbg_blob(Arena *self, void *blob) {
	Blob *b = get_blob(self, blob);
	if(b == NULL) {
		HANDLE_ERRF("blob with ptr %p not found!\n", blob);
	}
	
	printf("state of blob @ %p:\n", blob);
	printf("\traw bytes: ");
	for(u32 i= 0; i < b->size; i++) {
		printf("%x", ((unsigned char*)(b->mem))[i]);
	}
	printf("\n");
	printf("\tsize: %zu\n", b->size);
	printf("\tused: %i\n", b->used);
	printf("\tnext: %p\n", b->next);
}

void set_arena_error_handling_method(ArenaErrorHandle method) {
	error_handle_method = method;
}
