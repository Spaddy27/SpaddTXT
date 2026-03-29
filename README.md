# SpaddTXT Text Editor

A tui text editor written in C using ncurses.



## Features (Current)

- ncurses terminal UI
- Dynamic line buffer
- Character insertion/deletion
- Horizontal scrolling for long lines
- Basic arrow key navigation

## Planned Features

- File opening/saving
- Line insertion/deletion
- Keyboard shortcuts
- Clipboard
- Search functionality
- Undo/redo system

## Next Commit Goals
- Allow line inserting between lines and in middle of line
- Add bounds to cursor and implement wrapping to next or previous line

## Build

Requires:

- gcc
- make
- ncurses

Build with:

make

Run:

./SpaddTxT

## Goals of This Project

This project is intended to deepen understanding of:

- low-level memory management in C
- terminal UI programming
- dynamic data structures
- editor architecture
