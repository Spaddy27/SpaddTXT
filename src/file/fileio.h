//
// Created by mspad on 3/29/2026.
//




#include <stdio.h>

#ifndef FILEIO_H

#define FILEIO_H


#include "../editor/editor.h"



void file_open(Editor *editor, const char *filename, int format);

void file_save(Editor *editor,int format);

void file_save_as(Editor *editor, const char *filename, int format);

#endif //FILEIO_H
