//
// Created by mspad on 3/24/2026.
//

#include "editor.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <locale.h>
#include "../file/fileio.h"

void editor_init(Editor *editor) {
    editor->cursor_x = 0;
    editor->cursor_y = 0;


    editor->y_offset = 0;
    editor->x_offset = 0;

    editor->running = 1;


    editor->buffer = calloc(1, sizeof(Buffer));
    buffer_init(editor->buffer);

//    editor_open_file(editor, "src/buffer/buffer.c");
        editor_open_file(editor, "test.c");

}

void editor_shutdown(Editor *editor) {
    //TODO-free buffer?
    file_save_as(editor, "SpaddTxTOUT.txt", 1); //HARDCODED OUTPUT
    editor->running = 0;
    buffer_free(editor->buffer);
  //    buffer_free(editor->buffer);
    if (editor->buffer)
        free(editor->buffer);
}

void editor_open_file(Editor *editor, const char *filename) {
    file_open(editor, filename, 1);
}


void editor_insert_char(Editor *editor, char c) {
    buffer_insert_char(editor->buffer, editor->cursor_y, editor->cursor_x, c);
    editor->cursor_x++;
}

void editor_delete_char(Editor *editor) {
    size_t origLen = strlen(buffer_get_line(editor->buffer, editor->cursor_y));
    buffer_delete_char(editor->buffer, editor->cursor_y, editor->cursor_x);
    editor->cursor_x--;


    //HANDLE BACK WRAP
    if (editor->cursor_x < 0) {
        editor->cursor_y--;
        if (editor->cursor_y < 0) {
            editor->cursor_y = 0;
            editor->cursor_x = 0;
            return;
        }

        char *line = editor->buffer->lines[editor->cursor_y];
        size_t len = strlen(line);
        editor->cursor_x = len - origLen;
    }
}


void editor_insert_line(Editor *editor, const char *line) {
    buffer_insert_line(editor->buffer, editor->cursor_y, editor->cursor_x, line);
    editor->cursor_y++;
    editor->cursor_x = 0;
}

void editor_delete_line(Editor *editor) {
}

void editor_move_cursor(Editor *editor, int x, int y) {
    char *currentLine = buffer_get_line(editor->buffer, editor->cursor_y);


    if (y < 0 || y >= editor->buffer->line_count)return; //prevent going to non-existent line
    char *targetLine = buffer_get_line(editor->buffer, y);
    if (x < 0)                                              //IF X TRIES TO GO BELOW 0, WRAP TO END OF PREV LINE
    {
        y--;
        if (y < 0)return;

        targetLine = buffer_get_line(editor->buffer, y);
        if (targetLine == NULL) { return; }
        editor->cursor_x = strlen(targetLine) - 1;
        editor->cursor_y = y;
        return;
    }
                            //IF X IS PAST END OF LINE, WRAP TO START OF NEXT LINE IF THERE IS ONE
        if (x >= strlen(targetLine)) {                          //IF X IS PAST END OF LINE, WRAP TO START OF NEXT LINE IF THERE IS ONE


            if (targetLine == currentLine) {
                if (!buffer_get_line(editor->buffer, y + 1))return;
                editor->cursor_x = 0;
                editor->cursor_y = y + 1;
            } else {                                            //GO TO MAX LINE LENGTH UPON VERTICAL MOVE PAST END OF LINE
                editor->cursor_x = strlen(targetLine) - 1;
                editor->cursor_y = y;
            }
            return;
        }

        editor->cursor_x = x;
        editor->cursor_y = y;
    }

