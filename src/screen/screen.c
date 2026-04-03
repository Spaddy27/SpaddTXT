//
// Created by mspad on 3/24/2026.
//

#include "screen.h"
#include <string.h>
#include <ncurses.h>
//TILE- an editor, a window, its size, and its position
//WINDOW_MANAGER- a collection of windows and their positions, add or delete tiles, move + organize tiles

void initScreen(Editor *editor) {
    getmaxyx(editor->window, editor->screen_y, editor->screen_x);
    screen_render(editor);
}

void screen_render(Editor *editor) {
    //TODO-handle issues with scrolling
  //  clear();
    editor_scroll(editor);

    screen_draw_rows(editor);

    wmove(
        editor->window,
        editor->cursor_y - editor->y_offset,
        editor->cursor_x - editor->x_offset
    );

    wrefresh(editor->window);
}

void editor_scroll(Editor *editor) {
    if (editor->cursor_x < editor->x_offset)
        editor->x_offset = editor->cursor_x;

    if (editor->cursor_x >= editor->x_offset + editor->screen_x)
        editor->x_offset = editor->cursor_x - editor->screen_x + 1;



    if (editor->cursor_y < editor->y_offset)
        editor->y_offset = editor->cursor_y;

    if (editor->cursor_y >= editor->y_offset + editor->screen_y)
        editor->y_offset = editor->cursor_y - editor->screen_y +1;
}

void screen_draw_rows(Editor *editor) {
    for (int y = 0; y < editor->screen_y; y++) {
        int file_row = y + editor->y_offset;

        wmove(editor->window, y, 0);
       // clrtoeol();

        if (file_row >= editor->buffer->line_count)
            continue;

        char *line = editor->buffer->lines[file_row];
        int len = strlen(line);

        if (len > editor->x_offset) {
            char *visible = line + editor->x_offset;

            int visible_len = len - editor->x_offset;

            if (visible_len > editor->screen_x)
                visible_len = editor->screen_x;

            mvwprintw(editor->window, y, 0, visible, visible_len);
         //   wmvwprintw(editor->window, y, visible_len, ""); //CLEAR REST OF LINE
        }
    }
}
