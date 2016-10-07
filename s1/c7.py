#!/usr/bin/python3

from Crypto.Cipher import AES
import base64

def decryptAES_ECB(enc):
    key = "YELLOW SUBMARINE"
    mode = AES.MODE_ECB
    aes = AES.new(key, mode)
    return aes.decrypt(enc)

if __name__ == "__main__":
    t3 = ""
    with open('7.txt', 'r') as myfile:
        t3=myfile.read().replace('\n', '')
    t3 = base64.b64decode(t3)
    print("Decrypted:")
    print(decryptAES_ECB(t3).decode('latin1')[:256])
