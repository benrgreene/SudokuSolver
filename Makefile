CC=gcc
CFLAGS=-o
SOURCES=sudoku.c utilities.c

sudoku : $(SOURCES)
	$(CC) $(SOURCES) $(CFLAGS) sudoku