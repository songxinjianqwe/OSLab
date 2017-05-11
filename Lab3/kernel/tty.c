
#include "type.h"
#include "const.h"
#include "protect.h"
#include "proto.h"
#include "string.h"
#include "proc.h"
#include "global.h"
#include "keyboard.h"


PUBLIC void task_tty() {
    while (1) {
        keyboard_read();
    }
}


PUBLIC void in_process(u32 key) {
    char output[2] = {'\0', '\0'};
    //if key is printable,then print it
    if (!(key & FLAG_EXT)) {
        output[0] = key & 0xFF;
        disp_str(output);
        // move cursor to the current display position
        disable_int();
        out_byte(CRTC_ADDR_REG, CURSOR_H);
        out_byte(CRTC_DATA_REG, ((disp_pos / 2) >> 8) & 0xFF);
        out_byte(CRTC_ADDR_REG, CURSOR_L);
        out_byte(CRTC_DATA_REG, (disp_pos / 2) & 0xFF);
        enable_int();
    } else {
        //process unprintable character

    }
}

