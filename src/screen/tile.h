//
// Created by mspad on 3/24/2026.
//
#include "../editor/editor.h"
#ifndef TILE_H
#define TILE_H
typedef struct {
    Editor *editor;
    WINDOW *window;
    int width;
    int height;
    int x;
    int y;
    char *title;
    int isActive;  

}Tile;

Tile *init_tile( int width, int height, int x, int y, const char *title);

Editor *get_tile_editor(Tile *tile);
WINDOW *get_tile_window(Tile *tile);
void resize_tile(Tile *tile, int new_height, int new_width);
void tile_render(Tile *tile);

#endif //TILE_H
