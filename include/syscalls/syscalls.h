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

#include <c/sys/types.h>
#include <c/stddef.h>

#define SYSCALL0(_ret, _idx) \
    __asm__ volatile( \
        "syscall" \
            : "=a"(_ret) \
            : "a"(_idx) \
            : "memory", "rcx", "r11")

#define SYSCALL1(_ret, _idx, _a) \
    __asm__ volatile( \
        "syscall" \
            : "=a"(_ret) \
            : "a"(_idx), "D"(_a) \
            : "memory", "rcx", "r11")

#define SYSCALL2(_ret, _idx, _a, _b) \
    __asm__ volatile( \
        "syscall" \
            : "=a"(_ret) \
            : "a"(_idx), "D"(_a), "S"(_b) \
            : "memory", "rcx", "r11")

#define SYSCALL3(_ret, _idx, _a, _b, _c) \
    __asm__ volatile( \
        "syscall" \
            : "=a"(_ret) \
            : "a"(_idx), "D"(_a), "S"(_b), "d"(_c) \
            : "memory", "rcx", "r11")

#define SYSCALL4(_ret, _idx, _a, _b, _c, _d) \
    __asm__ volatile( \
        "movq %5, %%r10\n\t" \
        "syscall" \
            : "=a"(_ret) \
            : "a"(_idx), "D"(_a), "S"(_b), "d"(_c), "r"(_d) \
            : "memory", "rcx", "r10", "r11")

#define SYSCALL5(_ret, _idx, _a, _b, _c, _d, _e) \
    __asm__ volatile( \
        "movq %5, %%r10\n\t" \
        "movq %6, %%r8\n\t" \
        "syscall" \
            : "=a"(_ret) \
            : "a"(_idx), "D"(_a), "S"(_b), "d"(_c), "r"(_d), "r"(_e) \
            : "memory", "rcx", "r8", "r10", "r11")

#define SYSCALL6(_ret, _idx, _a, _b, _c, _d, _e, _f) \
    __asm__ volatile( \
        "movq %5, %%r10\n\t" \
        "movq %6, %%r8\n\t" \
        "movq %7, %%r9\n\t" \
        "syscall" \
            : "=a"(_ret) \
            : "a"(_idx), "D"(_a), "S"(_b), "d"(_c), "r"(_d), "r"(_e), "r"(_f) \
            : "memory", "rcx", "r8", "r9", "r10", "r11")

#define SYS_read (0)
#define SYS_write (1)
#define SYS_open (2)
#define SYS_close (3)
/* ... */
#define SYS_mmap (9)
#define SYS_mprotect (10)
#define SYS_munmap (11)
/* ... */
#define SYS_exit_group (231)
/* ... */

ssize_t sys_read(int fd, void *buf, size_t nbytes);
ssize_t sys_write(int fd, const void *buf, size_t nbytes);
int sys_open(const char *path, int flags, int mode);
int sys_close(int fd);

void *sys_mmap(void *addr, size_t length, int prot, int flags, int fd, off_t offset);
int mprotect(void *addr, size_t length, int prot);
int sys_munmap(void *addr, size_t length);

__attribute__((noreturn))
void sys_exit_group(int exit_status);
