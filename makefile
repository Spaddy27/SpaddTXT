# Compiler
CC = gcc

# Compiler flags
CFLAGS = -Wall -g

# Libraries
LIBS = -lncurses

# Directories
SRC_DIR = src
BUILD_DIR = build

# Find all source files
SRCS = $(shell find $(SRC_DIR) -name "*.c")


SRCS := $(filter-out screen.c, $(SRCS))

# Convert source files to object files in build directory
OBJS = $(patsubst $(SRC_DIR)/%.c,$(BUILD_DIR)/%.o,$(SRCS))

# Executable name
TARGET = SpaddTxT

# Default target
all: $(TARGET)

# Link object files
$(TARGET): $(OBJS)
	$(CC) $(OBJS) -o $(TARGET) $(LIBS)

# Compile source files
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c
	mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

# Clean build files
clean:
	rm -rf $(BUILD_DIR) $(TARGET)

# Rebuild everything
rebuild: clean all

# Run the editor
run: $(TARGET)
	./$(TARGET)
