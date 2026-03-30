//#include <ncurses.h>
//#include <locale.h>
#include "../editor/editor.h"
#define CTRL(c) ((c) & 0x1F)
#ifndef INPUT_H
#define INPUT_H


typedef struct {

}Input;

void input_handle_key(Editor *editor, int key);


#endif //INPUT_H