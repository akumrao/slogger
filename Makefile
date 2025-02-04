CFILES   = $(wildcard *.c)
OBJFILES = $(CFILES:.c=.o)
OUT      = main

CC      = gcc
CFLAGS  = -g -Wall 
#LDFLAGS = -L/additional/lib/dir
LDLIBS  = -lpthread

$(OUT): $(OBJFILES)

.PHONY: clean

clean:
	rm -f $(OBJFILES) $(OUT)
