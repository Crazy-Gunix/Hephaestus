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

#include "enginel/graphics.h"

#include <lualib.h>
#include <lauxlib.h>
#include <raylib.h>



static int enginel_draw_pixel(lua_State *L)
{ // draw_pixel(x, y, r, g, b, a) -> (nil)
        if (lua_gettop(L) != 6)
                return lua_error(L);

        const Vector2 pos = {
                lua_tonumber(L, 1),
                lua_tonumber(L, 2)
        };
        const Color color = {
                lua_tonumber(L, 3),
                lua_tonumber(L, 4),
                lua_tonumber(L, 5),
                lua_tonumber(L, 6)
        };

        DrawPixelV(pos, color);
        return 0;
}

static int enginel_draw_line(lua_State *L)
{ // draw_line(x1, y1, x2, y2, r, g, b, a) -> (nil)
        if (lua_gettop(L) != 8)
                return lua_error(L);

        const Vector2 start = {
                lua_tonumber(L, 1),
                lua_tonumber(L, 2)
        };
        const Vector2 end = {
                lua_tonumber(L, 3),
                lua_tonumber(L, 4)
        };
        const Color color = {
                lua_tonumber(L, 5),
                lua_tonumber(L, 6),
                lua_tonumber(L, 7),
                lua_tonumber(L, 8)
        };

        DrawLineV(start, end, color);
        return 0;
}

static int enginel_draw_circle(lua_State *L)
{ // draw_circle(x, y, radius, r, g, b, a) -> (nil)
        if (lua_gettop(L) != 7)
                return lua_error(L);

        const Vector2 center = {
                lua_tonumber(L, 1),
                lua_tonumber(L, 2)
        };
        const float radius = lua_tonumber(L, 3);
        const Color color = {
                lua_tonumber(L, 4),
                lua_tonumber(L, 5),
                lua_tonumber(L, 6),
                lua_tonumber(L, 7)
        };

        DrawCircleV(center, radius, color);
        return 0;
}

static int enginel_draw_rect(lua_State *L)
{ // draw_rect(x, y, w, h, r, g, b, a) -> (nil)
        if (lua_gettop(L) != 8)
                return lua_error(L);

        const Rectangle rect = {
                lua_tonumber(L, 1),
                lua_tonumber(L, 2),
                lua_tonumber(L, 3),
                lua_tonumber(L, 4)
        };
        const Color color = {
                lua_tonumber(L, 5),
                lua_tonumber(L, 6),
                lua_tonumber(L, 7),
                lua_tonumber(L, 8)
        };

        DrawRectangleRec(rect, color);
        return 0;
}

static int enginel_draw_tri(lua_State *L)
{ // draw_triangle(x1, y1, x2, y2, x3, y3, r, g, b, a) -> (nil)
        if (lua_gettop(L) != 10)
                return lua_error(L);

        const Vector2 verts[3] = {
                {
                        lua_tonumber(L, 1),
                        lua_tonumber(L, 2)
                },
                {
                        lua_tonumber(L, 3),
                        lua_tonumber(L, 4)
                },
                {
                        lua_tonumber(L, 5),
                        lua_tonumber(L, 6)
                }
        };
        const Color color = {
                lua_tonumber(L, 7),
                lua_tonumber(L, 8),
                lua_tonumber(L, 9),
                lua_tonumber(L, 10)
        };

        DrawTriangle(verts[0], verts[1], verts[2], color);
        return 0;
}





void load_enginel_graphics(lua_State *L)
{
        lua_register(L, "draw_pixel", enginel_draw_pixel);
        lua_register(L, "draw_line", enginel_draw_line);
        lua_register(L, "draw_circle", enginel_draw_circle);
        lua_register(L, "draw_rect", enginel_draw_rect);
        lua_register(L, "draw_tri", enginel_draw_tri);

        return;
}

