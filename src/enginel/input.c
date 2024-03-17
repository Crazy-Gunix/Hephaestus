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

#include "enginel/input.h"

#include <string.h>

#include <lualib.h>
#include <lauxlib.h>
#include <raylib.h>



const char *enginel_keys = ""
        "KEY_APOSTROPHE = 39\n"
        "KEY_COMMA = 44\n"
        "KEY_MINUS = 45\n"
        "KEY_PERIOD = 46\n"
        "KEY_SLASH = 47\n"
        "KEY_ZERO = 48\n"
        "KEY_ONE = 49\n"
        "KEY_TWO = 50\n"
        "KEY_THREE = 51\n"
        "KEY_FOUR = 52\n"
        "KEY_FIVE = 53\n"
        "KEY_SIX = 54\n"
        "KEY_SEVEN = 55\n"
        "KEY_EIGHT = 56\n"
        "KEY_NINE = 57\n"
        "KEY_SEMICOLON = 59\n"
        "KEY_EQUAL = 61\n"
        "KEY_A = 65\n"
        "KEY_B = 66\n"
        "KEY_C = 67\n"
        "KEY_D = 68\n"
        "KEY_E = 69\n"
        "KEY_F = 70\n"
        "KEY_G = 71\n"
        "KEY_H = 72\n"
        "KEY_I = 73\n"
        "KEY_J = 74\n"
        "KEY_K = 75\n"
        "KEY_L = 76\n"
        "KEY_M = 77\n"
        "KEY_N = 78\n"
        "KEY_O = 79\n"
        "KEY_P = 80\n"
        "KEY_Q = 81\n"
        "KEY_R = 82\n"
        "KEY_S = 83\n"
        "KEY_T = 84\n"
        "KEY_U = 85\n"
        "KEY_V = 86\n"
        "KEY_W = 87\n"
        "KEY_X = 88\n"
        "KEY_Y = 89\n"
        "KEY_Z = 90\n"
        "KEY_LEFT_BRACKET = 91\n"
        "KEY_BACKSLASH = 92\n"
        "KEY_RIGHT_BRACKET = 93\n"
        "KEY_GRAVE = 96\n"
        "KEY_SPACE = 32\n"
        "KEY_ESCAPE = 256\n"
        "KEY_ENTER = 257\n"
        "KEY_TAB = 258\n"
        "KEY_BACKSPACE = 259\n"
        "KEY_INSERT = 260\n"
        "KEY_DELETE = 261\n"
        "KEY_RIGHT = 262\n"
        "KEY_LEFT = 263\n"
        "KEY_DOWN = 264\n"
        "KEY_UP= 265\n"
        "KEY_PAGE_UP = 266\n"
        "KEY_PAGE_DOWN = 267\n"
        "KEY_HOME = 268\n"
        "KEY_END = 269\n"
        "KEY_CAPS_LOCK = 280\n"
        "KEY_SCROLL_LOCK = 281\n"
        "KEY_NUM_LOCK = 282\n"
        "KEY_PRINT_SCREEN = 283\n"
        "KEY_PAUSE = 284\n"
        "KEY_F1 = 290\n"
        "KEY_F2 = 291\n"
        "KEY_F3 = 292\n"
        "KEY_F4 = 293\n"
        "KEY_F5 = 294\n"
        "KEY_F6 = 295\n"
        "KEY_F7 = 296\n"
        "KEY_F8 = 297\n"
        "KEY_F9 = 298\n"
        "KEY_F10 = 299\n"
        "KEY_F11 = 300\n"
        "KEY_F12 = 301\n"
        "KEY_LEFT_SHIFT = 340\n"
        "KEY_LEFT_CONTROL = 341\n"
        "KEY_LEFT_ALT = 342\n"
        "KEY_LEFT_SUPER= 343\n"
        "KEY_RIGHT_SHIFT = 344\n"
        "KEY_RIGHT_CONTROL = 345\n"
        "KEY_RIGHT_ALT = 346\n"
        "KEY_RIGHT_SUPER = 347\n"
        "KEY_KB_MENU = 348\n"
        "KEY_KP_0 = 320\n"
        "KEY_KP_1 = 321\n"
        "KEY_KP_2 = 322\n"
        "KEY_KP_3 = 323\n"
        "KEY_KP_4 = 324\n"
        "KEY_KP_5 = 325\n"
        "KEY_KP_6 = 326\n"
        "KEY_KP_7 = 327\n"
        "KEY_KP_8 = 328\n"
        "KEY_KP_9 = 329\n"
        "KEY_KP_DECIMAL = 330\n"
        "KEY_KP_DIVIDE = 331\n"
        "KEY_KP_MULTIPLY = 332\n"
        "KEY_KP_SUBTRACT = 333\n"
        "KEY_KP_ADD = 334\n"
        "KEY_KP_ENTER = 335\n"
        "KEY_KP_EQUAL = 336\n";

const char *enginel_mouse = ""
        "MOUSE_LEFT = 0\n"
        "MOUSE_MIDDLE = 2\n"
        "MOUSE_RIGHT = 1\n";



static int enginel_key_down(lua_State *L)
{ // key_down(key) -> (bool)
        if (lua_gettop(L) != 1)
                return lua_error(L);

        lua_pushboolean(L, IsKeyDown(lua_tonumber(L, 1)));
        return 1;
}

static int enginel_key_up(lua_State *L)
{ // key_up(key) -> (bool)
        if (lua_gettop(L) != 1)
                return lua_error(L);

        lua_pushboolean(L, IsKeyUp(lua_tonumber(L, 1)));
        return 1;
}

static int enginel_mouse_down(lua_State *L)
{ // mouse_down(button) -> (bool)
        if (lua_gettop(L) != 1)
                return lua_error(L);

        lua_pushboolean(L, IsMouseButtonDown(lua_tonumber(L, 1)));
        return 1;
}

static int enginel_mouse_up(lua_State *L)
{ // mouse_up(button) -> (bool)
        if (lua_gettop(L) != 1)
                return lua_error(L);

        lua_pushboolean(L, IsMouseButtonUp(lua_tonumber(L, 1)));
        return 1;
}



void load_enginel_input(lua_State *L)
{
        luaL_dostring(L, enginel_keys);
        luaL_dostring(L, enginel_mouse);

        lua_register(L, "key_down", enginel_key_down);
        lua_register(L, "key_up", enginel_key_up);
        lua_register(L, "mouse_down", enginel_mouse_down);
        lua_register(L, "mouse_up", enginel_mouse_up);

        return;
}
