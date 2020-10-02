CXX = g++
#-Wall flag - tells the compiler to give you (almost) all warnings
#T-g flag - asks gcc to put debugging information in the object file and the executable
CXXFLAGS = -g -std=gnu++11
TARGETS_SERVER =  run_server.o server.o 


all : server

server : run_server.o server.o
	$(CXX) $(CXXFLAGS) $(TARGETS_SERVER) -o server -lpthread

run_server.o : ./src/run_server.cpp server.o
	$(CXX) $(CXXFLAGS) -c ./src/run_server.cpp 

server.o : ./src/server.cpp ./include/server.h
	$(CXX) $(CXXFLAGS) -c ./src/server.cpp 

clean:
	rm *.o server




