.DEFAULT = all
.PHONY = clean

CXX_FLAGS = -std=c++11 -ggdb -Wall -lboost_system -lpthread -lboost_program_options

all: client server


client: UDPClient.o client.cpp
	g++ $(CXX_FLAGS) client.cpp UDPClient.o -o client

server: server.cpp
	g++ $(CXX_FLAGS) server.cpp -o server

UDPClient.o: UDPClient.hpp UDPClient.cpp
	g++ $(CXX_FLAGS) -c UDPClient.cpp -o UDPClient.o

clean:
	rm -rf *.o
	rm -rf client server
