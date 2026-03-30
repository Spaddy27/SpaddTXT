//
// Created by mspad on 3/24/2026.
//

#ifndef BUFFER_H
#define BUFFER_H
typedef struct {
    char **lines;
    int line_count;
    int capacity;
}Buffer;

void buffer_init(Buffer *buffer);

void buffer_insert_char(Buffer *buffer, int row, int col, char c);
void buffer_delete_char(Buffer *buffer, int row, int col);


void buffer_insert_line(Buffer *buffer, int index,int col,const char *text);
void buffer_delete_line(Buffer *buffer, int index,int col);
char* buffer_get_line(Buffer *buffer,int index);

void buffer_free(Buffer *buffer);

#endif //BUFFER_H
