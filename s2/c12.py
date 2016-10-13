#!/usr/bin/python3

import py_common.cryptoUtils as cu
import base64
import string
import sys

class oracle:
    def __init__(self):
        self.key = cu.createRandomString(16)
        self.secret = "Um9sbGluJyBpbiBteSA1LjAKV2l0aCBteSByYWctdG9wIGRvd24gc28gbXkgaGFpciBjYW4gYmxvdwpUaGUgZ2lybGllcyBvbiBzdGFuZGJ5IHdhdmluZyBqdXN0IHRvIHNheSBoaQpEaWQgeW91IHN0b3A/IE5vLCBJIGp1c3QgZHJvdmUgYnkK"

    def encrypt(self, plain):
        plain += base64.b64decode(self.secret)
        plain = cu.pad(plain, 16)
        return cu.encryptAES_ECB(plain, self.key)

if __name__ == "__main__":
    o = oracle()
    cryptxt = o.encrypt(b"");
    test = bytearray()
    while True:
        c1 = o.encrypt(bytes(test))
        if len(cryptxt) < len(c1):
            break
        test.append(ord('A'))
    blockLen = len(c1) - len(cryptxt)
    padLen = len(test)
    print("[>] Block len:",blockLen)
    print("[>] Padding:", padLen)
    test = b"A"*3*blockLen
    if not cu.isECB(o.encrypt(test)):
        print("[-] Non ECB oracle")
        sys.exit(1)
    print("[+] ECB mode discovered")

    fillLen = blockLen-1
    decoded = bytearray([ord("A") for _ in range(fillLen)])
    #   subtract padLen as decoding is done over printable
    while (len(decoded) - fillLen + padLen) < len(cryptxt):
        #   fillLen does not count into block count
        blockNmbr = (len(decoded)-fillLen)//blockLen
        #   prefixLen means missing characters for current block
        #   so it must decrease from blockLen-1 -> 0 for each block
        #   fillLen also have to be subtract
        #   and it happens that blockLen - 1 == fillLen
        prefixLen = (fillLen * 2 - len(decoded))%blockLen
        c1 = o.encrypt(bytes(decoded[:prefixLen]))
        c1 = c1[blockNmbr*blockLen:(blockNmbr+1)*blockLen]
        found = False
        for c in string.printable:
            test = decoded[-blockLen+1:]
            test.append(ord(c))
            c2 = o.encrypt(bytes(test))[0:blockLen]
            if c2 == c1:
                found = True
                decoded.append(ord(c))
                break
        if not found:
            print("[-] No match in alphabet")
            sys.exit(1)
    print("[>] Decoded:")
    print(decoded[fillLen:].decode('latin-1'))
