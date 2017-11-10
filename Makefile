CC=gcc
LD=gcc
AS=gcc

CSOURCES=$(wildcard src/*.c)
ASOURCES=$(wildcard src/*.S)

COBJ=$(CSOURCES:.c=.o)
AOBJ=$(ASOURCES:.S=.o)

CFLAGS=-c -Iinclude/
ASFLAGS=-c -Iinclude/

EXEC=haruka

$(EXEC): $(COBJ) $(AOBJ)
	$(LD) $^ -o $@

%.o: %.c
	$(CC) $(CFLAGS) $< -o $@

%.o: %.S
	$(AS) $(ASFLAGS) $< -o $@

clean:
	rm -f $(AOBJ) $(COBJ) $(EXEC)

.PHONY: $(EXEC) clean
