CFLAGS := -x c -Wall -pthread -g
CPPFLAGS := -Wall -pthread -g
LDFLAGS :=
LDLIBS := -lpthread

all: concurrency metaprogramming algo hw roi
.PHONY: all

clean:
	rm -f test_* *.o
.PHONY: clean

include concurrency/Makefile \
 	metaprogramming/Makefile \
	algo/Makefile hw/Makefile \
	ROI/Makefile
