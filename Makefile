.DEFAULT = all
.PHONY = clean

CXX_FLAGS = -std=c++11 -ggdb -Wall -lboost_system -lpthread

all: client server


client: client.cpp
	g++ $(CXX_FLAGS) client.cpp -o client

server: server.cpp
	g++ $(CXX_FLAGS) server.cpp -o server

clean:
	rm -rf *.o
	rm -rf client server
