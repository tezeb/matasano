#!/usr/bin/env python3

from py_common.cryptoUtils import pad, unpadVerified

def testPadding(text):
    try:
        print("[+]", unpadVerified(text))
    except ValueError as error:
        print("[-]", repr(text))
        print("[-] failed:",error)

def main():
    s = b"YELLOW SUBMARINE"
    j = 1
    for i in range(10, 25):
        tmp = pad(s, i)
        testPadding(tmp)
        testPadding(tmp[:-j])
        j%=10
        j+=1

if __name__ == "__main__":
    main()
