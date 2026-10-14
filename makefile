CC = gcc
CFLAGS = -Wall -Wextra -g 

UNAME_S := $(shell uname -s)

SRC_COMMON = board.c engine.c tests.c ui.c all_move.c
SRC_CLI    = main.c $(SRC_COMMON)
OBJ_CLI    = $(SRC_CLI:.c=.o)

SRC_GUI    = main.c $(SRC_COMMON)
OBJ_GUI    = $(SRC_GUI:.c=.o)

EXEC = herisson


ifeq ($(UNAME_S), Darwin) # MAC OS
    RAYLIB_FLAGS = $(shell pkg-config --cflags raylib 2>/dev/null || echo "-I/opt/homebrew/include")
    RAYLIB_LIBS  = $(shell pkg-config --libs raylib 2>/dev/null || echo "-L/opt/homebrew/lib -lraylib") \
                   -framework OpenGL -framework Cocoa -framework IOKit -framework CoreVideo
else # Linux
    RAYLIB_FLAGS = $(shell pkg-config --cflags raylib 2>/dev/null)
    RAYLIB_LIBS  = $(shell pkg-config --libs --static raylib 2>/dev/null || echo "-lraylib -lGL -lm -lpthread -ldl -lrt -lX11")
endif # If you are on windows, i'm sorry you'll have figure out by yourself how to compile...

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
	@rm -f main.o ui.o

clean:
	rm -f *.o $(EXEC)
