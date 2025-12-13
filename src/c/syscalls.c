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

#include <syscalls/syscalls.h>

ssize_t sys_read(int fd, void *buf, size_t nbytes) {
    int read;
    SYSCALL3(read, SYS_read, fd, buf, nbytes);

    return read;
}

ssize_t sys_write(int fd, const void *buf, size_t nbytes) {
    long written;
    SYSCALL3(written, SYS_write, fd, buf, nbytes);

    return written;
}

int sys_open(const char *path, int flags, int mode) {
    int fd;
    SYSCALL3(fd, SYS_open, path, flags, mode);

    return fd;
}

int sys_close(int fd) {
    long r;
    SYSCALL1(r, SYS_close, fd);

    return (int) r;
}

void *sys_mmap(void *addr, size_t length, int prot, int flags, int fd, off_t offset) {
    void *ret;
    SYSCALL6(ret, SYS_mmap, addr, length, (long) prot, (long) flags, (long) fd, (long) offset);

    return ret;
}

int mprotect(void *addr, size_t length, int prot) {
    int ret;
    SYSCALL3(ret, SYS_mprotect, addr, length, prot);

    return ret;
}

int sys_munmap(void *addr, size_t length) {
    int ret;
    SYSCALL2(ret, SYS_munmap, addr, length);

    return ret;
}

__attribute__((noreturn))
void sys_exit_group(int exit_status) {
    long _unused;
    SYSCALL1(_unused, SYS_exit_group, exit_status);

    __builtin_unreachable();
}
