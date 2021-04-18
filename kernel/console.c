#include <n7OS/console.h>
#include <inttypes.h>
#include <n7OS/cpu.h>
#include <string.h>

#define CONSOLE_LINES 25
#define CONSOLE_COLUMNS 80
#define CONSOLE_TAB 3

uint16_t * const CONSOLE_ORIGIN = (uint16_t *) 0xB8000;


int line=0;
int column=0;
uint8_t font=0xF;

typedef enum color_t {BLACK,BLUE,GREEN,CYAN,RED,MAGENTA,BROWN,GRAY,DK_GRAY,LT_BLUE,LT_GREEN,LT_CYAN,LT_RED,LT_MAGENTA,YELLOW,WHITE} color_t;

/*
* Set the text color
*/
void setcolor(color_t c){
    font &= 0xF0; // Clear the color
    font |= c; // Set the new color
}

/*
* Set the background color
*/
void setbgcolor(color_t c){
        font &= 0x8F;
        font |= c<<4;
}

/*
* Set the cursor position
*    - l : line
*    - c : column
*
* Indices are 0-based
*/
static inline void setcursor(int l, int c){
    outb(0xF,0x3D4);
    int position=l*CONSOLE_COLUMNS+c;
    outb(position & (0xFF),0x3D5);
    outb(0xE,0x3D4);
    outb((position >> 8),0x3D5);
}

/*
* Set the cursor to the next position (right to left, then top to bottom)
* If the cursor reaches the last available position, it goes
* back to its origin (0,0)
*/
static inline void nextpos(){
    if(column==(CONSOLE_COLUMNS - 1)){
        column=0;
        if(line == CONSOLE_LINES - 1){
            memcpy(CONSOLE_ORIGIN,CONSOLE_ORIGIN+CONSOLE_COLUMNS,(CONSOLE_LINES-1)*CONSOLE_COLUMNS*2);
            for(int i=0;i<CONSOLE_COLUMNS;i++){
                CONSOLE_ORIGIN[(CONSOLE_LINES-1)*CONSOLE_COLUMNS+i] = (font<<8)|' ';
            }
        }else{
            line++;
        }
    }else{
        column++;
    }
}

/*
* Print the given char
*    - c: the char to print
*/
void console_putchar(const char c){

    if(c > 31 && c < 127){
        CONSOLE_ORIGIN[line*CONSOLE_COLUMNS+column] = (font<<8)|c;
        nextpos();
    }else if(c == '\n'){
        column = 0;
        if(line < (CONSOLE_LINES - 1)){
            line++;
        }else{
            // Skip first line
            memcpy(CONSOLE_ORIGIN,CONSOLE_ORIGIN+CONSOLE_COLUMNS,(CONSOLE_LINES-1)*CONSOLE_COLUMNS*2);
            for(int i=0;i<CONSOLE_COLUMNS;i++){
                CONSOLE_ORIGIN[(CONSOLE_LINES-1)*CONSOLE_COLUMNS+i] = (font<<8)|' ';
            }
        }
    }else if(c == '\b'){
        if(column > 0){
            column--;
        }else{
            column = CONSOLE_COLUMNS-1;
            if(line>0){
                line--;
            }
            // Nothing special otherwise
        }
    }else if(c == '\t'){
        for(int j=0;j < CONSOLE_TAB;j++){
            CONSOLE_ORIGIN[line*CONSOLE_COLUMNS+column] = (font<<8)|' ';
            nextpos();
        }
    }else if(c == '\f'){
        for(int j=0;j<CONSOLE_LINES;j++){
            for(int i=0;i<CONSOLE_COLUMNS;i++){
                CONSOLE_ORIGIN[j*CONSOLE_COLUMNS+i] = (font<<8)|' ';
            }
        }
        line=0;
        column=0;
    }else if(c == '\r'){
        column=0;
    }  
    setcursor(line,column);
}

/* Print the given byte-sequence
*    -s: the bytes to print
*    -len: the number of bytes
*/
void console_putbytes(const char *s, int len) {
    setcolor(WHITE);
    setbgcolor(CYAN);
	for(int i=0;i<len;i++){
        console_putchar(s[i]);
	}
}

