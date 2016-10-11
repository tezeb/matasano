#!/usr/bin/python3

from Crypto.Cipher import AES
import string
import random
from .utils import findMaxRepetitionsCnt

def strxor(a, b):
    #   assert len(b) > 0
    #   assume a and b are bytes arrays
    c = (b*(len(a)//len(b)+1))[:len(a)]
    return bytes([
        x ^ y for(x, y) in zip(a, c)
        ])

def bruteXor(res, predicate, *args):
    ret = None
    dec = None
    max_v = 0
    for x in range(0, 256):
        t1 = strxor(res, bytes([x]))
        value = predicate(t1, *args)
        if value > max_v:
            ret = t1
            max_v = value
            dec = x
    return (ret, dec, max_v)

def strToBin(a):
    abin=""
    for v in a:
        abin += bin(v)[2:].zfill(8)
    return abin

def editDist(a, b):
    dist=0
    for x, y in zip(strToBin(a), strToBin(b)):
#        print(x, "=", y)
        if x != y:
            dist+=1
    return dist

def pad(string, new_len):
    c = new_len - len(string)%new_len
    return string + bytes([c] * c)

def decryptAES_ECB(cryptxt, key):
    mode = AES.MODE_ECB
    aes = AES.new(key, mode)
    return aes.decrypt(cryptxt)

def encryptAES_ECB(plaintxt, key):
    mode = AES.MODE_ECB
    aes = AES.new(key, mode)
    return aes.encrypt(plaintxt)

def decryptAES_CBC(cryptxt, key, iv):
    plain = bytes()
    for i in range(16, len(cryptxt)+1, 16):
        middle = decryptAES_ECB(cryptxt[i-16:i], key)
        plain += strxor(middle, iv)
        iv = cryptxt[i-16:i]
    return plain

def encryptAES_CBC(plain, key, iv):
    cryptxt = bytes()
    for i in range(16, len(plain)+1, 16):
        middle = strxor(plain[i-16:i], iv)
        cryptxt += encryptAES_ECB(middle, key)
        iv = cryptxt[i-16:i]
    return cryptxt

def createRandomString(length, alphabet=string.printable):
    return ''.join(random.SystemRandom().choice(alphabet) for _ in range(length)).encode('ascii')

def isECB(cryptxt, blockLen=16):
    (count, chunk) = findMaxRepetitionsCnt(
            [cryptxt[i:i+blockLen] for i in range(0, len(cryptxt)-blockLen+1, blockLen)]
            )
    if count > 1:
        return True
    return False
