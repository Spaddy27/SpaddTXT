#include <ncurses.h>
#include <locale.h>

#include "input.h"
#include "../editor/editor.h"

//#include "../screen/screen.h"

void input_handle_key(Editor *editor, Window_manager *wm, int key) {
    //TODO-HANDLE TABS and  CTRL + char
    switch (key) {
        case CTRL('e'):
            editor_shutdown(editor);
            break;
        case CTRL('o'):
        //CREATE POPUP TILE TO OPEN FILE
            char *tmp=popUpWindow(wm, 3, 30, 5, 5, "Open File:");
            editor_open_file(editor, tmp);
            //CHANGE TITLE OF TILE TO FILE NAME
            set_tile_title(wm->active_tile, tmp);
           // editor_shutdown(editor);
            break;
        case '\n':
            editor_insert_line(editor, "\n");
            break;
        case KEY_BACKSPACE:
            editor_delete_char(editor);
            break;
        case KEY_LEFT:
            editor_move_cursor(editor, editor->cursor_x - 1, editor->cursor_y);
            break;
        case KEY_RIGHT:
            editor_move_cursor(editor, editor->cursor_x + 1, editor->cursor_y);
            break;
        case KEY_UP:
            editor_move_cursor(editor, editor->cursor_x, editor->cursor_y - 1);
            break;
        case KEY_DOWN:
            editor_move_cursor(editor, editor->cursor_x, editor->cursor_y + 1);
            break;
        default:
            editor_insert_char(editor, key);
    }
}
