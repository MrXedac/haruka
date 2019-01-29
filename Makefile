CC=gcc
LD=gcc
AS=gcc

CSOURCES=$(wildcard src/*.c)
ASOURCES=$(wildcard src/*.S)

COBJ=$(CSOURCES:.c=.o)
AOBJ=$(ASOURCES:.S=.o)

CFLAGS=-c -Iinclude/

# Comment this out on non-macOS compilation targets, but make sure SDL2 is still linked
OSX_FRAMEWORK_PATH=/Library/Frameworks
OSX_LDFLAGS=-L/usr/local/lib -lSDL2 -Wl,-framework,OpenGL -lSDL2_image -lSDL2_mixer -lSDL2_ttf -lsimple2d
OSX_CFLAGS=-I/usr/local/include/SDL2 -D_THREAD_SAFE

ASFLAGS=-c -Iinclude/

EXEC=haruka

$(EXEC): $(COBJ) $(AOBJ)
	$(LD) $(OSX_LDFLAGS) $^ -o $@

%.o: %.c
	$(CC) $(CFLAGS) $(OSX_CFLAGS) $< -o $@

%.o: %.S
	$(AS) $(ASFLAGS) $< -o $@

clean:
	rm -f $(AOBJ) $(COBJ) $(EXEC)

.PHONY: $(EXEC) clean
