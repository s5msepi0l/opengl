TARGET = main

CC = g++
CFLAGS = -std=c++17 -Wall
LDFLAGS = -lGL -lGLU -lglut -lGLEW -lglfw -lX11 -lXxf86vm -lXrandr -lpthread -lXi -ldl -lXinerama -lXcursor

SRC_FILES = main.cpp state.cpp lib/engine/world.cpp lib/engine/renderer.cpp lib/engine/game_engine.cpp

OBJ_FILES = $(SRC_FILES:.cpp=.o)

all: $(TARGET)

$(TARGET): $(OBJ_FILES)
	$(CC) $(OBJ_FILES) -o $(TARGET) $(CFLAGS) $(LDFLAGS)
	#@echo "Build successful, running ./$(TARGET)"
	./$(TARGET)  # Automatically execute the program after building

%.o: %.cpp
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJ_FILES) $(TARGET)