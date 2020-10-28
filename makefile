CC = gcc
defaultmake: server.c 
	$(CC) -o server server.c
