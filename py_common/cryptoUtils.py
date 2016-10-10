#!/usr/bin/python3

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
