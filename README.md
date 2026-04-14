# SpaddTXT Text Editor

A tui text editor written in C using ncurses.


## Features (Current)

- ncurses terminal UI
- Dynamic line buffer
- Character insertion/deletion
- Horizontal scrolling for long lines
- Arrow key navigation
- Line insertion/deletion
- File opening/saving
- Open and navigate multiple windows

## Planned Features

- Delete Windows
- Keyboard shortcuts
- Clipboard
- Search functionality
- Undo/redo system


## Next Commit Goals

- Handle deleting windows
- Handle different file formats(ex. DOS)


## Build

Requires:

- gcc
- make
- ncurses

Build with:

make

Run:

./SpaddTxT [file_name]

## Commands

Quit: CTRL+e

Open: CTRL+o

Save As: CTRL+s

New Window: CTRL+n 
 - h -> split current window and add new below
 - v -> split current window and add new to the right

Switch Window: CTRL+w
 - u -> UP 
 - j -> DOWN
 - h -> LEFT
 - k -> RIGHT

## Goals of This Project

This project is intended to deepen understanding of:

- low-level memory management in C
- terminal UI programming
- dynamic data structures
- editor architecture
