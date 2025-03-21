#ifndef ARENA_H
#define ARENA_H

#include <stdlib.h>
#include <stdbool.h>
#include <inttypes.h>

#define STD_BLOB_SIZE 8

typedef enum ArenaErrorHandle_t {
	ARENA_NO_ERR = 0,	// stops function execution, no aditional info provided
	ARENA_THROW_WARNING,	// stops function execution, prints error cause
	ARENA_THROW_ERR		// stops program execution, prints error cause
} ArenaErrorHandle;

extern ArenaErrorHandle error_handle_method; 

#define HANDLE_ERR(_err) \
	if(error_handle_method != ARENA_NO_ERR) { \
		fprintf(stderr, _err); \
		if(error_handle_method == ARENA_THROW_ERR) { \
			exit(1); \
		} \
	} 

#define HANDLE_ERRF(_err, ...) \
	if(error_handle_method != ARENA_NO_ERR) { \
		fprintf(stderr, _err, __VA_ARGS__); \
		if(error_handle_method == ARENA_THROW_ERR) { \
			exit(1); \
		} \
	} 

struct Blob_t;
typedef struct Blob_t {
	size_t size;
	void *mem;
	struct Blob_t *next;
	bool used : 1;
} Blob;

typedef struct Arena_t {
	size_t size;
	Blob *ll_root;
	uint8_t *mem;
} Arena;

/*
 * creates an arena with size n;
 *
 * @param 1: arena size
 * @return: Arena data structure
 */
Arena create_arena(size_t size);

/*
 * deletes an arena and free all of its memory;
 *
 * @param 1: ptr to the arena to delete
 */
void delete_arena(Arena *self);

/*
 * allocates n bytes in the arena;
 *
 * @param 1: ptr to the arena
 * @param 2: number of bytes to allocate
 * @return: ptr to the allocated memory or NULL if failed to allocate
 */
void *arena_alloc(Arena *self, size_t size);

/*
 * free a ptr in the arena;
 *
 * @param 1: ptr to the arena
 * @param 2: ptr to be freed
 */
void arena_free(Arena *restrict self, void *ptr);

/*
 * copy n bytes to a ptr in the arena;
 * checks if the ptr is a valid ptr inside the arena;
 *
 * @param 1: ptr to the arena
 * @param 2: destination ptr inside the arena
 * @param 3: source ptr to be copied
 * @param 4: number of bytes to copy
*/
void arena_copy(Arena *restrict self, void *restrict dest, void *src, size_t n);

/*
 * get the Blob that contains the data to a ptr inside the arena;
 * 
 * @param 1: ptr to the arena
 * @param 2: ptr allocated in the arena
 * @return: blob ptr with the data from the ptr passed as @param 2
 */
Blob *get_blob(Arena *restrict self, void *ptr);

/*
 * prints state of arena;
 *
 * @param 1: ptr to the arena
 */
void dbg_arena(Arena *self);

/*
 * prints state of a blob allocated in the arena;
 *
 * @param 1: ptr to the arena
 * @param 2: ptr allocated in the arena
 */ 
void dbg_blob(Arena *self, void *blob);

/*
 * sets how the arena will handle errors;
 *
 * @param 1: ArenaErrorHandle representing how errors will be handled
 */
void set_arena_error_handling_method(ArenaErrorHandle method);

#endif
