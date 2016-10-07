#!/usr/bin/python3

import binascii
from py_common.cryptoUtils import strxor

if __name__ == '__main__':
    t1 = "1c0111001f010100061a024b53535009181c"
    t2 = "686974207468652062756c6c277320657965"
    t3 = b"746865206b696420646f6e277420706c6179"
    res = strxor(binascii.unhexlify(t1), binascii.unhexlify(t2))
    #print(binascii.hexlify(bin(res)))
    print(binascii.hexlify(bytes(res, "ascii")))
    if binascii.hexlify(bytes(res, "ascii")) == t3:
        print("MATCH")
    print(res)
