#!/usr/bin/python3

import binascii
import itertools
from py_common import languageScore
from py_common.cryptoUtils import strxor
from collections import Counter

def main():
    t1 = binascii.unhexlify(b"1b37373331363f78151b7f2b783431333d78397828372d363c78373e783a393b3736")
    max_v = 0
    ret = str()
    for x in range(0, 256):
        res = strxor(t1, chr(x).encode('latin1'))
        value = languageScore.freqScore(res)
        if value > max_v:
            ret = res
            max_v = value
    print("Max: ",max_v," ",ret)

if __name__ == '__main__':
    main()
