CPPFLAGS := -Wall -pthread -g
LDFLAGS :=
LDLIBS := -lpthread

all: concurrency metaprogramming algo
.PHONY: all

clean:
	rm -f test_* *.o
.PHONY: clean

include concurrency/Makefile metaprogramming/Makefile algo/Makefile
