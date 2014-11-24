CC=g++

CFLAGS=-c -Wall
LFLAGS=-lm

SOURCES=runCpluPlus.cpp JSON.cpp JSONValue.cpp
HEADERS=JSON.h JSONValue.h cPlusPlusCompiler.h

OBJECTS=$(SOURCES:.cpp=.o)

EXECUTABLE=TestJSON

all: $(SOURCES) $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(CC) $(LFLAGS) $(OBJECTS) -o $@

.cpp.o:
	$(CC) $(CFLAGS) $< -o $@

#obj/%.o: %.cpp $(HEADERS)
#	$(CC) $(CFLAGS) $(@:obj/%.o=%.cpp) -o $@

clean:
	rm -f $(OBJECTS) $(EXECUTABLE) *~