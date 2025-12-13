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

#ifndef _C_SYS_MMAN_H
#define _C_SYS_MMAN_H

#define PROT_READ (0x1)
#define PROT_WRITE (0x2)
#define PROT_EXEC (0x4)
#define PROT_NONE (0x0)

#define MAP_SHARED (0x01)
#define MAP_PRIVATE (0x02)
#define MAP_SHARED_VALIDATE (0x03)
#define MAP_DROPPABLE (0x08)
#define MAP_TYPE (0x0f)
#define MAP_FIXED (0x10)
#define MAP_FILE (0)
#define MAP_ANONYMOUS (0x20)
#define MAP_ANON MAP_ANONYMOUS

#endif /* _C_SYS_MMAN_H */