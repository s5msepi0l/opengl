# Define target executable name
TARGET = main

# Compiler and flags
CC = g++
CFLAGS = -std=c++17 -Wall
LDFLAGS = -lGL -lGLU -lglut -lGLEW -lglfw -lX11 -lXxf86vm -lXrandr -lpthread -lXi -ldl -lXinerama -lXcursor

# Source files (add your additional .cpp files here)
SRC_FILES = main.cpp state.cpp lib/engine/renderer.cpp lib/engine/game_engine.cpp lib/engine/world.cpp

# Object files (generated from source files)
OBJ_FILES = $(SRC_FILES:.cpp=.o)

# Default target to build the executable
all: $(TARGET)

# Rule to build the target executable
$(TARGET): $(OBJ_FILES)
	$(CC) $(OBJ_FILES) -o $(TARGET) $(CFLAGS) $(LDFLAGS)
	#@echo "Build successful, running ./$(TARGET)"
	./$(TARGET)  # Automatically execute the program after building

# Rule to compile .cpp files into .o files
%.o: %.cpp
	$(CC) $(CFLAGS) -c $< -o $@

# Clean rule to remove object files and the executable
clean:
	rm -f $(OBJ_FILES) $(TARGET)