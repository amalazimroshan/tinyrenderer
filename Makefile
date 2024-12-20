# Compiler
CXX = g++

# Compiler Flags
CXXFLAGS = -Wall -Wextra -std=c++17

# SDL2 flags
SDL2_CFLAGS = $(shell sdl2-config --cflags)
SDL2_LDFLAGS = $(shell sdl2-config --libs)

# Include directories
INCLUDES = -I.

# Source files
SRCS = main.cpp display.cpp geometry.cpp our_gl.cpp model.cpp tgaimage.cpp

# Header files
HEADERS = display.h geometry.h our_gl.h model.h tgaimage.h

# Object files
OBJS = $(SRCS:.cpp=.o)

# Output binary
TARGET = main

# Build rules
all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) $(OBJS) -o $(TARGET) $(SDL2_LDFLAGS)

%.o: %.cpp $(HEADERS)
	$(CXX) $(CXXFLAGS) $(SDL2_CFLAGS) $(INCLUDES) -c $< -o $@

clean:
	rm -f $(OBJS) $(TARGET)

# Phony targets
.PHONY: all clean
