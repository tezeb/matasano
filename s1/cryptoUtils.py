#!/usr/bin/python3

def strxor(a, b):
    #   assert len(b) > 0
    #   assume a and b are bytes arrays
    c = (b*(len(a)//len(b)+1))[:len(a)]
    return "".join([
        chr(x ^ y) for(x, y) in zip(a, c)
        ])
