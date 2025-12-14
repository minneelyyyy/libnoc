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
#include <std/io.h>
#include <std/memory.h>

#define min(_a, _b) ({ \
    typeof(_a) a = _a; \
    typeof(_b) b = _b; \
    a < b ? a : b; \
})

#define BUFFER_SIZE (4096)

usize std_io_buffer_consume(struct std_io_buffer *buffer, byte *buf, usize nbytes) {
    const usize nbytes_to_consume = min(nbytes, buffer->sz - buffer->idx);

    std_memory_copy(buf, buffer->buf + buffer->idx, nbytes_to_consume);

    buffer->idx += nbytes_to_consume;

    // reset buffer if completely drained
    if (buffer->idx == buffer->sz)
        buffer->sz = buffer->idx = 0;

    return nbytes_to_consume;
}

usize std_io_buffer_fill(struct std_io_buffer *buffer, struct std_io_reader *reader) {
    // attempt to fill from sz to end of buffer. sz is most likely 0 at this point anyway
    usize n = std_io_read(reader, buffer->buf + buffer->sz, buffer->cap - buffer->sz);
    buffer->sz += n;

    return n;
}

usize std_io_buffer_flush(struct std_io_buffer *buffer, struct std_io_writer *writer) {
    const usize size = buffer->sz;
    usize nbytes = size;
    usize n;

    while ((n = std_io_write(writer, buffer->buf + buffer->idx, nbytes)) < nbytes) {
        if (n == 0)
            return size - nbytes;

        nbytes -= n;
        buffer->idx += n;
    }

    buffer->sz = buffer->idx = 0;

    return size;
}

usize std_io_buffer_fill_bytes(struct std_io_buffer *buffer, const byte *buf, usize nbytes) {
    const usize nbytes_to_fill = min(nbytes, buffer->cap - buffer->sz);

    std_memory_copy(buffer->buf + buffer->sz, buf, nbytes_to_fill);
    buffer->sz += nbytes_to_fill;

    return nbytes_to_fill;
}

usize _std_io_buffered_read(struct std_io_bufreader *reader, byte *buf, usize nbytes) {
    struct std_io_buffer *buffer = &reader->buf;

    // consume as many bytes from the buffer as possible
    usize n = std_io_buffer_consume(buffer, buf, nbytes);
    usize rem = nbytes - n;

    if (rem > 0) {
        // the read request is too big for our buffer to matter
        if (rem >= buffer->cap) {
            n += std_io_read(reader->internal, buf + n, rem);
            return n;
        }

        std_io_buffer_fill(buffer, reader->internal);
        n += std_io_buffer_consume(buffer, buf + n, rem);
    }

    return n;
}

static void _std_io_bufreader_create(struct std_io_bufreader *br, struct std_io_reader *internal,
                                                         struct std_allocator *alloc, struct std_io_buffer *buffer)
{
    *br = (struct std_io_bufreader) {
        .internal = internal,
        .reader = {
            .handle = br,
            .impl = {
                .read = (std_io_read_fn) _std_io_buffered_read,
            },
        },
        .alloc = alloc,
        .buf = *buffer,
    };
}

bool std_io_bufreader_new(struct std_io_bufreader *br, struct std_io_reader *internal, struct std_allocator *alloc) {
    struct std_io_buffer buffer = std_io_buffer_allocate(alloc, BUFFER_SIZE);
    if (buffer.cap == 0)
        return false;

    _std_io_bufreader_create(br, internal, alloc, &buffer);

    return true;
}

void std_io_bufreader_with_buf(struct std_io_bufreader *br, struct std_io_reader *internal, struct std_io_buffer *buffer) {
    _std_io_bufreader_create(br, internal, NULL, buffer);
}

void std_io_bufreader_free(struct std_io_bufreader *br) {
    if (br->alloc)
        br->alloc->free(br->alloc, br->buf.buf);
}

usize _std_io_buffered_write(struct std_io_bufwriter *bw, byte *buf, usize nbytes) {
    struct std_io_buffer *buffer = &bw->buf;

    // nbytes is too big for our buffer to matter
    if (nbytes > buffer->cap) {
        usize n = std_io_buffer_flush(buffer, bw->internal);
        n += std_io_write(bw->internal, buf, nbytes);

        return n;
    }

    usize n = std_io_buffer_fill_bytes(buffer, buf, nbytes);
    usize rem = nbytes - n;

    if (rem > 0) {
        std_io_buffer_flush(buffer, bw->internal);
        n += std_io_buffer_fill_bytes(buffer, buf + n, rem);
    }

    return n;
}

static void _std_io_bufwriter_create(struct std_io_bufwriter *bw, struct std_io_writer *internal,
                                                         struct std_allocator *alloc, struct std_io_buffer *buffer)
{
    *bw = (struct std_io_bufwriter) {
        .internal = internal,
        .writer = {
            .handle = bw,
            .impl = {
                .write = (std_io_write_fn) _std_io_buffered_write,
            },
        },
        .alloc = alloc,
        .buf = *buffer,
    };
}

bool std_io_bufwriter_new(struct std_io_bufwriter *bw, struct std_io_writer *internal, struct std_allocator *alloc) {
    struct std_io_buffer buffer = std_io_buffer_allocate(alloc, BUFFER_SIZE);
    if (buffer.cap == 0)
        return false;

    _std_io_bufwriter_create(bw, internal, alloc, &buffer);
    return true;
}

void std_io_bufwriter_with_buf(struct std_io_bufwriter *bw, struct std_io_writer *internal, struct std_io_buffer *buffer) {
    _std_io_bufwriter_create(bw, internal, NULL, buffer);
}

void std_io_bufwriter_free(struct std_io_bufwriter *bw) {
    if (bw->alloc)
        bw->alloc->free(bw->alloc, bw->buf.buf);
}

void std_io_bufwriter_flush(struct std_io_bufwriter *bw) {
    std_io_buffer_flush(&bw->buf, bw->internal);
}

void std_io_bufwriter_copy(struct std_io_bufwriter *dest, struct std_io_reader *src, usize nbytes) {
    (void) dest;
    (void) src;
    (void) nbytes;
}

void std_io_bufwriter_copy_full(struct std_io_bufwriter *dest, struct std_io_reader *src) {
    while (std_io_buffer_fill(&dest->buf, src) > 0) {
        std_io_bufwriter_flush(dest);
    }

    std_io_bufwriter_flush(dest);
}
