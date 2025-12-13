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
#include <std/types.h>

int main(int argc, char *argv[]) {
    if (argc == 1) {
        std_io_println("USAGE: cat <FILES...>");
        return 1;
    }

    struct std_io_writer *stdout = std_io_stdout();

    for (int i = 1; i < argc; i++) {
        const char *filepath = argv[i];
        struct std_io_file *file = std_io_file_open(filepath, STD_IO_FILE_READ, &std_heap);
        struct std_io_reader reader = std_io_file_reader(file);

        usize n;
        byte buf[4096];
        while ((n = std_io_read(&reader, buf, sizeof buf)) > 0) {
            std_io_write(stdout, buf, n);
        }

        std_io_file_close(file);
    }

    return 0;
}
