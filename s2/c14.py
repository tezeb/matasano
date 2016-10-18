#!/usr/bin/python3

import py_common.cryptoUtils as cu
import base64
import string
import sys
import random
from py_common.utils import findMaxRepetitionsCnt
from binascii import hexlify, unhexlify

class service:
    def __init__(self, key=bytes(cu.createRandomString(16)), prefixLen=2048):
        self.key = key
        self.secret = "Um9sbGluJyBpbiBteSA1LjAKV2l0aCBteSByYWctdG9wIGRvd24gc28gbXkgaGFpciBjYW4gYmxvdwpUaGUgZ2lybGllcyBvbiBzdGFuZGJ5IHdhdmluZyBqdXN0IHRvIHNheSBoaQpEaWQgeW91IHN0b3A/IE5vLCBJIGp1c3QgZHJvdmUgYnkK"
        self.prefixLen = prefixLen

    def encrypt(self, plain):
        data = cu.createRandomString(random.randint(0, self.prefixLen))
        data += plain
        data += base64.b64decode(self.secret)
        data = cu.pad(data, 16)
#        print("E:",len(data), hexlify(data))
        return cu.encryptAES_ECB(bytes(data), self.key)

def main():
    o = service(prefixLen=2048)
    test = bytearray()
    blockLen = cu.discoverBlockLen(o)
    #   markers for finding our cryptxt after encryption
    #   use of two different two protect against randomness prepending "A"
    marker1 = bytearray(b"A" * blockLen)
    marker2 = bytearray(b"B" * blockLen)
    print("[>] Block len:",blockLen)
    test = marker1 * 8
    cryptxt = o.encrypt(test)
    (ecb_a, aChunk) = cu.isECB(cryptxt, retChunk=True)
    test = marker2 * 8
    cryptxt = o.encrypt(test)
    (ecb_b, bChunk) = cu.isECB(cryptxt, retChunk=True)
    if not ecb_a or not ecb_b:
        print("[-] Non ECB oracle")
        sys.exit(1)
    print("[+] ECB mode discovered")

    #   at most 31bytes longer then the real cryptxt
    cryptxtLen = len(cryptxt) - cryptxt.rfind(bChunk) - blockLen 
    
    cu.decryptEcbMode(o, blockLen, cryptxtLen, marker1 + marker2, aChunk + bChunk)

if __name__ == "__main__":
    main()
