#include <ncurses.h>
#include <locale.h>

#include "input.h"
#include "../editor/editor.h"
#include "../screen/window_manager.h"

//#include "../screen/screen.h"
char *tmp;
void input_handle_key(Editor *editor, Window_manager *wm, int key) {
    //TODO-HANDLE TABS and  CTRL + char
    switch (key) {
        case CTRL('w'): //TODO-CTRL +h for moving inside a file
            //TODO-READ FOR OTHER KEY AND THEN SWITCH TO TILE BASED ON DIRECTION OF KEY (UP, DOWN, LEFT, RIGHT)
            WINDOW *active_window = getActiveTileWindow(wm);
            int ch = wgetch(active_window);

            changeFocus(wm, ch);
            break;
        case CTRL('n'):
            insertTile(wm, "NEWWIN");
            break;
        case KEY_RESIZE:
            getmaxyx(stdscr, wm->screen_y, wm->screen_x);
            resizeTile(wm, 0, wm->screen_y, wm->screen_x); //TODO-RESIZE ALL TILES, NOT JUST ACTIVE TILE
            break;
        case CTRL('e'):
            prompt_save_changes(editor, wm);
            editor_shutdown(editor);
            //TODO- if no tiles left, close window and exit program
            break;
        case CTRL('s'):
            tmp=popUpWindow(wm, 3, 30, 0, 0, "Save To:");//TODO-add known file name to input window
            editor_save_file(editor, tmp);
            //file_save_as(editor, tmp, 1);
            set_tile_title(wm->active_tile, tmp);       //UPDATE TILE TITLE TO FILE NAME
            editor->changed=0; //RESET CHANGED FLAG AFTER SAVING
            
            break;
        case CTRL('o'):
            //IF EDITOR HAS CHANGES, PROMPT TO SAVE BEFORE OPENING NEW FILE
            prompt_save_changes(editor, wm);

            //RESET THE EDITOR BEFORE OPENING NEW FILE
            editor_shutdown(editor);
            editor_init(editor);

            //CREATE POPUP TILE TO OPEN FILE
            tmp=popUpWindow(wm, 3, 30, 0, 0, "Open File:");
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
                char *response=popUpWindow(wm, 3, 40, 0, 0, "Unsaved Changes! Save this file? (y/n)");
                if(response[0]=='y' || response[0]=='Y') {
                    tmp=popUpWindow(wm, 3, 30, 0, 0, "Save To:");//TODO-add known file name to input window
                    editor_save_file(editor, tmp);
                }
            }
}
