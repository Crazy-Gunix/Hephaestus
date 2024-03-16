/*
 * BSD 3-Clause License
 *
 * Copyright (c) 2024, Jacob Gibson
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice, this
 *    list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 *
 * 3. Neither the name of the copyright holder nor the names of its
 *    contributors may be used to endorse or promote products derived from
 *    this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include "file_util.h"

#include <stdio.h>
#include <stdlib.h>

#include <raylib.h>

#include "mem.h"



struct file_dat read_file(char *path)
{
        FILE *fp = NULL;
        struct file_dat fd = {0};

        fp = fopen(path, "rb");
        if (fp == NULL) {
                perror("fopen");
                return fd;
        }

        if (fseek(fp, 0, SEEK_END) != 0) {
                perror("fseek");
                fclose(fp);
                fp = NULL;
                return fd;
        }

        const int size = ftell(fp);
        if (size == -1) {
                perror("ftell");
                fclose(fp);
                fp = NULL;
                return fd;
        }
        if (fseek(fp, 0, 0) != 0) {
                perror("fseek");
                fclose(fp);
                fp = NULL;
                return fd;
        }

        char *data = mem_alloc(size);

        fread(data, size, 1, fp);
        if (ferror(fp)) {
                perror("fread");
                free(data);
                data = NULL;
        } else {
                fd.data = data;
                fd.len = size;
        }

        fclose(fp);
        fp = NULL;
        return fd;
}

