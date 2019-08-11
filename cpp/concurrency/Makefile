CPPFLAGS=-Wall -pthread -O3
LDFLAGS=
LDLIBS=-lpthread

TARGETS=barrier semaphore
TESTS=test_thread_creation

all: ${TARGETS}

test: ${TESTS}

barrier: barrier.o
	g++ $(LDFLAGS) -o barrier barrier.o $(LDLIBS)

barrier.o: barrier.cpp
	g++ $(CPPFLAGS) -c barrier.cpp

semaphore: semaphore.o
	g++ $(LDFLAGS) -o semaphore semaphore.o $(LDLIBS)

semaphore.o: semaphore.cpp
	g++ $(CPPFLAGS) -c semaphore.cpp

test_thread_creation: test_thread_creation.o
	g++ $(LDFLAGS) -o test_thread_creation test_thread_creation.o $(LDLIBS)

test_thread_creation.o: test_thread_creation.cpp
	g++ $(CPPFLAGS) -c test_thread_creation.cpp

.PHONY: clean

clean:
	rm -f ${TARGETS} ${TESTS} *.o
