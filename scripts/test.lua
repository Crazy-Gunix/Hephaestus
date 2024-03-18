--[[
This is free and unencumbered software released into the public domain.

Anyone is free to copy, modify, publish, use, compile, sell, or
distribute this software, either in source code form or as a compiled
binary, for any purpose, commercial or non-commercial, and by any
means.

In jurisdictions that recognize copyright laws, the author or authors
of this software dedicate any and all copyright interest in the
software to the public domain. We make this dedication for the benefit
of the public at large and to the detriment of our heirs and
successors. We intend this dedication to be an overt act of
relinquishment in perpetuity of all present and future rights to this
software under copyright law.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
IN NO EVENT SHALL THE AUTHORS BE LIABLE FOR ANY CLAIM, DAMAGES OR
OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
OTHER DEALINGS IN THE SOFTWARE.

For more information, please refer to <https://unlicense.org>
]]--



local x = 40;
local y = 500;



function init()
    print("Hello from Lua!")

    return
end

function loop()
    draw_pixel(96, 12, 255, 255, 255, 255)
    draw_line(112, 256, 80, 384, 0, 0, 255, 255)
    draw_circle(544, 420, 80, 0, 255, 255, 255)
    draw_rect(32, 64, 256, 96, 255, 0, 0, 255)
    draw_tri(632, 24, 568, 128, 696, 128, 255, 0, 255, 255)
    draw_text("WSAD to move", 324, 286, 24, 0, 255, 0, 255)

    if key_down(KEY_W) then
        y = y - 5
    elseif key_down(KEY_S) then
        y = y + 5
    end
    if key_down(KEY_A) then
        x = x - 5
    elseif key_down(KEY_D) then
        x = x + 5
    end

    draw_rect(x, y, 100, 100, 225, 225, 225, 255)

    return
end

function exit()
    print("Goodbye!")

    return
end
