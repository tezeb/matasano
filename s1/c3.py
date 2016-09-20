#!/usr/bin/python

import binascii
import itertools
from collections import Counter

def strxor(a, b):
    return "".join([
        chr(x ^ b) for (x,) in zip(a)
        ])

def getCount(haystack, needle):
    cnt = 0
    brd_v = 0
    value = 0
    for key in haystack:
        if cnt < 6:
            brd_v = key[1]
        if key[1] != brd_v and cnt > 5:
            break
        if key[0] in needle:
            value+=1
    return value

def main():
    t1 = binascii.unhexlify(b"1b37373331363f78151b7f2b783431333d78397828372d363c78373e783a393b3736")
    max_v = 0
    lp = list()
    l = list()
    i = 0
    for x in range(0, 256):
        res = strxor(t1, x)
        c = Counter(res)
        value = getCount(c.most_common(), ' etaoi')
        value += getCount(c.most_common()[::-1], 'vkjxqz')
        i+=1
        if value > max_v:
            print(value,">",max_v)
            lp = l
            l = list()
            max_v = value
            l.append(res)
        elif max_v == value:
            l.append(res)
        elif abs(max_v-value) < 2:
            lp.append(res)
    print("Max: ",max_v)
    for k in lp:
        print(k)
    print("-------------")
    for k in l:
        print(k)

if __name__ == '__main__':
    main()
