#!/usr/bin/env python3

import py_common.cryptoUtils as cu
from base64 import b64decode
import sys

class service:
    def __init__(self):
        self.key = cu.createRandomString(16)
        self.secrets = [
	    b"MDAwMDAwTm93IHRoYXQgdGhlIHBhcnR5IGlzIGp1bXBpbmc=",
	    b"MDAwMDAxV2l0aCB0aGUgYmFzcyBraWNrZWQgaW4gYW5kIHRoZSBWZWdhJ3MgYXJlIHB1bXBpbic=",
	    b"MDAwMDAyUXVpY2sgdG8gdGhlIHBvaW50LCB0byB0aGUgcG9pbnQsIG5vIGZha2luZw==",
	    b"MDAwMDAzQ29va2luZyBNQydzIGxpa2UgYSBwb3VuZCBvZiBiYWNvbg==",
	    b"MDAwMDA0QnVybmluZyAnZW0sIGlmIHlvdSBhaW4ndCBxdWljayBhbmQgbmltYmxl",
	    b"MDAwMDA1SSBnbyBjcmF6eSB3aGVuIEkgaGVhciBhIGN5bWJhbA==",
	    b"MDAwMDA2QW5kIGEgaGlnaCBoYXQgd2l0aCBhIHNvdXBlZCB1cCB0ZW1wbw==",
	    b"MDAwMDA3SSdtIG9uIGEgcm9sbCwgaXQncyB0aW1lIHRvIGdvIHNvbG8=",
	    b"MDAwMDA4b2xsaW4nIGluIG15IGZpdmUgcG9pbnQgb2g=",
	    b"MDAwMDA5aXRoIG15IHJhZy10b3AgZG93biBzbyBteSBoYWlyIGNhbiBibG93"]

    def createSecret(self):
        for i in self.secrets:
            i = cu.pad(b64decode(i))
            iv = cu.createRandomString(16)
            ret = iv
            ret += cu.encryptAES_CBC(i, self.key, iv)
            yield ret

    def consumeSecret(self, cryptxt):
        iv = cryptxt[:16]
        p = cu.unpadVerified(cu.decryptAES_CBC(cryptxt[16:], self.key, iv))

def main():
    oracle = service()
    for s in oracle.createSecret():
        print(cu.CBC_OraclePadding(oracle, s))

if __name__ == "__main__":
    main()
