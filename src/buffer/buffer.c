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
    buffer_insert_line(buffer, 0,0, NULL); //GIVE INITIAL LINE

}

void buffer_expand(Buffer *buf) {
    //DOUBLE THE AMOUNT OF LINES THAT CAN BE HELD
    buf->capacity *= 2;
    buf->lines = realloc(
        buf->lines,
        sizeof(char *) * buf->capacity
    );
}

void buffer_insert_line(Buffer *buf, int row, int col, const char *text) {
//TODO-implement shift rest of line

    //CHECK IF FULL
    if (buf->line_count + 1 > buf->capacity - 1) {
        buffer_expand(buf);
    }
    char *currentLine=buffer_get_line(buf, row);//GET THE LINE WHERE WILL GO
    if (row!=0)
    currentLine=buffer_get_line(buf, row-1);//GET THE LINE WHERE INSERTION IS HAPPENING



    //SHIFT ALL LINES AFTER INSERTION POINT DOWNWARDS
    if (row <= buf->line_count)         //IF LINE IS NOT AT THE END, SHIFT ALL OTHER LINES
        memmove(&buf->lines[row] + 1, &buf->lines[row], sizeof(char *) * (buf->line_count - row));

    char *newLine = buffer_get_line(buf, row);
    if (currentLine != NULL) {
        size_t lineLen = strlen(currentLine);

        if (lineLen-col!=0) {
            //Allcoate space for new line
            newLine=calloc(1,lineLen-col+1);                        //+1 to ensure null termination

            memcpy(newLine, &currentLine[col], lineLen-col);                //COPY FROM CURSOR TO END OF LINE TO NEXT LINE

            memset(currentLine + col, '\0', lineLen-col);               //set the rest of the old line to null

        }
        else {

        newLine=calloc(2,sizeof(char)); //allocate new line
        }
    }
    else {
        newLine=calloc(2,sizeof(char)); //allocate new line
        newLine[0]='\n';//INSERT LINE BREAK AT END
    }


    buf->lines[row] = newLine;
    buf->line_count++;
}

void buffer_expand_line(Buffer *buf,char *line,int row,int col,char c) {
    size_t old_len = strlen(line);
    size_t new_len = (old_len+1)*sizeof(char) * 2;
    char *temp = realloc(line, new_len );
    if (temp != NULL) {

        line = temp;
        memset(line + old_len+1, '\0', new_len - old_len-1);        //FILL NEW SPACE WITH \0

        memmove(&line[col + 1],                                            //SHIFT THE LINE RIGHT
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




    char *line =buffer_get_line(buf,row);    //find the corresponding line in the buffer

    if (line == NULL) {
        buffer_insert_line(buf, row,col, &c);
        return;
    } else if ((strlen(line) + 1)*sizeof(char) >= sizeof(*line)) {
        buffer_expand_line(buf, line, row, col, c);
    } else {
        //SHIFT THE LINE RIGHT
        memmove(&line[col + 1],
                               &line[col],
                               strlen(line) - col+1);

        line[col] = c;          //INSERT THE CHAR
        buf->lines[row] = line;
    }


}

char* buffer_get_line(Buffer *buf, int index) {
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
