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

void insertTile(Window_manager *wm , const char *title) {
    //GET ACTIVE TILE
    Tile *active_tile = wm->active_tile;
    //DETERMINE NEW SIZE AND POSITION FOR NEW TILE
    int resize_height = active_tile->height / 2;            //TODO-ENSURE ROOM TO SPLIT TILE BEFORE RESIZING

    int newTileHeight = active_tile->height -resize_height;     //INSERTING VERTICALLY, SO NEW TILE HEIGHT IS HALF OF ACTIVE TILE HEIGHT
    int newTileStartY = active_tile->y + resize_height;         //NEW TILE STARTS BELOW ACTIVE TILE, SO START Y IS ACTIVE TILE Y + NEW TILE HEIGHT

    //RESIZE ACTIVE TILE TO MAKE ROOM FOR NEW TILE
    resizeTile(wm, wm->active_index, resize_height, active_tile->width); 

    newTile(wm, newTileHeight, active_tile->width, newTileStartY, active_tile->x, title);


   

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
    for(int i=0; i<(sizeof(overlappingIndexes)/sizeof(overlappingIndexes[0]))-1; i++) {
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
    //TODO-RETURN ARRAY OF INDEXES OF TILES THAT OVERLAP WITH GIVEN SPACE
   int indexes[wm->window_count];
    int count=0;
    for(int i=0; i<wm->window_count; i++) {
        Tile *tile=wm->tiles[i];
        int tile_starty=tile->y;
        int tile_endy=tile->y + tile->height;
        int tile_startx=tile->x;
        int tile_endx=tile->x + tile->width;

        if(tile_starty < endy && tile_endy > starty && tile_startx < endx && tile_endx > startx) {
            indexes[count]=i;
            count++;

        }
    }
    if(count>0) {
        int *result=malloc(count * sizeof(int));
        memcpy(result, indexes, count * sizeof(int));
        return result;
    }
    return NULL;
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