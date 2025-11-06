CXX = g++
CXXFLAGS = -std=c++17 -Wall -O2
LDFLAGS = -lSDL2 -lm

TARGET = easy3d
SOURCES = main.cpp
HEADERS = viewpoint.hpp wall.hpp space.hpp utils.hpp

all: $(TARGET)

$(TARGET): $(SOURCES) $(HEADERS)
	$(CXX) $(CXXFLAGS) $(SOURCES) -o $(TARGET) $(LDFLAGS)

clean:
	rm -f $(TARGET)

run: $(TARGET)
	./$(TARGET)

.PHONY: all clean run
