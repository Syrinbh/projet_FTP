.PHONY: all clean

# Disable implicit rules
.SUFFIXES:

CC      = gcc
CFLAGS  = -Wall -g
VPATH   = src/
INCLDIR = -Iinclude/

LIBS    = -lpthread

INCLUDE = include/csapp.h include/ftp.h
OBJS    = csapp.o

all: ftpserver ftpclient

%.o: %.c $(INCLUDE)
	$(CC) $(CFLAGS) $(INCLDIR) -c -o $@ $

ftpserver: ftpserver.o $(OBJS)
	$(CC) -o $@ $(LDFLAGS) $^ $(LIBS)

ftpclient: ftpclient.o $(OBJS)
	$(CC) -o $@ $(LDFLAGS) $^ $(LIBS)

clean:
	rm -f ftpserver ftpclient *.o