//
// Created by mspad on 3/24/2026.
//
#include "../buffer/buffer.h"
#include <ncurses.h>



#ifndef EDITOR_H
#define EDITOR_H





typedef struct {
    int cursor_x;
    int cursor_y;

    int screen_y;
    int screen_x;

    int y_offset;
    int x_offset;

    int running;

   // WINDOW *window;
//TEST
    Buffer *buffer;

} Editor;



void editor_init(Editor *editor);
void editor_shutdown(Editor *editor);

void editor_open_file(Editor *editor, const char *filename);
void editor_save_file(Editor *editor, const char *filename);


void editor_insert_char(Editor *editor, char c);
void editor_delete_char(Editor *editor);


void editor_insert_line(Editor *editor,const char *line);
void editor_delete_line(Editor *editor);

void editor_move_cursor(Editor *editor, int x, int y);


#endif //EDITOR_H