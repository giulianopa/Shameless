mkfile_path = $(abspath $(lastword $(MAKEFILE_LIST)))
current_dir = $(notdir $(patsubst %/,%,$(dir $(mkfile_path))))

concurrency: test_concurrency test_thread_creation
.PHONY: concurrency

concurrency.o: ${current_dir}/concurrency.cpp
	g++ $(CPPFLAGS) -c $<

test_concurrency: test_concurrency.o concurrency.o
	g++ $(LDFLAGS) -o $@ $? $(LDLIBS)

test_concurrency.o: ${current_dir}/test_concurrency.cpp
	g++ $(CPPFLAGS) -c $<

test_thread_creation: test_thread_creation.o concurrency.o
	g++ $(LDFLAGS) -o $@ $? $(LDLIBS)

test_thread_creation.o: ${current_dir}/test_thread_creation.cpp
	g++ $(CPPFLAGS) -c $<
