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

struct std_io_writer_impl {
    usize (*write)(void*, const byte*, usize);
};

struct std_io_writer {
    void *handle;
    struct std_io_writer_impl impl;
};

static inline usize std_io_write(struct std_io_writer *writer, const byte* buf, usize nbytes) {
    return writer->impl.write(writer->handle, buf, nbytes);
}

static inline usize std_io_writes(struct std_io_writer *writer, const char* s, usize nbytes) {
    return writer->impl.write(writer->handle, (const byte*) s, nbytes);
}

struct std_io_buf_writer;

struct std_io_reader_impl {
    usize (*read)(void*, byte*, usize);
};

struct std_io_reader {
    void *handle;
    struct std_io_reader_impl impl;
};

static inline usize std_io_read(struct std_io_reader *reader, byte* buf, usize nbytes) {
    return reader->impl.read(reader->handle, buf, nbytes);
}

static inline usize std_io_reads(struct std_io_reader *reader, char* s, usize nbytes) {
    return reader->impl.read(reader->handle, (byte*) s, nbytes);
}

struct std_io_file;
extern struct std_io_writer_impl std_io_file_impl;

#define STD_IO_FILE_READ (0x1)
#define STD_IO_FILE_WRITE (0x2)

struct std_io_file *std_io_file_open(const char *path, u32 perms, struct std_allocator *alloc);
void std_io_file_close(struct std_io_file *f);
struct std_io_reader std_io_file_reader(struct std_io_file *f);

struct std_io_reader *std_io_stdin();
struct std_io_writer *std_io_stdout();
struct std_io_writer *std_io_stderr();

void std_io_printc(char s);
void std_io_print(const char *s);
void std_io_println(const char *s);

#endif /* _STD_IO_H */
