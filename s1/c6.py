#!/usr/bin/python3

import binascii
import base64
from cryptoUtils import editDist
from cryptoUtils import strxor
from cryptoUtils import bruteXor
from languageScore import freqScore

def findKeysize(enc):
    minDist = 8*len(enc)
    minLen = 0
    for i in range(1,40):
        dist = 0
#   this may require adjustment or try with n shortest values
        probes=10
        for j in range(probes):
            dist+=editDist(enc[j*i:(j+1)*i], enc[(j+1)*i:(j+2)*i])
        dist/=probes*i;
        if dist < minDist:
            minDist = dist
            minLen = i
    return (minDist, minLen)

if __name__ == '__main__':
    t1 = b"this is a test"
    t2 = b"wokka wokka!!!"
    print(editDist(t1, t2))
    t3 = ""
    with open('6.txt', 'r') as myfile:
        t3=myfile.read().replace('\n', '')
    t3 = base64.b64decode(t3)
    (min_dist, keylen)=findKeysize(t3)
    print("Found min_dist:",min_dist,"for len:",keylen)

    chunks = [t3[i:i+keylen] for i in range(0, len(t3)-keylen+1, keylen)]
    res = []

    for x in zip(*chunks):
        tmp = bytearray()
        for y in x:
            tmp.append(y)
        res.append(tmp)
    password=""
    for s in res:
        (a, b, c) = bruteXor(s, freqScore)
        password += chr(b)
    print(password)
    print("Decrypted:")
    print(strxor(t3, password.encode('latin1'))[0:256])
