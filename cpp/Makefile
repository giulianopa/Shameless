CPPFLAGS := -Wall -pthread -O3
LDFLAGS :=
LDLIBS := -lpthread

all: concurrency metaprogramming
.PHONY: all

clean:
	rm -f test_* *.o
.PHONY: clean

include concurrency/Makefile metaprogramming/Makefile
