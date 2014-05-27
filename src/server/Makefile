.PHONY: all clean

CXX_FLAGS = -std=c++11 -O2 -Wall -lboost_system -lpthread -lboost_program_options
COMMON = ../common.hpp ../logger.hpp
DIR = ../../bin

all: $(DIR)/serwer

$(DIR):
	mkdir -p $(DIR)

clean:
	rm -rf *.o
	rm -rf $(DIR)/serwer

$(DIR)/serwer: $(DIR) UDPServer.o TCPDiagnosticServer.o ConnectionsController.o Queue.o MixerController.o mixer.o server.cpp $(COMMON)
	g++ $(CXX_FLAGS) server.cpp UDPServer.o TCPDiagnosticServer.o ConnectionsController.o Queue.o MixerController.o mixer.o -o $(DIR)/serwer

UDPServer.o: UDPServer.hpp UDPServer.cpp $(COMMON)
	g++ $(CXX_FLAGS) -c UDPServer.cpp -o UDPServer.o

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