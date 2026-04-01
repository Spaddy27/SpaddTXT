//
// Created by mspad on 3/29/2026.
//

#include "fileio.h"
#include "stdlib.h"
#include "../editor/editor.h"


void file_open(Editor *editor, const char *filename, int format){

  char *line = NULL; // Initialize line pointer to NULL for automatic allocation
    size_t len = 0;    // Initialize size to 0
    ssize_t read;      // Variable to store the number of characters read

  FILE *fp = fopen(filename, "r");

  if (fp == NULL) {
        perror("Error opening file");
      //  free(line);
       // free(fp);
        return ;
    }
  while ((read = getline(&line, &len, fp)) != -1) {
        editor_insert_line(editor, line);

    }

	editor_move_cursor(editor, 0, 0);

    if (line) {
        free(line);
    }


  fclose(fp);
 // free(fp);

  }

void file_save(Editor *editor, int format){

}
void file_save_as(Editor *editor, const char *filename,int format){

  Buffer *buffer = editor->buffer;
  FILE *fp = fopen(filename, "w");



  //if (!fp) {}
  for( int i = 0; i < buffer->capacity ; i++) {
    char *line = buffer_get_line(buffer, i);
    if (line)
        fputs(line,fp);
 }

  fclose(fp);
}