#include <ncurses.h>
#include "window_manager.h"
#include "../editor/editor.h"
#include "../screen/screen.h"
#include "tile.h"

void init_window_manager(Window_manager *wm) {
    wm->running = 1;
    wm->window_count = 1;
   
    getmaxyx(stdscr, wm->screen_y, wm->screen_x);
 
    //wm->tiles[0]= init_tile( wm->screen_y, wm->screen_x, 0, 0);

    wm->active_tile = init_tile( wm->screen_y, wm->screen_x, 0, 0);
   
}

