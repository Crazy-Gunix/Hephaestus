local x = 40
local y = 500

function init()
        print("Hello from Lua!")
        return
end

function loop()
        draw_pixel(96, 12, 255, 255, 255, 255)
        draw_line(112, 256, 80, 384, 0, 0, 255, 255)
        draw_circle(544, 420, 80, 0, 255, 255, 255, 255)
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
