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

#include "engine/lua_util.h"

#include <lauxlib.h>
#include <lualib.h>
#include <raylib.h>

#include "enginel/enginel.h"



static inline bool check_lua(lua_State *L, int r)
{
        if (r != LUA_OK) {
                TraceLog(LOG_ERROR, "%s\n", lua_tostring(L, -1));
                return false;
        }

        return true;
}

static inline bool call_void_lua(lua_State *L, const char *func_name)
{
        lua_getglobal(L, func_name);
        if (!lua_isfunction(L, -1))
                return false;
        if (!check_lua(L, lua_pcall(L, 0, 0, 0)))
                return false;

        return true;
}





lua_State *init_lua(void)
{
        lua_State *L = NULL;
        L = luaL_newstate();

        if (L == NULL) {
                TraceLog(LOG_ERROR, "Failed to load Lua state");
                return L;
        }

        luaL_openlibs(L);
        TraceLog(LOG_INFO, "Loaded Lua state successfully");

        load_enginel(L);

        return L;
}

void lua_script_init(lua_State *L, const char *data)
{
        if (check_lua(L, luaL_dostring(L, data))) {
                if (call_void_lua(L, "init")) {
                        TraceLog(LOG_DEBUG, "initialized lua script successfully");
                        return;
                }
                TraceLog(LOG_WARNING, "failed to init lua script");
                return;
        }

        TraceLog(LOG_ERROR, "failed to load lua script");
        return;
}

void lua_script_exit(lua_State *L)
{
        if (call_void_lua(L, "exit")) {
                TraceLog(LOG_DEBUG, "exited lua script successfully");
                return;
        }

        TraceLog(LOG_WARNING, "failed to safely exit lua");
        return;
}

bool lua_script_loop(lua_State *L)
{
        if (!call_void_lua(L, "loop")) {
                TraceLog(LOG_ERROR, "loop script failed");
                return false;
        }

        return true;
}

