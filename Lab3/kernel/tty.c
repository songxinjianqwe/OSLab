
#include "type.h"
#include "const.h"
#include "protect.h"
#include "proto.h"
#include "string.h"
#include "proc.h"
#include "global.h"
#include "keyboard.h"
#include "global.h"

PUBLIC void clearScreen();

PRIVATE void set_cursor(int position);

PRIVATE void printSpecialChar(char ch);

PRIVATE void removeColorChars(char * str,int size);

PRIVATE void doQuery(int query_str_begin_index,int query_str_end_index);

PRIVATE void getStrFromDisplayMemory(char * str,int begin, int end);

PRIVATE int  strcmp(char * s1, char * s2, int cmpSize);

PRIVATE int indexOf(char * src, char *target, int targetSize);

PRIVATE void markColor(int display_begin_position,char * str,int color);

PRIVATE void recoverColor();


PRIVATE int query_mode_start_pos;
PRIVATE int has_entered_in_query_mode;
PRIVATE char text[SCREEN_SIZE];
PRIVATE char queryStr[SCREEN_SIZE];

PUBLIC void task_tty() {
    NORMAL_MODE = 1;
    query_mode_start_pos = -1;
    has_entered_in_query_mode = 0;
    clearScreen();
    int i = 0;
    while (1) {
        keyboard_read();
        //every 10ms interrupt one time,ticks++
        //if delay 100 ms,then interrupt 10 time
        milli_delay(100);
        i++;
        if (i % 800 == 0 && NORMAL_MODE) {
            clearScreen();
        }
    }
}

/**
 * process each character
 * @param key
 * @return
 */
PUBLIC void in_process(u32 key) {
    char output[2] = {'\0', '\0'};
    //if key is printable,then print it
    if (!(key & FLAG_EXT)) {
        output[0] = key & 0xFF;
        //if current mode is query mode and start position has not even been set
        if (!NORMAL_MODE && query_mode_start_pos == -1) {
            query_mode_start_pos = disp_pos;
        }

        //if current mode is query mode and has typed enter ,then do not print anything
        if (NORMAL_MODE | !has_entered_in_query_mode) {
            disp_color_str(output, NORMAL_MODE ? DEFAULT_CHAR_COLOR : QUERY_CHAR_COLOR);
            // move cursor to the current display position
            set_cursor(disp_pos / 2);
        }

    } else {
        //process unprintable character
        int raw_key = key & MASK_RAW;
        switch (raw_key) {
            case ENTER:
                if(NORMAL_MODE){
                    printSpecialChar('\n');
                }else  if (!NORMAL_MODE && has_entered_in_query_mode == 0) {
                    int curr_pos = disp_pos;
                    printSpecialChar('\n');
                    //do query
                    doQuery(query_mode_start_pos,curr_pos);
                    has_entered_in_query_mode = 1;
                }
                break;
            case BACKSPACE:
                if(disp_pos > 0){
                    printSpecialChar('\b');
                }
                break;
            case TAB:
                printSpecialChar('\t');
                break;
            case ESC:
                //second time
                if (NORMAL_MODE) {
                    output[0] = ' ';
                    int current_pos = disp_pos;
                    disp_pos = query_mode_start_pos;
                    for (int i = 0; i < (current_pos - query_mode_start_pos) / 2; ++i) {
                        disp_str(output);
                    }
                    recoverColor();
                    disp_pos = query_mode_start_pos;
                    set_cursor(disp_pos/2);
                    has_entered_in_query_mode = 0;
                    query_mode_start_pos = -1;
                }
                break;
            default:
                break;
        }
    }
}

/**
 * clear screen
 * @return
 */
PUBLIC void clearScreen() {
    disp_pos = 0;
    char output[2] = {'\0', '\0'};
    for (int i = 0; i < SCREEN_SIZE; ++i) {
        output[0] = ' ';
        disp_str(output);
    }
    disp_pos = 0;
    set_cursor(0);
}

/**
 * set cursor to specific position,based on char(2 bytes)
 * @param position
 * @return
 */
PRIVATE void set_cursor(int position) {
    disable_int();
    out_byte(CRTC_ADDR_REG, CURSOR_H);
    out_byte(CRTC_DATA_REG, (position >> 8) & 0xFF);
    out_byte(CRTC_ADDR_REG, CURSOR_L);
    out_byte(CRTC_DATA_REG, position & 0xFF);
    enable_int();
}

/**
 * remember that: disp_pos /2(based-on byte position) == cursor_position(based-on character position)
 * @param ch
 * @return
 */
PRIVATE void printSpecialChar(char ch) {
    char output[2] = {'\0', '\0'};
    switch (ch) {
        case '\n':
            disp_pos = ((disp_pos / 2) / SCREEN_WIDTH + 1) * SCREEN_WIDTH * 2;
            set_cursor(disp_pos / 2);
            break;
        case '\b':
            disp_pos -= 2;
            output[0] = ' ';
            disp_str(output);
            disp_pos -= 2;
            set_cursor(disp_pos / 2);
            break;
        case '\t':
            output[0] = ' ';
            for (int i = 0; i < 4; ++i) {
                disp_str(output);
            }
            set_cursor(disp_pos / 2);
        default:
            break;
    }
}

/**
 * mark input text with color
 * @param query_str_begin_index
 * @param query_str_end_index
 * @return
 */
PRIVATE void doQuery(int query_str_begin_index,int query_str_end_index){
    getStrFromDisplayMemory(queryStr,query_str_begin_index,query_str_end_index);
    getStrFromDisplayMemory(text,0,query_str_begin_index);
    int cmpLen = (query_str_end_index-query_str_begin_index)/2;
    int baseIndex = 0;
    int relativeIndex = 0;
    int original_disp_pos = disp_pos;
    while ((relativeIndex = indexOf(text + baseIndex, queryStr, cmpLen)) != -1) {
        markColor((baseIndex+relativeIndex)*2,queryStr,QUERY_CHAR_COLOR);
        baseIndex += (cmpLen + relativeIndex);
    }
    disp_pos = original_disp_pos;
}

/**
 * mark color for a string
 * @param display_begin_position
 * @param str
 * @param color
 * @return
 */
PRIVATE void markColor(int display_begin_position,char * str,int color){
    char output[2] = {'\0', '\0'};
    disp_pos = display_begin_position;
    disp_color_str(str,color);
}

/**
 * read display memory to main memory and remove
 * @param str
 * @param begin
 * @param end
 * @return
 */
PRIVATE void getStrFromDisplayMemory(char * str,int begin, int end) {
    read_display_memory(begin, end, str);
    removeColorChars(str,end-begin);
}

/**
 * recover input text to original color
 * @return
 */
PRIVATE void recoverColor(){
    disp_pos = 0;
    disp_str(text);
}

/**
 * remove color chars from display memory
 * @param str
 * @param size
 * @return
 */
PRIVATE void removeColorChars(char * str,int size){
    int index = 0;
    for(int i = 0; i < size;i+=2){
        str[index++] = str[i];
    }
    str[index] = '\0';
}

/**
 * compare first cmpSize of s1 and s2 ,if s1 == s2,return 0; else ,return -1
 * @param s1
 * @param s2
 * @param cmpSize
 * @return
 */
PRIVATE int  strcmp(char * s1, char * s2, int cmpSize) {
    int i = 0;
    while (s1[i] != '\0' && s2[i] != '\0' && i < cmpSize) {
        if (s1[i] != s2[i]) {
            return -1;
        }
        i++;
    }
    return 0;
}

/**
 * find the first index of target string in source string
 * @param src
 * @param target
 * @param targetSize
 * @return
 */
PRIVATE int indexOf(char * src, char *target, int targetSize) {
    int i = 0;
    while (src[i] != '\0') {
        if (strcmp(src + i, target, targetSize) == 0) {
            return i;
        }
        i++;
    }
    return -1;
}

