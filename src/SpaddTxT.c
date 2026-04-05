#include <ncurses.h>
#include <locale.h>

#include "editor/editor.h"
#include "input/input.h"
//#include "screen/screen.h"
#include "screen/window_manager.h"
#include "screen/tile.h"
#include <stdio.h>


int main(int argc, char *argv[]) {
    //INTITIALIZATION FOR NCURSES
    setlocale(LC_ALL, "");
    initscr();
    raw(); 
    noecho(); /* Don't echo() while we do getch */
    cbreak(); /* Line buffering disabled, Pass on everty thing to me */
    keypad(stdscr, TRUE); /* Enable function keys like F1, arrow keys, etc. */
    //scrollok(stdscr, TRUE); //ENABLE BOTTOM OF WINDOW SCROLL
     
    //CREATE WINDOW MANAGER
    Window_manager wm;
    WINDOW *active_window=NULL;
    Tile *active_tile=NULL;
    Editor *active_editor=NULL;

    init_window_manager(&wm);
    
    active_editor=getActiveTileEditor(&wm);
     active_window=getActiveTileWindow(&wm);
     active_tile=wm.active_tile;

//HANDLE ARGUMENTS TO OPEN FILES IN NEW TILES
    for(int i=1; i<argc; i++) {
        char *filename=argv[i];
        editor_open_file(active_editor, filename);
        set_tile_title(active_tile, filename);
        tile_render(active_tile);
    }     
   

    //TODO-handle changing windows and active tiles
    //TODO-handle when editor is closed, close tile, and if no tiles left, close window and exit program
    while(active_editor->running) { 
        active_editor=getActiveTileEditor(&wm);
     active_window=getActiveTileWindow(&wm);
     active_tile=wm.active_tile;
        int ch=wgetch(active_window);
        input_handle_key(active_editor, &wm,ch);
        if(active_editor->running==0) {     //CHECK TO ENSURE STILL RUNNING AFTER INPUT
            break;
        }
        tile_render(active_tile);
    }
        

    shutdown_window_manager(&wm);

    endwin(); /* End curses mode and restore terminal */
   // delwin();
    return 0;
}
