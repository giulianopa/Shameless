CPPFLAGS=-Wall -g -pthread
LDFLAGS=
LDLIBS=-lpthread

barrier: barrier.o
	g++ $(LDFLAGS) -o barrier barrier.o $(LDLIBS)

barrier.o: barrier.cpp
	g++ $(CPPFLAGS) -c barrier.cpp
