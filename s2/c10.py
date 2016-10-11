#!/usr/bin/python3

from Crypto.Cipher import AES
from py_common.cryptoUtils import decryptAES_CBC
import base64

if __name__ == "__main__":
    t3 = ""
    key = "YELLOW SUBMARINE"
    with open('10.txt', 'r') as myfile:
        t3=myfile.read().replace('\n', '')
    t3 = base64.b64decode(t3)
    iv = b"\x00"*16
    dec = decryptAES_CBC(t3, key, iv)
    print("Decrypted:")
    print(dec[:256].decode('latin-1'))
