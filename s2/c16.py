#!/usr/bin/python3

import py_common.cryptoUtils as cu
import base64
import string
import sys
from random import SystemRandom as SRand
import urllib.parse as url

class oracle:
    def __init__(self):
        self.key = cu.createRandomString(16)

    def createProfile(self, userdata):
        print("-"*32)
        plain = "comment1=cooking%20MCs;userdata="
        plain += url.quote(userdata)
        plain += ";comment2=%20like%20a%20pound%20of%20bacon"
        plain = cu.pad(plain.encode('ascii'), 16)
        print(repr(plain))
        iv = cu.createRandomString(16)
        return iv + cu.encryptAES_CBC(plain, self.key, iv)

    def logIn(self, data):
        iv = data[:16]
        plain = cu.decryptAES_CBC(data[16:], self.key, iv)
        plain = plain[:-plain[-1]].decode('latin-1')
        user = self.getUserInfo(plain)
        print("-"*32)
        print(repr(plain))
        for k,i in user.items():
            print(k,":",repr(i))

    def getUserInfo(self, data):
        user = dict()
        for i in data.split(';'):
            j = i.split('=')
            if len(j) != 2:
                continue
            user[j[0]] = url.unquote(j[1])
        return user

if __name__ == "__main__":
    o = oracle()
    cookie = o.createProfile("test@test.com")
    o.logIn(cookie)
    cookie = o.createProfile("test@test.com;admin=true")
    o.logIn(cookie)
    #   we know it, but it can also be found out
    blockLen = 16
    #   iv is the first block
    #   how to guess it?
    encData1 = "comment1=cooking%20MCs;userdata="
    mData = ""
    if len(encData1) % blockLen:
        mData = "A" * (blockLen - len(encData1)%blockLen)
    #   add block for mangling
    mData += "B" * blockLen
    toAppend = bytearray(";admin=true".encode('ascii'))
    toAppend[0] = ord('X')
    toAppend[6] = ord('X')
    mData += "C" * (blockLen - len(toAppend)%blockLen)
    mData += toAppend.decode('ascii')
    id1 = len(mData) - len(toAppend)
    id2 = id1 + 6
    print(mData)
    cryptxt = bytearray(o.createProfile(mData))
    #   there is iv at the beggining, so id1/id2
    #   points at block before the encrypted block
    cryptxt[id1+len(encData1)] ^= ord('X') ^ ord(';')
    cryptxt[id2+len(encData1)] ^= ord('X') ^ ord('=')
    o.logIn(bytes(cryptxt))
     
