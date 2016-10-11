#!/usr/bin/env python3

from py_common.cryptoUtils import pad

def main():
    s = b"YELLOW SUBMARINE"
    for i in range(10, 25):
        print(repr(pad(s, i).decode('latin-1')))

if __name__ == "__main__":
    main()
