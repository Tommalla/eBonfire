.DEFAULT = all
.PHONY = clean

CXX_FLAGS = -std=c++11 -ggdb -Wall -lboost_system -lpthread -lboost_program_options
COMMON = common.hpp logger.hpp

all: client server


client: UDPClient.o TCPDiagnosticClient.o client.cpp $(COMMON)
	g++ $(CXX_FLAGS) client.cpp UDPClient.o TCPDiagnosticClient.o -o client

server: UDPServer.o TCPDiagnosticServer.o ConnectionsController.o Queue.o MixerController.o mixer.o server.cpp $(COMMON)
	g++ $(CXX_FLAGS) server.cpp UDPServer.o TCPDiagnosticServer.o ConnectionsController.o Queue.o MixerController.o mixer.o -o server

UDPClient.o: UDPClient.hpp UDPClient.cpp $(COMMON)
	g++ $(CXX_FLAGS) -c UDPClient.cpp -o UDPClient.o

UDPServer.o: UDPServer.hpp UDPServer.cpp $(COMMON)
	g++ $(CXX_FLAGS) -c UDPServer.cpp -o UDPServer.o

TCPDiagnosticClient.o: TCPDiagnosticClient.hpp TCPDiagnosticClient.cpp $(COMMON)
	g++ $(CXX_FLAGS) -c TCPDiagnosticClient.cpp -o TCPDiagnosticClient.o

TCPDiagnosticServer.o: TCPDiagnosticServer.hpp TCPDiagnosticServer.cpp $(COMMON)
	g++ $(CXX_FLAGS) -c TCPDiagnosticServer.cpp -o TCPDiagnosticServer.o

ConnectionsController.o: ConnectionsController.hpp ConnectionsController.cpp
	g++ $(CXX_FLAGS) -c ConnectionsController.cpp -o ConnectionsController.o

Queue.o: Queue.hpp Queue.cpp
	g++ $(CXX_FLAGS) -c Queue.cpp -o Queue.o

MixerController.o: MixerController.hpp MixerController.cpp
	g++ $(CXX_FLAGS) -c MixerController.cpp -o MixerController.o

mixer.o: mixer.hpp mixer.cpp
	g++ $(CXX_FLAGS) -c mixer.cpp -o mixer.o

clean:
	rm -rf *.o
	rm -rf client server
