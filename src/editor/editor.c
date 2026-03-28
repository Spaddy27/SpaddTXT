//
// Created by mspad on 3/24/2026.
//

#include "editor.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <locale.h>

void editor_init(Editor *editor) {
    editor->cursor_x = 0;
    editor->cursor_y = 0;


    editor->y_offset = 0;
    editor->x_offset = 0;

    editor->running = 1;


    editor->buffer = calloc(1, sizeof(Buffer));
    buffer_init(editor->buffer);
}

void editor_shutdown(Editor *editor) {
    //TODO-free buffer?
    editor->running = 0;
}

void editor_open_file(Editor *editor, const char *filename) {
}


void editor_insert_char(Editor *editor, char c) {
    buffer_insert_char(editor->buffer, editor->cursor_y, editor->cursor_x, c);
    editor->cursor_x++;
}

void editor_delete_char(Editor *editor) {
    buffer_delete_char(editor->buffer, editor->cursor_y, editor->cursor_x);
    editor->cursor_x--;
}



void editor_insert_line(Editor *editor) {
    editor->cursor_y++;
    editor->cursor_x = 0;
    buffer_insert_line(editor->buffer, editor->cursor_y, '\0');
}

void editor_delete_line(Editor *editor) {}

void editor_move_cursor(Editor *editor, int x, int y) {
    //TODO-prevent illegal moves
    //IF X TRIES TO GO BELOW 0, WRAP TO END OF PREV LINE


    /*char *targetLine=buffer_get_line(editor->buffer,y);
 //   char *prevLine= buffer_get_line(editor->buffer,y-1);
 //   char *nextLine= buffer_get_line(editor->buffer,y+1);

    if (x<0)//IF X TRIES TO GO BELOW 0, WRAP TO END OF PREV LINE(GO TO BEFORE \n?)
    {
        y--;
        if (y<0)return;

         targetLine=buffer_get_line(editor->buffer,y);
        if (targetLine==NULL)return;
        editor->cursor_x = strlen(targetLine)-1;
        editor->cursor_y = y;
        return;
    }*/
    /*if (x<0&&y>0) {//IF X TRIES TO GO BELOW 0, WRAP TO END OF PREV LINE(GO TO BEFORE \n?)


        if (prevLine==NULL)return;
        editor->cursor_x = strlen(prevLine);
        editor->cursor_y --;
       // return;
    }
    else if (x>=) {//IF X IS PAST END OF LINE, WRAP TO START OF NEXT LINE IF THERE IS ONE

        if (prevLine==NULL)return;
        editor->cursor_x = strlen(prevLine);
        editor->cursor_y ++;
    }*/
    //if (y<0) return; //IF Y TRIES TO GO BELOW 0, BLOCK ACTION
    //E
    editor->cursor_x = x;
    editor->cursor_y = y;
}



