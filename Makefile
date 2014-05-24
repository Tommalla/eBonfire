.DEFAULT = all
.PHONY = clean

CXX_FLAGS = -std=c++11 -ggdb -Wall -lboost_system -lpthread -lboost_program_options

all: client server


client: UDPClient.o TCPDiagnosticClient.o client.cpp
	g++ $(CXX_FLAGS) client.cpp UDPClient.o TCPDiagnosticClient.o -o client

server: TCPDiagnosticServer.o server.cpp
	g++ $(CXX_FLAGS) server.cpp TCPDiagnosticServer.o -o server

UDPClient.o: UDPClient.hpp UDPClient.cpp
	g++ $(CXX_FLAGS) -c UDPClient.cpp -o UDPClient.o

TCPDiagnosticClient.o: TCPDiagnosticClient.hpp TCPDiagnosticClient.cpp
	g++ $(CXX_FLAGS) -c TCPDiagnosticClient.cpp -o TCPDiagnosticClient.o

TCPDiagnosticServer.o: TCPDiagnosticServer.hpp TCPDiagnosticServer.cpp
	g++ $(CXX_FLAGS) -c TCPDiagnosticServer.cpp -o TCPDiagnosticServer.o

clean:
	rm -rf *.o
	rm -rf client server
