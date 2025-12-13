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

#ifndef _C_FCNTL_H
#define _C_FCNTL_H

#define O_ACCMODE (0003)
#define O_RDONLY (00)
#define O_WRONLY (01)
#define O_RDWR (02)
#define O_CREAT (0100)
# define O_EXCL (0200)
# define O_NOCTTY (0400)
# define O_TRUNC (01000)
# define O_APPEND (02000)
# define O_NONBLOCK (04000)
# define O_NDELAY (O_NONBLOCK)
# define O_SYNC (04010000)
#define O_FSYNC (O_SYNC)
# define O_ASYNC (020000)

#endif /* _C_FCNTL_H */