//
// Created by mspad on 3/24/2026.
//

//#include "screen.h"
#include <string.h>
#include <ncurses.h>
#include "tile.h"
//TILE- an editor, a window, its size, and its position
//WINDOW_MANAGER- a collection of windows and their positions, add or delete tiles, move + organize tiles

Tile * init_tile( int height, int width, int y, int x) {

    Tile *tile=calloc(1, sizeof(Tile));
    
    tile->editor=calloc(1, sizeof(Editor));
     editor_init(tile->editor);
    
    tile->width = width;
    
    tile->height = height;
    tile->x = x;
    tile->y = y;

    tile->editor->screen_x = width-2;//ACCOUNT FOR BORDER
    tile->editor->screen_y = height-2;
  

    tile->window = newwin(tile->height, tile->width, tile->y, tile->x);
     box(tile->window, 0, 0);
     mvwprintw(tile->window, 0, 2, "Tile");
    keypad(tile->window, TRUE);
  
     tile_render(tile);
   //  int key = wgetch(tile->window);
     return tile;
     //GE
  //    int key = wgetch(tile->window);
}



void tile_render(Tile *tile) {
    //TODO-handle issues with scrolling
    Editor *editor = tile->editor;
   // wclear(tile->window);
    _scroll(editor);

    tile_draw_rows(tile);

    wmove(
        tile->window,
        editor->cursor_y - editor->y_offset +1,
        editor->cursor_x - editor->x_offset +1
    );

    wrefresh(tile->window);
}

void _scroll(Editor *editor) {
    if (editor->cursor_x < editor->x_offset)
        editor->x_offset = editor->cursor_x;

    if (editor->cursor_x >= editor->x_offset + editor->screen_x)
        editor->x_offset = editor->cursor_x - editor->screen_x + 1;



    if (editor->cursor_y < editor->y_offset)
        editor->y_offset = editor->cursor_y;

    if (editor->cursor_y >= editor->y_offset + editor->screen_y)
        editor->y_offset = editor->cursor_y - editor->screen_y +1;
}

void tile_draw_rows(Tile *tile) {
    Editor *editor = tile->editor;
    for (int y = 0; y < editor->screen_y; y++) {
        int file_row = y + editor->y_offset;

      // wmove(tile->window, y+1, 1);
    //   wclrtoeol(tile->window);
     mvwhline(tile->window, y + 1, 1, ' ', editor->screen_x);
        if (file_row >= editor->buffer->line_count)
            continue;

        char *line = editor->buffer->lines[file_row];
        int len = strlen(line);

        if (len > editor->x_offset) {
            char *visible = line + editor->x_offset ;

            int visible_len = len - editor->x_offset -1;

            if (visible_len > editor->screen_x)
                visible_len = editor->screen_x;

           
             mvwaddnstr(tile->window, y + 1, 1, visible, visible_len);
        
        }
    }
}

