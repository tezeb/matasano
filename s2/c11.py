#!/usr/bin/python3

import py_common.cryptoUtils as cu
from random import SystemRandom as SRand

def randomEncryption(plain):
    iv = cu.createRandomString(16)
    key = cu.createRandomString(16)
    #   prepend with random
    plain = cu.createRandomString(SRand().randint(5, 10)) + plain
    #   append with random
    plain += cu.createRandomString(SRand().randint(5, 10))
    #   pad
    plain = cu.pad(plain, 16)
    if SRand().randint(0,1):
        return (cu.encryptAES_ECB(plain, key),"ECB")
    else:
        return (cu.encryptAES_CBC(plain, key, iv),"CBC")

if __name__ == "__main__":
    numTests = 256
    data = b"A"*48
    ecb = 0
    cbc = 0
    for i in range(0, numTests):
        (cryptxt, truth) = randomEncryption(data)
        guess = cu.isECB(cryptxt)
        if guess == True and truth == "ECB":
            ecb+=1
        elif truth == "CBC":
            cbc += 1
    print("Tested:\t",numTests)
    print("ECB:\t",ecb)
    print("CBC:\t",cbc)
    if ecb+cbc == numTests:
        print("All guessed OK")
    else:
        print("Some guesses mismatched!!!")
