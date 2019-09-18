mkfile_path := $(abspath $(lastword $(MAKEFILE_LIST)))
current_dir := $(notdir $(patsubst %/,%,$(dir $(mkfile_path))))

hw: test_eeprom
.PHONY: hw

test_eeprom: test_eeprom.o eeprom.o
	g++ $(LDFLAGS) -o $@ $? $(LDLIBS)

eeprom.o: ${current_dir}/eeprom.c
	g++ $(CFLAGS) -c $<

test_eeprom.o: ${current_dir}/test_eeprom.c
	g++ $(CFLAGS) -c $<
