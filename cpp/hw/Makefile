mkfile_path = $(abspath $(lastword $(MAKEFILE_LIST)))
current_dir = $(notdir $(patsubst %/,%,$(dir $(mkfile_path))))

hw: eeprom_test
.PHONY: hw

eeprom_test: eeprom_test.o eeprom.o
	g++ $(LDFLAGS) -o $@ $? $(LDLIBS)

eeprom.o: ${current_dir}/eeprom.c
	g++ -x c $(CPPFLAGS) -c $<

eeprom_test.o: ${current_dir}/eeprom_test.c
	g++ -x c $(CPPFLAGS) -c $<
