TARGET = main
 

CC = g++
CFLAGS  = -o ${TARGET} -std=c++11 -Wall -lGL -lGLU -lglut -lGLEW -lglfw -lX11 -lXxf86vm -lXrandr -lpthread -lXi -ldl -lXinerama -lXcursor
 
all: $(TARGET)
 
$(TARGET): $(TARGET).cpp
	$(CC) -o $(TARGET) $(TARGET).cpp $(CFLAGS) 
	./$(TARGET)
 
clean:
	$(RM) $(TARGET)