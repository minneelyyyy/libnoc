/* Copyright 2025 minneelyyyy <abigail@minneelyyyy.dev>
*
* Licensed under the Apache License, Version 2.0 (the "License");
* you may not use this file except in compliance with the License.
* You may obtain a copy of the License at
*
* http://www.apache.org/licenses/LICENSE-2.0
*
* Unless required by applicable law or agreed to in writing, software
* distributed under the License is distributed on an "AS IS" BASIS,
* WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
* See the License for the specific language governing permissions and
* limitations under the License.
**/

#include <std/alloc.h>
#include <std/defs.h>
#include <std/memory.h>

#include <c/sys/mman.h>
#include <syscalls/syscalls.h>

/* This lib's default "heap" allocator
 * before you say it, yes this is garbage. I can't be assed to write a good allocator currently.
 * just pretend that this is temporary proof of concept code and it will be fixed soon:tm:
 */

void *_std_os_alloc_align(struct std_allocator*, usize size, usize align);
usize _std_os_get_alloc_size(const struct std_allocator*, const void *p);
void _std_os_free(struct std_allocator*, void *p);

void *_std_generic_alloc(struct std_allocator*, usize size);
void *_std_generic_resize(struct std_allocator*, void *p, usize size);

struct std_allocator std_heap = {
    .alloc = _std_generic_alloc,
    .alloc_align = _std_os_alloc_align,
    .get_alloc_size = _std_os_get_alloc_size,
    .free = _std_os_free,
    .resize = _std_generic_resize,
};

struct memory_block {
    usize size;
    usize alignment;
    struct memory_block *_reserved; /* *(ptr - sizeof(intptr)) gets the addr of this memory block.
                                     * NEVER DIRECTLY ACCESS THIS MEMBER, THIS ONLY EXISTS TO ENSURE THE SPACE EXISTS FOR IT. */
    byte raw[];
};

static inline const struct memory_block* ptr_to_block(const void* p) {
    const struct memory_block* block = *(const struct memory_block**) (((uintptr) p) - sizeof(const struct memory_block*));
    return block;
}

static inline struct memory_block* mut_ptr_to_block(void* p) {
    struct memory_block* block = *(struct memory_block**) (((uintptr) p) - sizeof(struct memory_block*));
    return block;
}

static inline void *block_to_ptr(struct memory_block *block) {
    byte *raw = (byte*) (((uintptr) block) + offsetof(struct memory_block, raw));
    uintptr offset = (block->alignment - (usize) raw % block->alignment) % block->alignment;
    void *ptr = raw + offset;
    return ptr;
}

static inline bool is_align_valid(usize align) {
    return align != 0 && ((align & (align - 1)) == 0);
}

static inline bool mmap_failed(void *p) {
    return (intptr)p < 0 && (intptr)p >= (intptr)-4095;
}

void *_std_os_alloc_align(struct std_allocator *alloc, usize size, usize align) {
    (void) alloc;

    if (!is_align_valid(align))
        return NULL;

    size_t padding = (align - sizeof(struct memory_block) % align) % align;
    size_t nbytes = sizeof(struct memory_block) + padding + size;

    struct memory_block *block = sys_mmap(NULL, nbytes, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
    if (mmap_failed(block))
        return NULL;

    block->size = size;
    block->alignment = align;

    void *ptr = block_to_ptr(block);

    /* write reference, used later when getting the memory block from a user ptr */
    struct memory_block **ref = (struct memory_block**) ((uintptr) ptr - sizeof(struct memory_block*));
    *ref = block;

    return ptr;
}

usize _std_os_get_alloc_size(const struct std_allocator *alloc, const void *p) {
    (void) alloc;

    if (!p)
        return 0;

    const struct memory_block *block = ptr_to_block(p);

    return block->size;
}

void _std_os_free(struct std_allocator *alloc, void *p) {
    (void) alloc;

    if (!p)
        return;

    struct memory_block *block = mut_ptr_to_block(p);
    usize padding = ((block->alignment - sizeof(struct memory_block) % block->alignment) % block->alignment);

    sys_munmap(block, sizeof(struct memory_block) + padding + block->size);
}

void *_std_generic_alloc(struct std_allocator *alloc, usize size) {
    return alloc->alloc_align(alloc, size, 16);
}

void *_std_generic_resize(struct std_allocator *alloc, void *p, usize size) {
    if (!p)
        return NULL;

    void *new_ptr = alloc->alloc(alloc, size);
    if (!new_ptr)
        return NULL;

    std_memory_copy(new_ptr, p, alloc->get_alloc_size(alloc, p));

    alloc->free(alloc, p);

    return new_ptr;
}
