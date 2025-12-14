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

#include <std/io.h>
#include <std/alloc.h>
#include <c/string.h>
#include <c/fcntl.h>
#include <syscalls/syscalls.h>

enum file_perms {
    FILE_READ = 0x1,
    FILE_WRITE = 0x2,
    FILE_READ_WRITE = FILE_READ | FILE_WRITE,
};

struct std_io_file {
    struct std_allocator *alloc;
    int fd;
    enum file_perms perms;
};

struct std_io_buf_writer {
    struct std_io_writer *internal;
};

struct std_io_file *_std_io_file_open(const char *path, int perms, struct std_allocator *alloc) {
    // TODO: error reporting method
    int fd = sys_open(path, perms, 0);
    if (fd < 0)
        return NULL;

    struct std_io_file *file = alloc->alloc(alloc, sizeof *file);
    if (!file)
        return NULL;

    file->alloc = alloc;
    file->fd = fd;

    return file;
}

struct std_io_file *std_io_file_open(const char *path, struct std_allocator *alloc) {
    return std_io_file_open_r(path, alloc);
}

struct std_io_file *std_io_file_open_r(const char *path, struct std_allocator *alloc) {
    struct std_io_file *f = _std_io_file_open(path, O_RDONLY, alloc);
    if (!f)
        return NULL;

    f->perms = FILE_READ;
    return f;
}

struct std_io_file *std_io_file_open_w(const char *path, struct std_allocator *alloc) {
    struct std_io_file *f = _std_io_file_open(path, O_WRONLY, alloc);
    if (!f)
        return NULL;

    f->perms = FILE_WRITE;
    return f;
}

struct std_io_file *std_io_file_open_rw(const char *path, struct std_allocator *alloc) {
    struct std_io_file *f = _std_io_file_open(path, O_RDWR, alloc);
    if (!f)
        return NULL;

    f->perms = FILE_READ_WRITE;
    return f;
}

struct std_io_file *std_io_file_create(const char *path, struct std_allocator *alloc) {
    struct std_io_file *f = _std_io_file_open(path, O_WRONLY | O_CREAT | O_EXCL, alloc);
    if (!f)
        return NULL;

    f->perms = FILE_WRITE;
    return f;
}

struct std_io_file *std_io_file_create_trunc(const char *path, struct std_allocator *alloc) {
    struct std_io_file *f = _std_io_file_open(path, O_WRONLY | O_CREAT | O_TRUNC, alloc);
    if (!f)
        return NULL;

    f->perms = FILE_WRITE;
    return f;
}

void std_io_file_close(struct std_io_file *f) {
    sys_close(f->fd);
    f->alloc->free(f->alloc, f);
}

usize _std_io_file_read(void *handle, byte* buf, usize nbytes) {
    struct std_io_file *f = handle;
    return sys_read(f->fd, buf, nbytes);
}

usize _std_io_file_write(void *handle, const byte* buf, usize nbytes) {
    struct std_io_file *f = handle;
    return sys_write(f->fd, buf, nbytes);
}

struct std_io_reader std_io_file_reader(struct std_io_file *f) {
    return (struct std_io_reader) {
        .handle = f,
        .impl = {
            .read = _std_io_file_read,
        },
    };
}

struct std_io_writer std_io_file_writer(struct std_io_file *f) {
    return (struct std_io_writer) {
        .handle = f,
        .impl = {
            .write = _std_io_file_write,
        },
    };
}

usize _std_io_fd_read(void* handle, byte* buffer, usize nbytes) {
    int fd = (intptr) handle;
    return sys_read(fd, buffer, nbytes);
}

usize _std_io_fd_write(void* handle, const byte* buffer, usize nbytes) {
    int fd = (intptr) handle;
    return sys_write(fd, buffer, nbytes);
}

static struct std_io_reader stdin_reader = {
    .handle = (void*) 0x0,
    .impl = {
        .read = _std_io_fd_read,
    },
};

static struct std_io_writer stdout_writer = {
    .handle = (void*) 1,
    .impl = {
        .write = _std_io_fd_write,
    },
};

static struct std_io_writer stderr_writer = {
    .handle = (void*) 2,
    .impl = {
        .write = _std_io_fd_write,
    },
};

struct std_io_reader *std_io_stdin() {
    return &stdin_reader;
}

struct std_io_writer *std_io_stdout() {
    return &stdout_writer;
}

struct std_io_writer *std_io_stderr() {
    return &stderr_writer;
}

void std_io_writer_printc(struct std_io_writer* w, char c) {
    w->impl.write(w->handle, (const byte*) &c, sizeof c);
}

void std_io_writer_print(struct std_io_writer* w, const char *s) {
    w->impl.write(w->handle, (const byte*) s, strlen(s));
}

void std_io_writer_println(struct std_io_writer* w, const char *s) {
    std_io_writer_print(w, s);
    std_io_writer_printc(w, '\n');
}

void std_io_printc(char c) {
    std_io_writer_printc(&stdout_writer, c);
}

void std_io_print(const char *s) {
    std_io_writer_print(&stdout_writer, s);
}

void std_io_println(const char *s) {
    std_io_writer_println(&stdout_writer, s);
}
