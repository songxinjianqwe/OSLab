/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
                            keyboard.c
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
                                                    Forrest Yu, 2005
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

#include "type.h"
#include "const.h"
#include "protect.h"
#include "proto.h"
#include "string.h"
#include "proc.h"
#include "global.h"
#include "keyboard.h"
#include "keymap.h"

PRIVATE KB_INPUT kb_in;
PRIVATE int shift_l;    /* l shift state */
PRIVATE int shift_r;    /* r shift state */
PRIVATE int caps_lock;    /* Caps Lock	 */
PRIVATE int column;
PRIVATE u8 get_byte_from_kbuf();

PUBLIC void keyboard_handler(int irq) {
    u8 scan_code = in_byte(KB_DATA);

    if (kb_in.count < KB_IN_BYTES) {
        *(kb_in.p_head) = scan_code;
        kb_in.p_head++;
        if (kb_in.p_head == kb_in.buf + KB_IN_BYTES) {
            kb_in.p_head = kb_in.buf;
        }
        kb_in.count++;
    }
}


/*======================================================================*
                           init_keyboard
*======================================================================*/
PUBLIC void init_keyboard() {
    kb_in.count = 0;
    kb_in.p_head = kb_in.p_tail = kb_in.buf;

    shift_l = shift_r = 0;

    put_irq_handler(KEYBOARD_IRQ, keyboard_handler);
    enable_irq(KEYBOARD_IRQ);
}


/*======================================================================*
                           keyboard_read
*======================================================================*/
PUBLIC void keyboard_read() {
    u8 scanCode;
    char output[2];
    int make;    /* 1: make;  0: break. */

    u32 key = 0;
    u32 *keyrow;    //points to one row in keymap [][]

    if (kb_in.count > 0) {
        scanCode = get_byte_from_kbuf();
        // judge Make Code or  Break Code

        // if scanCode is make code, make = 1, else make = 0
        make = (scanCode & FLAG_BREAK ? 0 : 1);
        //get the row of key code  in the keymap
        //scanCode and 0x7F to prevent array index out of bounds exception
        keyrow = &keymap[(scanCode & 0x7F) * MAP_COLS];

        column = 0;
        //if last character is shift ,then current character is uppercase
        if (shift_l || shift_r) {
            column = 1;
        }

        //get the real character of key code
        key = keyrow[column];

        switch (key) {
            case SHIFT_L:
                shift_l = make;
                break;
            case SHIFT_R:
                shift_r = make;
                break;
            default:
                break;
        }

        //if key code is make code
        if (make) {
            //if shift_l == 1,then key = FLAG_SHIFT_L,else key = 0
            key |= shift_l ? FLAG_SHIFT_L : 0;
            key |= shift_r ? FLAG_SHIFT_R : 0;
            in_process(key);
        }
    }
}

/*======================================================================*
			    get_byte_from_kbuf
 *======================================================================*/
PRIVATE u8 get_byte_from_kbuf()       //read a byte from my keyboard buffer
{
    u8 scan_code;

    while (kb_in.count <= 0) {}

    disable_int();
    scan_code = *(kb_in.p_tail);
    kb_in.p_tail++;
    if (kb_in.p_tail == kb_in.buf + KB_IN_BYTES) {
        kb_in.p_tail = kb_in.buf;
    }
    kb_in.count--;
    enable_int();
    return scan_code;
}

