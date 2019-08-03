CPPFLAGS=-Wall -g -pthread
LDFLAGS=
LDLIBS=-lpthread

TARGETS=barrier semaphore

all: ${TARGETS}

barrier: barrier.o
	g++ $(LDFLAGS) -o barrier barrier.o $(LDLIBS)

barrier.o: barrier.cpp
	g++ $(CPPFLAGS) -c barrier.cpp

semaphore: semaphore.o
	g++ $(LDFLAGS) -o semaphore semaphore.o $(LDLIBS)

semaphore.o: semaphore.cpp
	g++ $(CPPFLAGS) -c semaphore.cpp

.PHONY: clean

clean:
	rm -f ${TARGETS} *.o
