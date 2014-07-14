CC=g++
CFLAGS=-c -Wall -Wextra -ansi -pedantic
LDFLAGS=
SOURCES=main.cpp Dependencias.cpp
OBJECTS=$(SOURCES:.cpp=.o)
EXECUTABLE=ClassMaker

all: $(SOURCES) $(EXECUTABLE)
	
$(EXECUTABLE): $(OBJECTS) 
	$(CC) $(LDFLAGS) $(OBJECTS) -o $@

.cpp.o:
	$(CC) $(CFLAGS) $< -o $@

clean:
	rm *.o