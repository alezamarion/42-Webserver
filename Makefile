CC = g++
CFLAGS = -Wall -std=c++98
LDFLAGS =

all: server client

server: server.cpp
	$(CC) $(CFLAGS) -o server server.cpp $(LDFLAGS)

client: client.cpp
	$(CC) $(CFLAGS) -o client client.cpp $(LDFLAGS)

clean:
	rm -f server client
