#include <ncurses.h>
#include "window_manager.h"
#include "../editor/editor.h"
//#include "../screen/screen.h"
#include "tile.h"

void init_window_manager(Window_manager *wm) {
    wm->running = 1;
    wm->window_count = 0;
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
    if (wm->window_count >= 5) {
        //TODO-handle max window count
        return;
    }
    Tile *tile = init_tile(height, width, starty, startx, title);
    wm->tiles[wm->window_count] = tile;
    wm->active_tile = tile;
    wm->window_count++;
  }

char* popUpWindow(Window_manager *wm, int height, int width, int starty, int startx, const char *title) {
//     //TODO-create a new temporary window
     WINDOW *popup = newwin(height, width, starty, startx);
//     box(popup, 0, 0);
//     mvwprintw(popup, 0, 2, title);
//     wrefresh(popup);
//     //Create input buffer
//     char input[256];
//     //Get user input
//  //   wmvwgetnstr(popup, input, 255);
//     mvwgetnstr(popup,1, 0, input, 255);
//     //Destroy the popup window
//     delwin(popup);
//     //Return the user input
//     return strdup(input);
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
delwin(popup);
return strdup(input);


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