CC=g++
CFLAGS=-Wall -Wextra -std=c++11 -Iinclude -lncurses
LDFLAGS=-pthread

SOURCES=$(wildcard src/*.cpp)
OBJECTS=$(SOURCES:.cpp=.o)

EXECUTABLE=pkmn23

all: $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(CC) $(LDFLAGS) $(OBJECTS) -o $@ -lncurses

%.o: %.cpp
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f src/*.o $(EXECUTABLE)

run: $(EXECUTABLE)
	./$(EXECUTABLE)