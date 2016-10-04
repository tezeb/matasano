#!/usr/bin/python3

import binascii
from languageScore import freqScore
from cryptoUtils import strxor
from cryptoUtils import bruteXor

def main():
    ret = None
    max_v = 0
    with open("4.txt", "r") as file:
        for l in file:
            res = binascii.unhexlify(l.strip())
            (a, b, c) = bruteXor(res, freqScore)
            if c > max_v:
                ret = a
                max_v = c
    print("Max: ",max_v," ",ret)

if __name__ == '__main__':
    main()
