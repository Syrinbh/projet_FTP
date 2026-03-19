.PHONY: all clean

# Disable implicit rules
.SUFFIXES:

CC      = gcc
CFLAGS  = -Wall -g
INCLDIR = -Iinclude/
LIBS    = -lpthread
VPATH   = src/

INCLUDE = include/csapp.h include/ftp.h
OBJS    = csapp.o

all: ftpserver ftpclient

ftpserver.o: src/ftpserver.c $(INCLUDE)
	$(CC) $(CFLAGS) $(INCLDIR) -c -o ftpserver.o src/ftpserver.c

ftpclient.o: src/ftpclient.c $(INCLUDE)
	$(CC) $(CFLAGS) $(INCLDIR) -c -o ftpclient.o src/ftpclient.c

csapp.o: src/csapp.c include/csapp.h
	$(CC) $(CFLAGS) $(INCLDIR) -c -o csapp.o src/csapp.c

ftpserver: ftpserver.o $(OBJS)
	$(CC) -o ftpserver ftpserver.o $(OBJS) $(LIBS)

ftpclient: ftpclient.o $(OBJS)
	$(CC) -o ftpclient ftpclient.o $(OBJS) $(LIBS)

clean:
	rm -f ftpserver ftpclient *.o