CPPFLAGS=-Wall -pthread -O3
LDFLAGS=
LDLIBS=-lpthread

meta: meta.o
	g++ $(LDFLAGS) -o meta meta.o $(LDLIBS)

meta.o: meta.cpp
	g++ $(CPPFLAGS) -c meta.cpp

.PHONY: clean

clean:
	rm -f meta *.o
