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

#include "engine/engine.h"

#include <string.h>
#include <stdlib.h>

#include <lauxlib.h>
#include <lualib.h>

#include <raylib.h>
#include <raygui.h>
#include <dark/style_dark.h>

#include "util/mem.h"
#include "util/lua_helper.h"
#include "util/file.h"
#include "util/archive_helper.h"
#include "util/log.h"



static inline void load_file_dat(struct engine *e)
{
        FilePathList f = LoadDroppedFiles();
        e->loadable_file = true;
        
        const size_t size = strlen(f.paths[0]) + 1;
        e->loadable_path = (char*)mem_alloc(size);
        strncpy(e->loadable_path, f.paths[0], size);
        
        UnloadDroppedFiles(f);
        
        TraceLog(LOG_DEBUG, "file dropped: %s", e->loadable_path);

        return;
}

static inline void yes_load_file(struct engine *e)
{
        struct file_dat fd = read_file(e->loadable_path);
        e->loadable_file = false;

        if (fd.len == 0) {
                printlog(LERROR, "failed to load file", &e->log);
                free(e->loadable_path);
                e->loadable_path = NULL;
                return;
        }

        e->loaded_data = fd.data;

        if (IsFileExtension(e->loadable_path, ".lua")) {
                lua_script_init(e->L, e->loaded_data);
                e->file_loaded = true;
        } else if (IsFileExtension(e->loadable_path, ".tgz")) {
                ls_archive(e->loaded_data, fd.len);
        }

        free(e->loaded_data);
        free(e->loadable_path);
        e->loaded_data = NULL;
        e->loadable_path = NULL;

        return;
}



void engine_init(struct engine *e)
{
        InitWindow(800, 600, "hephaestus");
        GuiLoadStyleDark();
        SetTargetFPS(120);

        logger_init(&e->log);

        e->L = init_lua();
        if (e->L == NULL) {
                CloseWindow();
                return;
        }

        e->init = true;

        return;
}

void engine_run(struct engine *e)
{
        const Color bg_color = GetColor(GuiGetStyle(DEFAULT, BACKGROUND_COLOR));

        while (!WindowShouldClose()) {
                if (IsFileDropped()) {
                        if (!e->loadable_file && !e->file_loaded)
                                load_file_dat(e);
                        else // Ignore
                                UnloadDroppedFiles(LoadDroppedFiles());
                }

                if (IsKeyDown(KEY_F5) && e->file_loaded) {
                        lua_script_exit(e->L);
                        e->file_loaded = false;
                }

                BeginDrawing(); // Start Drawing Proccess

                ClearBackground(bg_color);

                if (e->loadable_file && !e->file_loaded) {
                       DrawText("Load File", 340, 160, 32, RAYWHITE);
                       DrawText(GetFileName(e->loadable_path),
                                       340, 192, 32, RAYWHITE);

                       GuiSetStyle(BUTTON, TEXT_ALIGNMENT, TEXT_ALIGN_CENTER);
                       if (GuiButton((Rectangle){256, 256, 128, 32}, "Yes"))
                               yes_load_file(e);
                       if (GuiButton((Rectangle){420, 256, 128, 32}, "No")) {
                                e->loadable_file = false;
                                e->file_loaded = false;
                                free(e->loadable_path);
                                e->loadable_path = NULL;
                       }
                }
                if (e->file_loaded)
                        e->file_loaded = lua_script_loop(e->L);

#ifndef NDEBUG
                DrawFPS(0, 0);
#endif

                EndDrawing(); // End Drawing Process
        }

        return;
}

void engine_cleanup(struct engine *e)
{
        if (e->file_loaded)
                lua_script_exit(e->L);
        lua_close(e->L);

        printlog(LINFO, "Closed Lua state", &e->log);

        CloseWindow();

        logger_quit(&e->log);

        if (e->loadable_path != NULL) {
                free(e->loadable_path);
                e->loadable_path = NULL;
        }

        return;
}

