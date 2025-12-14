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

#ifndef _STD_IO_H
#define _STD_IO_H

#include <std/alloc.h>
#include <std/types.h>

typedef usize (*std_io_write_fn)(void*, const byte*, usize);

struct std_io_writer_impl {
    std_io_write_fn write;
};

struct std_io_writer {
    void *handle;
    struct std_io_writer_impl impl;
};

typedef usize (*std_io_read_fn)(void*, byte*, usize);

struct std_io_reader_impl {
    std_io_read_fn read;
};

struct std_io_reader {
    void *handle;
    struct std_io_reader_impl impl;
};

static inline usize std_io_write(struct std_io_writer *w, const byte* buf, usize nbytes) {
    return w->impl.write(w->handle, buf, nbytes);
}

static inline bool std_io_write_byte(struct std_io_writer *w, byte b) {
    return std_io_write(w, &b, sizeof(b));
}

static inline usize std_io_read(struct std_io_reader *r, byte* buf, usize nbytes) {
    return r->impl.read(r->handle, buf, nbytes);
}

static inline byte std_io_read_byte(struct std_io_reader *r) {
    byte b;
    std_io_read(r, &b, sizeof b);

    return b;
}

struct std_io_buffer {
    byte *buf;
    usize sz;
    usize cap;
    usize idx;
};

static inline struct std_io_buffer std_io_buffer_create(byte *buf, usize nbytes) {
    return (struct std_io_buffer) {
        .buf = buf,
        .cap = nbytes,
    };
}

static inline struct std_io_buffer std_io_buffer_allocate(struct std_allocator *alloc, usize bufsz) {
    byte *buf = alloc->alloc(alloc, bufsz);

    return (struct std_io_buffer) {
        .buf = buf,
        .cap = buf ? bufsz : 0,
    };
}

static inline struct std_io_buffer std_io_buffer_from_bytes(byte *buf, usize nbytes) {
    return (struct std_io_buffer) {
        .buf = buf,
        .cap = nbytes,
        .sz = nbytes,
    };
}

struct std_io_bufreader {
    struct std_io_reader *internal;
    struct std_allocator *alloc;
    struct std_io_reader reader;
    struct std_io_buffer buf;
};

struct std_io_bufwriter {
    struct std_io_writer *internal;
    struct std_allocator *alloc;
    struct std_io_writer writer;
    struct std_io_buffer buf;
};

bool std_io_bufreader_new(struct std_io_bufreader *br, struct std_io_reader *internal, struct std_allocator *alloc);
void std_io_bufreader_with_buf(struct std_io_bufreader *br, struct std_io_reader *internal, struct std_io_buffer *buffer);
void std_io_bufreader_free(struct std_io_bufreader *br);

bool std_io_bufwriter_new(struct std_io_bufwriter *bw, struct std_io_writer *internal, struct std_allocator *alloc);
void std_io_bufwriter_with_buf(struct std_io_bufwriter *bw, struct std_io_writer *internal, struct std_io_buffer *buffer);
void std_io_bufwriter_free(struct std_io_bufwriter *bw);

void std_io_bufwriter_flush(struct std_io_bufwriter *bw);

void std_io_bufwriter_copy(struct std_io_bufwriter *dest, struct std_io_reader *src, usize nbytes);
void std_io_bufwriter_copy_full(struct std_io_bufwriter *dest, struct std_io_reader *src);

struct std_io_reader *std_io_stdin();
struct std_io_writer *std_io_stdout();
struct std_io_writer *std_io_stderr();

void std_io_writer_printc(struct std_io_writer* w, char c);
void std_io_writer_print(struct std_io_writer* w, const char *s);
void std_io_writer_println(struct std_io_writer* w, const char *s);

void std_io_printc(char c);
void std_io_print(const char *s);
void std_io_println(const char *s);

struct std_io_file;

struct std_io_file *std_io_file_open(const char *path, struct std_allocator *alloc);
struct std_io_file *std_io_file_open_r(const char *path, struct std_allocator *alloc);
struct std_io_file *std_io_file_open_w(const char *path, struct std_allocator *alloc);
struct std_io_file *std_io_file_open_rw(const char *path, struct std_allocator *alloc);
struct std_io_file *std_io_file_create(const char *path, struct std_allocator *alloc);
struct std_io_file *std_io_file_create_trunc(const char *path, struct std_allocator *alloc);
void std_io_file_close(struct std_io_file *f);

struct std_io_reader std_io_file_reader(struct std_io_file *f);
struct std_io_writer std_io_file_writer(struct std_io_file *f);

#endif /* _STD_IO_H */
