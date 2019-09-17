mkfile_path := $(abspath $(lastword $(MAKEFILE_LIST)))
current_dir := $(notdir $(patsubst %/,%,$(dir $(mkfile_path))))

metaprogramming: test_meta
.PHONY: metaprogramming

test_meta: test_meta.o
	g++ $(LDFLAGS) -o $@ $? $(LDLIBS)

test_meta.o: ${current_dir}/test_meta.cpp
	g++ $(CPPFLAGS) -c $<
