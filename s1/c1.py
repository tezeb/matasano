#!/usr/bin/python3

import binascii
import base64

if __name__ == '__main__':
    t1="49276d206b696c6c696e6720796f757220627261696e206c696b65206120706f69736f6e6f7573206d757368726f6f6d"
    print(binascii.unhexlify(t1))
    print(base64.b64encode(binascii.unhexlify(t1)))
    print(base64.b64encode(binascii.unhexlify(t1)[:-1]))
    print(base64.b64encode(binascii.unhexlify(t1)[:-2]))
    print(base64.b64encode(binascii.unhexlify(t1)[:-3]))
