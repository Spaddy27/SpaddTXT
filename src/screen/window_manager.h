//TILE- an editor, a window, its size, and its position
//WINDOW_MANAGER- a collection of windows and their positions, add or delete tiles, move + organize tiles
#include <ncurses.h>
#include "../editor/editor.h"
#include "tile.h"
#ifndef WINDOW_MANAGER_H
#define WINDOW_MANAGER_H

typedef struct{
Tile *tiles[5];
Tile *active_tile;
int window_count;
int screen_x;
int screen_y;
int running;
}Window_manager;

//void init_window_manager(Window_manager *wm);
//WINDOW *add_window(Window_manager *wm, int height, int width, int starty, int startx);

//void resize_tile(Window_manager *wm, int index, int new_height, int new_width);

//void move_tile(Window_manager *wm, int index, int new_starty, int new_startx);

//void pair_tile(WINDOW *window,Tile *tile);

#endif //SCREEN_H