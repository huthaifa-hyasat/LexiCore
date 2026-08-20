CC = gcc
CFLAGS = -Wall -Wextra -Wpedantic -std=c11 -Iinclude

TARGET = lexicore.exe

SRC = src/main.c src/hash_table.c
OBJ = src/main.o src/hash_table.o

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) -o $(TARGET)

src/main.o: src/main.c include/hash_table.h
	$(CC) $(CFLAGS) -c src/main.c -o src/main.o

src/hash_table.o: src/hash_table.c include/hash_table.h
	$(CC) $(CFLAGS) -c src/hash_table.c -o src/hash_table.o

run: $(TARGET)
	./$(TARGET)

clean:
	del /Q src\main.o src\hash_table.o $(TARGET) 2>nul