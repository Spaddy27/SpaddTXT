//#include <ncurses.h>
//#include <locale.h>
#include "../screen/window_manager.h"
#include "../editor/editor.h"
#define CTRL(c) ((c) & 0x1F)
#ifndef INPUT_H
#define INPUT_H


typedef struct {

}Input;

void input_handle_key(Editor *editor, Window_manager *wm, int key);

void prompt_save_changes(Editor *editor, Window_manager *wm);
#endif //INPUT_H