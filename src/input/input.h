//#include <ncurses.h>
//#include <locale.h>
#include "../editor/editor.h"
#define CTRL_KEY(k) ((k) & 0x1f)
#ifndef INPUT_H
#define INPUT_H


typedef struct {

}Input;

void input_handle_key(Editor *editor, int key);


#endif //INPUT_H