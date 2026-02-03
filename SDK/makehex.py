#!/usr/bin/env python3
#
# This is free and unencumbered software released into the public domain.

from sys import argv

if len(argv) < 2:
    print("Usage: ./script.py <bin_file>")
    exit(1)

binfile = argv[1]

with open(binfile, "rb") as f:
    bindata = f.read()

# Ensure data is 4-byte aligned for 32-bit words
assert len(bindata) % 4 == 0

data_word_count = len(bindata) // 4

# Only loop through the actual data
for i in range(data_word_count):
    # Print actual binary data in hex format (Little Endian to Big Endian conversion)
    w = bindata[4*i : 4*i+4]
    print("%02x%02x%02x%02x" % (w[3], w[2], w[1], w[0]))

# Append the FFFFFFFF marker and stop immediately
print("FFFFFFFF")