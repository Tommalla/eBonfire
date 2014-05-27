.PHONY = clean
DIR = ../../bin

CXX_FLAGS = -std=c++11 -O2 -Wall -lboost_system -lpthread -lboost_program_options
COMMON = ../common.hpp ../logger.hpp

all: $(DIR)/klient

clean:
	rm -rf *.o
	rm -rf $(DIR)/klient

$(DIR):
	mkdir -p $(DIR)

$(DIR)/klient: $(DIR) UDPClient.o TCPDiagnosticClient.o client.cpp $(COMMON)
	g++ $(CXX_FLAGS) client.cpp UDPClient.o TCPDiagnosticClient.o -o $(DIR)/klient

UDPClient.o: ProblematicConnectionException.hpp UDPClient.hpp UDPClient.cpp $(COMMON)
	g++ $(CXX_FLAGS) -c UDPClient.cpp -o UDPClient.o

TCPDiagnosticClient.o: ProblematicConnectionException.hpp TCPDiagnosticClient.hpp TCPDiagnosticClient.cpp $(COMMON)
	g++ $(CXX_FLAGS) -c TCPDiagnosticClient.cpp -o TCPDiagnosticClient.o
