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

#include "engine.h"

#include <stdbool.h>
#include <string.h>
#include <stdlib.h>

#include <lauxlib.h>
#include <lualib.h>

#include <raylib.h>
#include <raygui.h>
#include <dark/style_dark.h>

#include "mem.h"
#include "lua_util.h"
#include "file_util.h"
#include "json_util.h"
#include "archive_util.h"

void engine_init(struct engine *e)
{
        InitWindow(800, 600, "hephaestus");
        GuiLoadStyleDark();
        SetTargetFPS(120);
        
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
                        if (!e->file_to_load && !e->file_loaded) {
                                FilePathList f = LoadDroppedFiles();
                                e->file_to_load = true;
                                e->file_to_load_path = (char*)mem_alloc(strlen(f.paths[0]) + 1);
                                strcpy(e->file_to_load_path, f.paths[0]);
                                UnloadDroppedFiles(f);
                                TraceLog(LOG_DEBUG, "file dropped: %s", e->file_to_load_path);
                        } else { // Ignore
                                UnloadDroppedFiles(LoadDroppedFiles());
                        }
                }

                if (IsKeyDown(KEY_F5) && e->file_loaded) {
                        lua_script_exit(e->L);
                        e->file_loaded = false;
                }

                BeginDrawing();

                ClearBackground(bg_color);

                if (e->file_to_load && !e->file_loaded) {
                       DrawText("Load File", 200, 150, 32, RAYWHITE);
                       DrawText(GetFileName(e->file_to_load_path), 200, 200, 32, RAYWHITE);
                       GuiSetStyle(BUTTON, TEXT_ALIGNMENT, TEXT_ALIGN_CENTER);
                       if (GuiButton((Rectangle){25, 255, 125, 30}, "Yes")) {
                                e->file_to_load = false;
                                struct file_dat fd = read_file(e->file_to_load_path);
                                if (fd.len != 0) {
                                        e->loaded_data = fd.data;
                                        printf("%s\n", GetFileExtension(e->file_to_load_path));
                                        if (IsFileExtension(e->file_to_load_path, ".lua")) {
                                                lua_script_init(e->L, e->loaded_data);
                                                e->file_loaded = true;
                                        } else if (IsFileExtension(e->file_to_load_path, ".tgz")) {
                                                ls_archive(e->loaded_data, fd.len);
                                        }
                                        mem_free(e->loaded_data);
                                } else {
                                        TraceLog(LOG_ERROR, "failed to load file");
                                }
                                mem_free(e->file_to_load_path);
                       }
                       if (GuiButton((Rectangle){255, 255, 125, 30}, "No")) {
                                e->file_to_load = false;
                                e->file_loaded = false;
                                mem_free(e->file_to_load_path);
                       }
                }
                if (e->file_loaded)
                        e->file_loaded = lua_script_loop(e->L);

#ifndef NDEBUG
                DrawFPS(0, 0);
#endif

                EndDrawing();
        } 
}

void engine_cleanup(struct engine *e)
{
        if (e->file_loaded)
                lua_script_exit(e->L);

        lua_close(e->L);
        TraceLog(LOG_INFO, "Closed Lua state");
        CloseWindow();

        return;
}

