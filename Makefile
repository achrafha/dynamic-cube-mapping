CCFLAGS = -std=c11 -pedantic -Wall -Wextra `sdl2-config --cflags`
LDLIBS = -lGLU -lGL -lGLEW `sdl2-config --libs`
SRC = $(wildcard *.c)
OBJ = $(SOURCES:.c=.o)
BIN = cubemap

all: $(BIN)

cubemap: cubemap.o

%.o : %.c $(HDR)
	$(CC) $(CCFLAGS) -c $<

run: all
	./cubemap

shell:
	nix-shell

clean:
	$(RM) -f *.o $(BIN)
