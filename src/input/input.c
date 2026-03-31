#include <ncurses.h>
#include <locale.h>

#include "input.h"
#include "../editor/editor.h"

#include "../screen/screen.h"

void input_handle_key(Editor *editor, int key) {
//TODO-HANDLE TABS and  CTRL + char
    switch (key) {
        case CTRL('e'):
            editor_shutdown(editor);
            break;
        case CTRL('o'):
            editor_shutdown(editor);
        break;
        case '\n':
            //TODO-shift any of the line after it to the next line
           // editor_insert_char(editor, key);
            editor_insert_line(editor,"\n");
            break;
        case KEY_BACKSPACE:
            editor_delete_char(editor);
            break;
        case KEY_LEFT:
            editor_move_cursor(editor, editor->cursor_x -1,editor->cursor_y);
            break;
        case KEY_RIGHT:
            editor_move_cursor(editor, editor->cursor_x+1,editor->cursor_y);
            break;
        case KEY_UP:
            editor_move_cursor(editor, editor->cursor_x,editor->cursor_y-1);
            break;
        case KEY_DOWN:
            editor_move_cursor(editor, editor->cursor_x,editor->cursor_y+1);
            break;
        default:
            editor_insert_char(editor, key);
    }
}
