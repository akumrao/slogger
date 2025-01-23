CFILES   = $(wildcard *.c)
OBJFILES = $(CFILES:.c=.o)
OUT      = main

CC      = gcc
CFLAGS  = -g -Wall 
#LDFLAGS = -L/additional/lib/dir
#LDLIBS  = -ldependency1 -ldependency2

$(OUT): $(OBJFILES)

.PHONY: clean

clean:
	rm -f $(OBJFILES) $(OUT)
