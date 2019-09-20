#!/bin/bash
# Copyright (C) 2019 Giuliano Pasqualotto (github.com/giulianopa)
# This code is licensed under MIT license (see LICENSE.txt for details)
# Search for:
# pop  %rdi
# retq
# Or:  0x5f 0xc3
echo "Gadget address: "`xxd -c1 -p /lib/x86_64-linux-gnu/libc.so.6 | grep -n -B1 c3 | grep 5f -m1 | awk '{printf"%x\n",$1-1}'`
echo "system() address: "`nm -D /lib/x86_64-linux-gnu/libc.so.6 | grep '\<system\>'`
