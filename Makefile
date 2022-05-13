
CC = gcc
CFLAGS = -Wall -pthread
OBJS = server.o client.o helpers.o 

.SUFFIXES: .c .o 

all: server client

server: server.o helpers.o
	$(CC) $(CFLAGS) -o server server.o helpers.o 

client: client.o helpers.o
	$(CC) $(CFLAGS) -o client client.o helpers.o

.c.o:
	$(CC) $(CFLAGS) -o $@ -c $<
	
clean:
	-rm -f $(OBJS) server client