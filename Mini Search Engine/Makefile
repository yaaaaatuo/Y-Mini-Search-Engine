# Mini Search Engine Makefile

CC = gcc
CFLAGS = -Wall -Wextra -std=c99 -I./include
LDFLAGS =

SRC_DIR = src
OBJ_DIR = obj
BIN_DIR = bin

SOURCES = $(SRC_DIR)/main.c \
          $(SRC_DIR)/file_scanner.c \
          $(SRC_DIR)/tokenizer.c \
          $(SRC_DIR)/index.c \
          $(SRC_DIR)/search.c \
          $(SRC_DIR)/interactive.c

OBJECTS = $(SOURCES:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o)

TARGET = $(BIN_DIR)/search_engine

.PHONY: all clean directories

all: directories $(TARGET)

directories:
	@mkdir -p $(OBJ_DIR) $(BIN_DIR)

$(TARGET): $(OBJECTS)
	$(CC) $(OBJECTS) -o $@ $(LDFLAGS)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -rf $(OBJ_DIR) $(BIN_DIR)

run: all
	./$(TARGET) data

# Windows specific
windows: CC = gcc
windows: directories $(TARGET)
