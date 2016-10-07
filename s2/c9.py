#!/usr/bin/env python3

def pad(s, i):
    c = i - len(s)%i
    return s + chr(c) * c

def main():
    s = "YELLOW SUBMARINE"
    for i in range(10, 25):
        print(repr(pad(s, i)))

if __name__ == "__main__":
    main()
