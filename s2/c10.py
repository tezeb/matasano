#!/usr/bin/python3

from Crypto.Cipher import AES
from py_common.cryptoUtils import strxor
import base64

def decryptAES_ECB(enc, key):
    mode = AES.MODE_ECB
    aes = AES.new(key, mode)
    return aes.decrypt(enc)

def decryptAES_CBC(cryptxt, key, iv):
    plain = ""
    for i in range(16, len(cryptxt)+1, 16):
        middle = decryptAES_ECB(cryptxt[i-16:i], key)
        plain += strxor(middle, iv).decode('ascii')
        iv = cryptxt[i-16:i]
    return plain

if __name__ == "__main__":
    t3 = ""
    key = "YELLOW SUBMARINE"
    with open('10.txt', 'r') as myfile:
        t3=myfile.read().replace('\n', '')
    t3 = base64.b64decode(t3)
    iv = b"\x00"*16
    dec = decryptAES_CBC(t3, key, iv)
    print("Decrypted:")
    print(dec[:256])
