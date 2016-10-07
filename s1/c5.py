#!/usr/bin/python3

import binascii
from py_common.cryptoUtils import strxor

if __name__ == '__main__':
    t1 = b"Burning 'em, if you ain't quick and nimble\nI go crazy when I hear a cymbal"
    t2 = b"ICE"
    t3 = b"0b3637272a2b2e63622c2e69692a23693a2a3c6324202d623d63343c2a26226324272765272a282b2f20430a652e2c652a3124333a653e2b2027630c692b20283165286326302e27282f"
    res = strxor(t1, t2)
    #print(binascii.hexlify(bin(res)))
    print(binascii.hexlify(bytes(res, "ascii")))
    if binascii.hexlify(bytes(res, "ascii")) == t3:
        print("MATCH")
    else:
        print("NO MATCH")
