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

}Tile;

//void init_tile(Tile *tile, int width, int height, int x, int y);

//void tile_render(Tile *tile, Editor *editor);

#endif //TILE_H
