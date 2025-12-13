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

#ifndef _STD_ALLOC_H
#define _STD_ALLOC_H

#include <std/types.h>

struct std_allocator {
    void *(*alloc)(struct std_allocator*, usize);
    void *(*alloc_align)(struct std_allocator*, usize, usize);
    usize (*get_alloc_size)(const struct std_allocator*, const void*);
    void (*free)(struct std_allocator*, void*);
    void *(*resize)(struct std_allocator*, void*, usize);
};

extern struct std_allocator std_heap;

static inline void *std_heap_alloc(usize size) {
    return std_heap.alloc(&std_heap, size);
}

static inline void *std_heap_alloc_align(usize size, usize align) {
    return std_heap.alloc_align(&std_heap, size, align);
}

static inline usize std_heap_get_alloc_size(const void *p) {
    return std_heap.get_alloc_size(&std_heap, p);
}

static inline void std_heap_free(void *p) {
    std_heap.free(&std_heap, p);
}

static inline void *std_heap_resize(void *p, usize size) {
    return std_heap.resize(&std_heap, p, size);
}

#endif /* _STD_ALLOC_H */
