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

def pad(string, blockLen=16):
    c = blockLen - len(string)%blockLen
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

def isECB(cryptxt, blockLen=16, retChunk=False):
    (count, chunk) = findMaxRepetitionsCnt(
            [cryptxt[i:i+blockLen] for i in range(0, len(cryptxt)-blockLen+1, blockLen)]
            )
    if retChunk:
        return (count > 1, chunk)
    if count > 1:
        return True
    return False

def bfEcbByte(oracle, decoded, blockLen, marker, encMarker, needle, alphabet=string.printable):
    for c in alphabet:
        maybeNeedle = marker + decoded[-blockLen+1:]
        maybeNeedle.append(ord(c))
        maybeNeedle.append(ord('X'))
        c2 = oracle.encrypt(maybeNeedle * blockLen)
        offset = c2.find(encMarker)
        c2 = c2[offset+len(encMarker):offset+len(encMarker)+blockLen]
        if c2 == needle:
            decoded.append(ord(c))
            return True
    return False

def decryptEcbMode(oracle, blockLen, expectedCryptxLen, marker, encMarker, decoded = bytearray()):
    if len(decoded) == 0:
        decoded = bytearray([ord("C") for _ in range(blockLen-1)])
    fillLen = blockLen-1

    tries = 0
    while (len(decoded) - fillLen) < expectedCryptxLen:
        print("\rDecrypted:",len(decoded)-fillLen,"/",expectedCryptxLen,end="")
        #   fillLen does not count into block count
        blockNmbr = (len(decoded)-fillLen)//blockLen
        #   prefixLen means missing characters for current block
        #   so it must decrease from blockLen-1 -> 0 for each block
        #   fillLen also have to be subtracted
        #   and it happens that blockLen - 1 == fillLen
        prefixLen = (fillLen * 2 - len(decoded))%blockLen
        test = marker + decoded[:prefixLen]
        offset = -1
        ltries = 0
        while offset < 0:
            c1 = oracle.encrypt(test)
            offset = c1.find(encMarker)
            ltries+=1
            if ltries%100 == 0:
                test.insert(0, ord("X"))
        tries+=ltries
        #   the moment we will start decoding padding it will be aligned
        #   to 0x01(by our prefixLen) so we will get proper expectedCryptxLen
        #   and exacly 1byte of padding to decrypt(== 0x01)
        expectedCryptxLen = min(expectedCryptxLen, len(c1) - offset - len(encMarker) - prefixLen)
        needleOff = offset+len(encMarker)+blockNmbr*blockLen
        c1 = c1[needleOff:needleOff+blockLen]
        if not bfEcbByte(oracle, decoded, blockLen, marker, encMarker, c1):
            #   !!! WARNING !!!
            #   after hitting first padding byte, decoding further bytes requires
            #   one to increment already decrypted bytes of padding for each step
            #   as this is dynamically created by oracle for each data to be encrypted
            #   so it changes with each new byte under decryption
            #   This may be enabled by uncommenting padding, else statement
            #   and disabling expectedCryptxLen calculation above
            #
            padding = '\x01'
#           padding = ''.join(chr(i) for i in range(1, blockLen))
            if not bfEcbByte(oracle, decoded, blockLen, marker, encMarker, c1, alphabet=padding):
                print("\n[-] No match in alphabet(including padding bytes)\n")
                print("Use following snippet to continue:")
                print("key = unhexlify('",hexlify(oracle.key),"')")
                print("marker = unhexlify('",hexlify(marker),"')")
                print("encMarker = unhexlify('",hexlify(encMarker),"')")
                print("decoded = ",repr(decoded))
                print("o = service(key=key)")
                print("decryptEcbMode(o, 16, 139, marker, encMarker, decoded=decoded)")
                sys.exit(1)
            else:
                pass
#                print("[+] Binary match, probably padding")
#                for i in range(decoded[-1]):
#                    decoded[-(i+1)] += 1
    print("\n[>] Avg tries:",tries/(len(decoded)-fillLen))
    print("[>] Decoded:")
    print(decoded[fillLen:-decoded[-1]].decode('latin-1'))

def discoverBlockLen(oracle, probes=32, minBlockLen=16, interactive=True):
    test = bytearray()
    cryptLen = set()
    blockLen = 0
    while True:
        for _ in range(probes):
            cryptLen.add(len(oracle.encrypt(bytes(test))))
            test.append(ord('A'))
        l = sorted(cryptLen)
        blockLen = l[-1]-l[0]
        p = l[0]
        for i in l:
            j = i - p
            if j > 0 and j < blockLen:
                blockLen = i - p
            p = i
        if interactive:
            print("[>] BlockLen guess:",blockLen)
        if blockLen == minBlockLen or (verbose and input("Try harder? [y/N]") != "y"):
            break
    return blockLen
