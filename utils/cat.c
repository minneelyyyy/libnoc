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
#include <std/types.h>

int main(int argc, const char *argv[]) {
    if (argc == 1) {
        std_io_println("USAGE: cat <FILES...>");
        return 1;
    }

    byte stack_backed_buffer[4096];
    struct std_io_buffer buffer = std_io_buffer_create(stack_backed_buffer, sizeof stack_backed_buffer);

    struct std_io_bufwriter stdout = {};
    std_io_bufwriter_with_buf(&stdout, std_io_stdout(), &buffer);

    for (int i = 1; i < argc; i++) {
        struct std_io_file *file = std_io_file_open(argv[i], &std_heap);
        struct std_io_reader reader = std_io_file_reader(file);

        std_io_bufwriter_copy_full(&stdout, &reader);

        std_io_file_close(file);
    }

    return 0;
}
