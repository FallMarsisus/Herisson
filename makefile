CC = gcc
CFLAGS = -Wall -Wextra -std=c99 -pedantic

UNAME_S := $(shell uname -s)

SRC_COMMON = board.c engine.c tests.c ui.c
SRC_CLI    = main.c $(SRC_COMMON)
OBJ_CLI    = $(SRC_CLI:.c=.o)

SRC_GUI    = main.c $(SRC_COMMON)
OBJ_GUI    = $(SRC_GUI:.c=.o)

EXEC = herisson


ifeq ($(UNAME_S), Darwin)
    RAYLIB_FLAGS = $(shell pkg-config --cflags raylib 2>/dev/null || echo "-I/opt/homebrew/include")
    RAYLIB_LIBS  = $(shell pkg-config --libs raylib 2>/dev/null || echo "-L/opt/homebrew/lib -lraylib") \
                   -framework OpenGL -framework Cocoa -framework IOKit -framework CoreVideo
else
    RAYLIB_FLAGS = $(shell pkg-config --cflags raylib 2>/dev/null)
    RAYLIB_LIBS  = $(shell pkg-config --libs raylib 2>/dev/null || echo "-lraylib") -lm -lpthread -ldl
endif

.PHONY: all gui clean

all: $(EXEC)

$(EXEC): $(OBJ_CLI)
	$(CC) $(CFLAGS) -o $@ $^

gui: CFLAGS += -DENABLE_GUI $(RAYLIB_FLAGS)
gui: LDFLAGS += $(RAYLIB_LIBS)
gui: clean_gui $(OBJ_GUI)
	$(CC) $(CFLAGS) -o $(EXEC) $(OBJ_GUI) $(LDFLAGS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean_gui:
	@rm -f main.o

clean:
	rm -f *.o $(EXEC)