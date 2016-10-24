#!/usr/bin/env python3

import py_common.cryptoUtils as cu
from base64 import b64decode

def main():
    secret = "L77na/nrFsKvynd6HzOoG7GHTLXsTVu9qvY/2syLXzhPweyyMTJULu/6/kXX0KSvoOLSFQ=="
    key = "YELLOW SUBMARINE"
    print(cu.doAES_CTR(b64decode(secret), key).decode('ascii'))

if __name__ == "__main__":
    main()
