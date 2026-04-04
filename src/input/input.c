#include <ncurses.h>
#include <locale.h>

#include "input.h"
#include "../editor/editor.h"

//#include "../screen/screen.h"
char *tmp;
void input_handle_key(Editor *editor, Window_manager *wm, int key) {
    //TODO-HANDLE TABS and  CTRL + char
    switch (key) {
        case CTRL('e'):
            prompt_save_changes(editor, wm);
            editor_shutdown(editor);
            //TODO- if no tiles left, close window and exit program
            break;
        case CTRL('s'):
            tmp=popUpWindow(wm, 3, 30, 5, 5, "Save To:");//TODO-add known file name to input window
            file_save_as(editor, tmp, 1);
            break;
        case CTRL('o'):
        //IF EDITOR HAS CHANGES, PROMPT TO SAVE BEFORE OPENING NEW FILE
           prompt_save_changes(editor, wm);

            //RESET THE EDITOR BEFORE OPENING NEW FILE
            editor_shutdown(editor);
            editor_init(editor);
            
        //CREATE POPUP TILE TO OPEN FILE
            tmp=popUpWindow(wm, 3, 30, 5, 5, "Open File:");
            editor_open_file(editor, tmp);
            //CHANGE TITLE OF TILE TO FILE NAME
            set_tile_title(wm->active_tile, tmp);
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
void prompt_save_changes(Editor *editor, Window_manager *wm) {
    if(editor->changed==1) {
                char *response=popUpWindow(wm, 3, 40, 5, 5, "Unsaved Changes! Save this file? (y/n)");
                 refresh();
                if(response[0]=='y' || response[0]=='Y') {
                    tmp=popUpWindow(wm, 3, 30, 5, 5, "Save To:");//TODO-add known file name to input window
                    file_save_as(editor, tmp, 1);
                    refresh();
                }
            }
}
