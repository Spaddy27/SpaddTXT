//
// Created by mspad on 3/24/2026.
//
#include <stdlib.h>
#include <string.h>
#include "buffer.h"
#include <stdio.h>
//TEST
void buffer_init(Buffer *buffer) {
    buffer->capacity = 8; //Amount of lines that can be held
    buffer->line_count = 0; //amount of lines with text
    buffer->lines = calloc(buffer->capacity, sizeof(char *));
    buffer_insert_line(buffer, 0, NULL);
}

void buffer_expand(Buffer *buf) {
    //DOUBLE THE AMOUNT OF LINES THAT CAN BE HELD
    buf->capacity *= 2;
    buf->lines = realloc(
        buf->lines,
        sizeof(char *) * buf->capacity
    );
}

void buffer_insert_line(Buffer *buf, int row, const char *text) {
//TODO-implement shift rest of line

    //CHECK IF FULL
    if (buf->line_count + 1 >= buf->capacity - 1) {
        buffer_expand(buf);
    }
    //SHIFT ALL LINES AFTER INSERTION POINT DOWNWARDS
    if (row <= buf->line_count) //IF LINE IS NOT AT THE END OF PROGRAM, SHIFT ALL OTHER LINES
        memmove(buf->lines[row] + 1, buf->lines[row], sizeof(char *) * (buf->line_count - row));

    char *line = buf->lines[row];
    if (text != NULL) {
        line = calloc(1, sizeof(char) * strlen(text) + 1);

        line = memcpy(line, text, strlen(text) * sizeof(char));
    } else {
        line = calloc(1, sizeof(char));
    }

    buf->lines[row] = line;
    buf->line_count++;
}

void buffer_expand_line(Buffer *buf,char *line,int row,int col,char c) {
    size_t old_len = strlen(line);
    size_t new_len = (old_len+1)*sizeof(char) * 2;
    char *temp = realloc(line, new_len );
    if (temp != NULL) {

        line = temp;
        memset(line + old_len+1, '\0', new_len - old_len-1);        //FILL NEW SPACE WITH \0
        //SHIFT THE REST OF THE MEMORY
        memmove(&line[col + 1],                                            //SHIFT THE MEMORY DOWN
                   &line[col],
                   old_len - col+1);


        line[col] = c;
        buf->lines[row] = line;
    } else {
        free(line);
    }
}
void buffer_delete_char(Buffer *buf, int row, int col) {
    char *line = buf->lines[row]; //corresponding line in the buffer
    //TODO-handle with start of line delete
    memmove(line + col - 1, line + col, strlen(line) - 1);


    buf->lines[row] = line;
}

void buffer_insert_char(Buffer *buf, int row, int col,  char c) {




    char *line = buf->lines[row];                                              //find the corresponding line in the buffer

    if (line == NULL) {
        buffer_insert_line(buf, row, &c);
        return;
    } else if ((strlen(line) + 1)*sizeof(char) >= sizeof(*line)) {
        buffer_expand_line(buf, line, row, col, c);
    } else {
        //TODO-handle insertions not at the end of the line
        memmove(&line[col + 1],
                               &line[col],
                               strlen(line) - col+1);

        line[col] = c;
        buf->lines[row] = line;
    }


}

char* buffer_get_line(Buffer *buf, int index) {
    char *line = buf->lines[index];
    return line;

}


void buffer_free(Buffer *buffer) {
    //TODO- figure out why  this results in a segmentation fault
    for (int i = 0; i < buffer->line_count - 1; i++) {
        if (buffer->lines[i] != NULL)
            free(buffer->lines[i]);
    }
    if (buffer->lines != NULL)
        free(buffer->lines);
}
