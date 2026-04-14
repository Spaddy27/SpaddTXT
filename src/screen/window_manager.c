#include <ncurses.h>
#include "window_manager.h"
#include "../editor/editor.h"
//#include "../screen/screen.h"
#include "tile.h"

void init_window_manager(Window_manager *wm) {
    wm->running = 1;
    wm->window_count = 0;
    wm->capacity=5; //INITIAL CAPACITY OF 5 TILES, DOUBLE WHEN NEEDED

    Tile **tiles=calloc(5, sizeof(Tile*)); //TODO-dynamic array of tiles

    getmaxyx(stdscr, wm->screen_y, wm->screen_x);
    wm->tiles = tiles;

        
    newTile(wm, wm->screen_y, wm->screen_x, 0, 0, "untitled");
    
}

WINDOW *getActiveTileWindow(Window_manager *wm) {
    return wm->active_tile->window;
}
Editor *getActiveTileEditor(Window_manager *wm) {
    return wm->active_tile->editor;
}
void resizeTile(Window_manager *wm, int index, int new_height, int new_width) {
    Tile *tile = wm->tiles[index];
    
    if (!tile)        return;

    wclear(tile->window);
    wrefresh(tile->window); 
    wresize(tile->window, new_height, new_width);   
    tile->width = new_width;
    tile->height = new_height;
    tile->editor->screen_x = new_width-2;   //ACCOUNT FOR// 
    tile->editor->screen_y = new_height-2;      //BORDER//
 
   
    box(tile->window, 0, 0);
    mvwprintw(tile->window, 0, 2, tile->title);

  

}

void newTile(Window_manager *wm, int height, int width, int starty, int startx, const char *title) {
   
     if (wm->window_count >= wm->capacity) {
         wm->capacity *= 2;
         wm->tiles = realloc(wm->tiles, wm->capacity * sizeof(Tile*));
     }
 

    Tile *tile = init_tile(height, width, starty, startx, title);
    wm->tiles[wm->window_count] = tile;
    wm->active_tile = tile;
    wm->active_index = wm->window_count;
    wm->window_count++;
  }

void insertTile(Window_manager *wm , char direction, const char *title) {
    //GET ACTIVE TILE
    Tile *active_tile = wm->active_tile;

    if(direction=='h') {
    //DETERMINE NEW SIZE AND POSITION FOR NEW TILE
    int resize_height = active_tile->height / 2;            //TODO-ENSURE ROOM TO SPLIT TILE BEFORE RESIZING
    int newTileHeight = active_tile->height -resize_height;     //INSERTING VERTICALLY, SO NEW TILE HEIGHT IS HALF OF ACTIVE TILE HEIGHT
    int newTileStartY = active_tile->y + resize_height;         //NEW TILE STARTS BELOW ACTIVE TILE, SO START Y IS ACTIVE TILE Y + NEW TILE HEIGHT

    //RESIZE ACTIVE TILE TO MAKE ROOM FOR NEW TILE
    resizeTile(wm, wm->active_index, resize_height, active_tile->width); 
    wm->active_tile->isActive=0;
    newTile(wm, newTileHeight, active_tile->width, newTileStartY, active_tile->x, title);
    }
    else if(direction=='v') {
        //DETERMINE NEW SIZE AND POSITION FOR NEW TILE
    int resize_width = active_tile->width / 2;            //TODO-ENSURE ROOM TO SPLIT TILE BEFORE RESIZING
    int newTileWidth = active_tile->width -resize_width;     //INSERTING HORIZONTALLY, SO NEW TILE WIDTH IS HALF OF ACTIVE TILE WIDTH
    int newTileStartX = active_tile->x + resize_width;         //NEW TILE STARTS TO THE RIGHT OF ACTIVE TILE, SO START X IS ACTIVE TILE X + NEW TILE WIDTH  

    resizeTile(wm, wm->active_index, active_tile->height, resize_width);
    wm->active_tile->isActive=0;
    
    newTile(wm, active_tile->height, newTileWidth, active_tile->y, newTileStartX, title);


    }
   

}

char* popUpWindow(Window_manager *wm, int height, int width, int starty, int startx, const char *title) {

     WINDOW *popup = newwin(height, width, starty, startx);
     int *overlappingIndexes=tilesInSpace(wm, starty, startx, starty+height, startx+width);

box(popup, 0, 0);
mvwprintw(popup, 0, 2, title);
wmove(popup, 1, 1);
wrefresh(popup);

char input[256];
int i = 0;
int ch;

echo();
keypad(popup, TRUE);

while ((ch = wgetch(popup)) != '\n' && i < 255) {
    if (ch == KEY_BACKSPACE || ch == 127) {
        if (i > 0) {
            i--;
            input[i] = '\0';
            // Erase the last character on screen
            mvwaddch(popup, 1, 1 + i, ' ');
            wmove(popup, 1, 1 + i);
        }
    } else {
        input[i++] = ch;
        input[i]   = '\0';
        // Echo the character inside the border
        mvwaddch(popup, 1, i, ch);
    }
    wrefresh(popup);
}

noecho();
wclear(popup);
wrefresh(popup);
delwin(popup);

if(overlappingIndexes) {
    for(int i=0; overlappingIndexes[i] != -1; i++) {
        Tile *tile=wm->tiles[overlappingIndexes[i]];
        box(tile->window, 0, 0);
        mvwprintw(tile->window, 0, 2, tile->title);
        tile_render(tile);
    }
    free(overlappingIndexes);
}
return strdup(input);


}

int* tilesInSpace(Window_manager *wm, int starty, int startx, int endy, int endx) {
   int indexes[wm->window_count];
    int count=0;
    for(int i=0; i<wm->window_count; i++) {
        Tile *tile=wm->tiles[i];
        int tile_starty=tile->y;
        int tile_endy=tile->y + tile->height;
        int tile_startx=tile->x;
        int tile_endx=tile->x + tile->width;

        //FIND ANY TILE THAT OVERLAPS WITH THE GIVEN SPACE
        if(tile_starty < endy && tile_endy > starty && tile_startx < endx && tile_endx > startx) {
            indexes[count]=i;
            count++;
        }
    }
    if(count>0) {
        int *result=malloc((count+1) * sizeof(int));
        memcpy(result, indexes, count * sizeof(int));
        result[count] = -1;
        return result;
    }
    return NULL;
}

void changeFocus(Window_manager *wm, char direction) {
    Tile *old_active = wm->active_tile;
    int active_starty = old_active->y;
    int active_endy = old_active->y + old_active->height;
    int active_startx = old_active->x;
    int active_endx = old_active->x + old_active->width;

    switch (direction) {
        case 'u':  //UP: find tile directly above (its endy == our starty)
            if(active_starty != 0) {
                int* overlappingIndexes=tilesInSpace(wm, 0, active_startx, active_starty, active_endx);
                if(overlappingIndexes) {
                    for(int i=0; overlappingIndexes[i] != -1; i++) {
                        Tile *tile=wm->tiles[overlappingIndexes[i]];
                        if(tile->y + tile->height == active_starty) {
                            wm->active_tile = tile;
                            wm->active_index = overlappingIndexes[i];
                            break;
                        }
                    }
                    free(overlappingIndexes);
                }
            }
            break;
        case 'j':  //DOWN: find tile directly below (its starty == our endy)
            if(active_endy != wm->screen_y) {
                int* overlappingIndexes=tilesInSpace(wm, active_endy, active_startx, wm->screen_y, active_endx);
                if(overlappingIndexes) {
                    for(int i=0; overlappingIndexes[i] != -1; i++) {
                        Tile *tile=wm->tiles[overlappingIndexes[i]];
                        if(tile->y == active_endy) {
                            wm->active_tile = tile;
                            wm->active_index = overlappingIndexes[i];
                            break;
                        }
                    }
                    free(overlappingIndexes);
                }
            }
            break;
        case 'h':  //LEFT: find tile directly left (its endx == our startx)
            if(active_startx != 0) {
                int* overlappingIndexes=tilesInSpace(wm, active_starty, 0, active_endy, active_startx);
                if(overlappingIndexes) {
                    for(int i=0; overlappingIndexes[i] != -1; i++) {
                        Tile *tile=wm->tiles[overlappingIndexes[i]];
                        if(tile->x + tile->width == active_startx) {
                            wm->active_tile = tile;
                            wm->active_index = overlappingIndexes[i];
                            break;
                        }
                    }
                    free(overlappingIndexes);
                }
            }
            break;
        case 'k':  //RIGHT: find tile directly right (its startx == our endx)
            if(active_endx != wm->screen_x) {
                int* overlappingIndexes=tilesInSpace(wm, active_starty, active_endx, active_endy, wm->screen_x);
                if(overlappingIndexes) {
                    for(int i=0; overlappingIndexes[i] != -1; i++) {
                        Tile *tile=wm->tiles[overlappingIndexes[i]];
                        if(tile->x == active_endx) {
                            wm->active_tile = tile;
                            wm->active_index = overlappingIndexes[i];
                            break;
                        }
                    }
                    free(overlappingIndexes);
                }
            }
            break;
        default:
            break;
    }

    if(wm->active_tile != old_active) {
        //UNHIGHLIGHT old active tile
        old_active->isActive=0;
        box(old_active->window, 0, 0);
        mvwprintw(old_active->window, 0, 2, old_active->title);
        tile_render(old_active);

        //HIGHLIGHT NEW ACTIVE WINDOW
        wm->active_tile->isActive=1;
        // init_pair(1, COLOR_RED, COLOR_BLACK);
        // wattron(getActiveTileWindow(wm), COLOR_PAIR(1));
        // box(getActiveTileWindow(wm), 0, 0);
        // wattroff(getActiveTileWindow(wm), COLOR_PAIR(1));
        // mvwprintw(getActiveTileWindow(wm), 0, 2, wm->active_tile->title);
        //wrefresh(getActiveTileWindow(wm));
        tile_render(wm->active_tile);
       // wrefresh(getActiveTileWindow(wm));
    }
}

void shutdown_window_manager(Window_manager *wm) {
for (int i = 0; i < wm->window_count; i++) {
    delwin(wm->tiles[i]->window);
    free(wm->tiles[i]->editor);
    free(wm->tiles[i]);
}
free(wm->tiles);
wm->running = 0;



}
