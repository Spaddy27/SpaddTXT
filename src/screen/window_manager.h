//TILE- an editor, a window, its size, and its position
//WINDOW_MANAGER- a collection of windows and their positions, add or delete tiles, move + organize tiles
#include <ncurses.h>
#include "../editor/editor.h"
#include "tile.h"
#ifndef WINDOW_MANAGER_H
#define WINDOW_MANAGER_H

typedef struct{
  //  struct Student *students = malloc(n * sizeof(struct Student));
  Tile **tiles; //dynamic array of tiles


Tile *active_tile;
int active_index;
int capacity; //for dynamic array of tiles, start with capacity of 5 and double when needed
int window_count;
int screen_x;
int screen_y;
int running;
}Window_manager;
WINDOW *getActiveTileWindow(Window_manager *wm);
Editor *getActiveTileEditor(Window_manager *wm);
int* tilesInSpace(Window_manager *wm, int starty, int startx, int endy, int endx);
void newTile(Window_manager *wm, int height, int width, int starty, int startx, const char *title);

char* popUpWindow(Window_manager *wm, int height, int width, int starty, int startx, const char *title);


void resizeTile(Window_manager *wm, int index, int new_height, int new_width);

void moveTile(Window_manager *wm, int index, int new_starty, int new_startx);

void shutdown_window_manager(Window_manager *wm);

#endif //SCREEN_H