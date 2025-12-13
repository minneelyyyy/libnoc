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

#include <std/types.h>
#include <c/string.h>

size_t strlen(const char *s) {
    size_t len = 0;

    while (s[len])
        len++;

    return len;
}

void *memcpy(void *restrict dest, const void *restrict src, size_t n) {
    byte *restrict pdest = (byte *restrict)dest;
    const byte *restrict psrc = (const byte *restrict)src;

    for (size_t i = 0; i < n; i++) {
        pdest[i] = psrc[i];
    }

    return dest;
}

void *memset(void *s, int c, size_t n) {
    byte *p = (byte *)s;

    for (size_t i = 0; i < n; i++) {
        p[i] = (byte)c;
    }

    return s;
}

void *memmove(void *dest, const void *src, size_t n) {
    byte *pdest = (byte *)dest;
    const byte *psrc = (const byte *)src;

    if (src > dest) {
        for (size_t i = 0; i < n; i++) {
            pdest[i] = psrc[i];
        }
    } else if (src < dest) {
        for (size_t i = n; i > 0; i--) {
            pdest[i-1] = psrc[i-1];
        }
    }

    return dest;
}

int memcmp(const void *s1, const void *s2, size_t n) {
    const byte *p1 = (const byte *)s1;
    const byte *p2 = (const byte *)s2;

    for (size_t i = 0; i < n; i++) {
        if (p1[i] != p2[i]) {
            return p1[i] < p2[i] ? -1 : 1;
        }
    }

    return 0;
}
