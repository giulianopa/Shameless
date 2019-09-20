mkfile_path := $(abspath $(lastword $(MAKEFILE_LIST)))
current_dir := $(notdir $(patsubst %/,%,$(dir $(mkfile_path))))

roi: test_overflow gen_shell
.PHONY: roi

test_overflow: test_overflow.o
	g++ $(LDFLAGS) -o $@ $? $(LDLIBS)

test_overflow_disassemble: ${current_dir}/test_overflow.c
	gcc -S -o $@.s $<

test_overflow.o: ${current_dir}/test_overflow.c
	g++ -fno-stack-protector $(CFLAGS) -c $<

gen_shell: gen_shell.o
	g++ $(LDFLAGS) -o $@ $? $(LDLIBS)

gen_shell.o: ${current_dir}/shell.c
	g++ $(CFLAGS) -c $< -o $@

test_unwinding: test_unwinding.o
	g++ $(LDFLAGS) -o $@ $? $(LDLIBS)

test_unwinding.o: ${current_dir}/test_unwinding.cpp
	g++ $(CPPFLAGS) -c $<
