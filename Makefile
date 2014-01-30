CC=g++
CFLAGS=-c -O3 -Wall
LDFLAGS=
SRCDIR=src
_SOURCES=calc.cpp \
		parser.cpp \
		variables.cpp \
		vlf.cpp \
		vlfcomplex.cpp
SOURCES=$(patsubst %,$(SRCDIR)/%,$(_SOURCES))
OBJDIR=.
_OBJECTS=$(_SOURCES:.cpp=.o)
OBJECTS=$(patsubst %,$(OBJDIR)/%,$(_OBJECTS))
EXECUTABLE=calc

all: $(SOURCES) $(EXECUTABLE)
	
$(EXECUTABLE): $(OBJECTS)
	$(CC) $(LDFLAGS) $(OBJECTS) -o $@

$(OBJDIR)/%.o: $(SRCDIR)/%.cpp
	$(CC) $(CFLAGS) $< -o $@

.PHONY: clean

clean:
	rm $(OBJDIR)/*.o
