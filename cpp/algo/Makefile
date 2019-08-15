mkfile_path = $(abspath $(lastword $(MAKEFILE_LIST)))
current_dir = $(notdir $(patsubst %/,%,$(dir $(mkfile_path))))

algo: test_tensor
.PHONY: metaprogramming

test_tensor: tensor.o
	g++ $(LDFLAGS) -o $@ $? $(LDLIBS)

tensor.o: ${current_dir}/tensor.cpp
	g++ $(CPPFLAGS) -c $<
