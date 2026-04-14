//
// Created by mspad on 3/24/2026.
//

//#include "screen.h"
#include <string.h>
#include <ncurses.h>
#include "tile.h"
//TILE- an editor, a window, its size, and its position
//WINDOW_MANAGER- a collection of windows and their positions, add or delete tiles, move + organize tiles

Tile * init_tile( int height, int width, int y, int x, const char *title) {

    noecho(); /* Don't echo() while we do getch */
    cbreak(); /* Line buffering disabled, Pass on everty thing to me */

//TILE
    Tile *tile=calloc(1, sizeof(Tile));
    tile->width = width;
    tile->height = height;
    tile->x = x;
    tile->y = y;
    tile->title = calloc(31, sizeof(char)); //GIVE MAX TITLE LENGTH OF 30 + NULL TERMINATOR
    
    
//EDITOR   
    tile->editor=calloc(1, sizeof(Editor));
    editor_init(tile->editor);
    tile->editor->screen_x = width-2;   //ACCOUNT FOR// 
    tile->editor->screen_y = height-2;      //BORDER//


set_tile_title(tile, tile->title);


  
//WINDOW
    tile->window = newwin(tile->height, tile->width, tile->y, tile->x);
    noecho(); /* Don't echo() while we do getch */
    cbreak(); /* Line buffering disabled, Pass on everty thing to me */
    tile->isActive=1;
    
    init_pair(1, COLOR_RED, COLOR_BLACK);
    wattron(tile->window, COLOR_PAIR(1));
    box(tile->window, 0, 0);
    wattroff(tile->window, COLOR_PAIR(1)); 
    mvwprintw(tile->window, 0, 2, tile->title);
    keypad(tile->window, TRUE);
  
    
    
    tile_render(tile);
   
     return tile;
     
}

WINDOW *get_tile_window(Tile *tile) {
    return tile->window;
}
Editor *get_tile_editor(Tile *tile) {
    return tile->editor;
}

void set_tile_title(Tile *tile, const char *title) {
    //CLEAR PREVIOUS 
    memset(tile->title, '\0', 31);
    
  if(tile->editor->filename) {
    if(strlen(tile->editor->filename)>30) {//ONLY COPY FIRST 30 CHARACTERS IF TITLE TOO LONG
        strncpy(tile->title, tile->editor->filename, 30);
        tile->title[30] = '\0'; //ENSURE NULL TERMINATION
    }
    else
    strcpy(tile->title, tile->editor->filename);
}
//REDRAW TITLE AND BOX
    wclear(tile->window);
    box(tile->window, 0, 0);
    mvwprintw(tile->window, 0, 2, tile->title);
    wrefresh(tile->window);

}
void change_editor(Tile *tile, Editor *new_editor) {
    tile->editor = new_editor;
    set_tile_title(tile, tile->title);
    tile_render(tile);
}
void tile_render(Tile *tile) {
   
    Editor *editor = tile->editor;
    if (!editor)
        return;
   
    _scroll(editor);

    tile_draw_rows(tile);
    if(tile->isActive)
{
        init_pair(1, COLOR_RED, COLOR_BLACK);
        wattron(tile->window, COLOR_PAIR(1));
        box(tile->window, 0, 0);
        wattroff(tile->window, COLOR_PAIR(1));     
        mvwprintw(tile->window, 0, 2, tile->title);
}

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

