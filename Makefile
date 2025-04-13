# Compiler
CC = gcc

# Installation prefix (overridable)
PREFIX ?= /usr/local
DATADIR ?= $(PREFIX)/share/ScottsPong
ASSETDIR ?= $(DATADIR)/assets

# Compilation flags
CFLAGS = -Wall -Wextra -std=c11 -DASSETDIR=\"$(ASSETDIR)/\" `pkg-config --cflags sdl SDL_image`
LDFLAGS = `pkg-config --libs sdl SDL_image`

# Source files
SRC = main.c Racket.c
OBJ = $(SRC:.c=.o)
HEADERS = Constantes.h Racket.h

# Target binary
TARGET = ScottsPong

# Default rule
all: $(TARGET)

# Build target
$(TARGET): $(OBJ)
	$(CC) $(CFLAGS) -o $@ $(OBJ) $(LDFLAGS)

# Pattern rule for object files
%.o: %.c $(HEADERS)
	$(CC) $(CFLAGS) -c $< -o $@

# Installation
install: $(TARGET)
	mkdir -p $(DESTDIR)$(PREFIX)/bin
	cp $(TARGET) $(DESTDIR)$(PREFIX)/bin/

# Clean
clean:
	rm -f $(TARGET) $(OBJ)

.PHONY: all clean install
