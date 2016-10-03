#!/usr/bin/python3

import binascii
from languageScore import freqScore
from cryptoUtils import strxor

def main():
    ret = None
    max_v = 0
    with open("4.txt", "r") as file:
        for l in file:
            res = binascii.unhexlify(l.strip())
            for x in range(0, 256):
                t1 = strxor(res, str(chr(x)).encode('latin1'))
                value = freqScore(t1)
                if value > max_v:
                    ret = t1
                    max_v = value
    print("Max: ",max_v," ",ret)

if __name__ == '__main__':
    main()
