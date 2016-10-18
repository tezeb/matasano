#!/usr/bin/env python3

from py_common.cryptoUtils import pad

def stripPadding(text):
    for i in range(2, ord(text[-1])):
        if text[-i] != text[-1]:
            raise ValueError("Wrong padding!")
    return text[:-ord(text[-1])]

def testPadding(text):
    try:
        print(stripPadding(text))
    except ValueError as error:
        print("[-]", repr(text))
        print("[-] failed:",error)

def main():
    s = b"YELLOW SUBMARINE"
    j = 1
    for i in range(10, 25):
        tmp = pad(s, i).decode('latin-1')
        testPadding(tmp)
        testPadding(tmp[:-j])
        j%=10
        j+=1

if __name__ == "__main__":
    main()
