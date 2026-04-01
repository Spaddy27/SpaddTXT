////
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
    //   buffer_insert_line(buffer, 0,0, NULL); //GIVE INITIAL LINE
}

void buffer_expand(Buffer *buf) {
    //DOUBLE THE AMOUNT OF LINES THAT CAN BE HELD
    buf->capacity *= 2;
    buf->lines = realloc(
        buf->lines,
        sizeof(char *) * buf->capacity
    );
    for (int i = buf->line_count; i < buf->capacity; i++) {
        buf->lines[i] = NULL;
    }
}

void buffer_expand_line(Buffer *buf, char *line, int row, int col, char c) {
    size_t old_len = strlen(line);
    size_t new_len = (old_len + 1) * 2;
    char *temp = realloc(line, new_len);
    if (temp != NULL) {
        line = temp;
        memset(line + old_len + 1, '\0', new_len - old_len - 1); //FILL NEW SPACE WITH \0

        memmove(&line[col + 1], //SHIFT THE LINE RIGHT
                &line[col],
                old_len - col + 1);


        line[col] = c;
        buf->lines[row] = line;
    } else {
        free(line);
    }
}

void buffer_insert_line(Buffer *buf, int row, int col, const char *text) {
    //CHECK IF FULL
    if (buf->line_count + 1 > buf->capacity - 1) {
        buffer_expand(buf);
    }
    char *currentLine = buffer_get_line(buf, row); //GET THE INSERTION POINT

    size_t textLen = 0;
    if (text) {
        textLen = strlen(text);
    }
    size_t lineLen = 0;


    if (currentLine == NULL) {
        //IF BEING INSERTED ON NULL LINE
        free(currentLine);
        currentLine = calloc(textLen + 2, sizeof(char));
        if (textLen > 0) {
            memcpy(currentLine, text, textLen);
            if (text[textLen - 1] != '\n') {
                currentLine[textLen] = '\n'; //ENSURE LINE BREAK
            }
        }
        buf->lines[row] = currentLine;
        buf->line_count++;
    } else {
        //INSERTING ON ALLOCATED LINE
        lineLen = strlen(currentLine);
        // size_t lineLen = strlen(currentLine);
        if (col == 0) {
            //IF INSERTING AT START OF LINE
            if (row <= buf->line_count) //IF LINE IS NOT AT THE END, SHIFT ALL OTHER LINES
                memmove(&buf->lines[row] + 1, &buf->lines[row], sizeof(char *) * (buf->line_count - row));
            if (textLen > 0) {
                char *newLine = calloc(textLen + 2, sizeof(char));
                memcpy(newLine, text, textLen);
                if (text[textLen - 1] != '\n') {
                    newLine[textLen] = '\n'; //ENSURE LINE BREAK
                }
                buf->lines[row] = newLine;
                buf->line_count++;
            }
        } else if (lineLen - 1 - col == 0) {
            //IF INSERTING AT END OF LINE

            if (row < buf->line_count) //IF LINE IS NOT AT THE END, SHIFT ALL OTHER LINES
                memmove(&buf->lines[row + 2], &buf->lines[row + 1], sizeof(char *) * (buf->line_count - row));

            if (textLen > 0) {
                char *newLine = calloc(textLen + 2, sizeof(char));
                memcpy(newLine, text, textLen);
                if (text[textLen - 1] != '\n') {
                    newLine[textLen] = '\n'; //ENSURE LINE BREAK
                }
                buf->lines[row + 1] = newLine;
                buf->line_count++;
            }
        } else {
            //TODO-handle full line insertion/wrapping for copy paste
            //IF INSERTING AT MIDDLE OF LINE
            if (row < buf->line_count) //IF LINE IS NOT AT THE END, SHIFT ALL OTHER LINES
                memmove(&buf->lines[row + 2], &buf->lines[row + 1], sizeof(char *) * (buf->line_count - row));

            if (textLen > 0) {
                char *newLine = calloc(textLen + lineLen + 2, sizeof(char));
                memcpy(newLine, &currentLine[col], lineLen - col);
                memset(currentLine + col, '\0', lineLen - col);
                currentLine[col] = '\n';
                buf->lines[row + 1] = newLine;
                buf->line_count++;
            }
        }
    }
}

void buffer_delete_char(Buffer *buf, int row, int col) {
    char *line = buffer_get_line(buf, row); //corresponding line in the buffer
    size_t lineLen = strlen(line);

    if (col == 0) {
        if (row == 0)return;

        char *prevLine = buffer_get_line(buf, row - 1);
        size_t prevLen = strlen(prevLine);

        if (/*lineLen == 1 && */line[0] == '\n') {
            free(line); // remove the empty line
            //  line=NULL;
        } else {
            char *newLine = calloc(prevLen + lineLen + 2, sizeof(char));

            memcpy(newLine, prevLine, prevLen - 1);
            memcpy(&newLine[prevLen - 1], line, lineLen);

            free(prevLine);
            free(line);

            buf->lines[row - 1] = newLine;
        }

        memmove(&buf->lines[row],
                &buf->lines[row + 1],
                sizeof(char *) * (buf->line_count - row));

        buf->line_count--;
    } else {

        memmove(&line[col - 1], //SHIFT THE LINE LEFT
                &line[col],
                strlen(line) - col + 1); //CHANGED TO -1*/

        buf->lines[row] = line;
    }
}

void buffer_insert_char(Buffer *buf, int row, int col, char c) {
    char *line = buffer_get_line(buf, row); //find the corresponding line in the buffer

    if (line == NULL) {
        buffer_insert_line(buf, row, col, &c);
        return;
    } else if ((strlen(line) + 2) * sizeof(char) >= sizeof(*line)) {
        buffer_expand_line(buf, line, row, col, c);
    } else {
        //SHIFT THE LINE RIGHT
        memmove(&line[col + 1],
                &line[col],
                strlen(line) - col + 1);

        line[col] = c; //INSERT THE CHAR
        buf->lines[row] = line;
    }
}

char *buffer_get_line(Buffer *buf, int index) {
    char *line = buf->lines[index];
    return line;
}


void buffer_free(Buffer *buffer) {
    for (int i = 0; i < buffer->line_count - 1; i++) {
        if (buffer->lines[i] != NULL)
            free(buffer->lines[i]);
    }
    if (buffer->lines != NULL)
        free(buffer->lines);
}
