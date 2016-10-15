#!/usr/bin/python3

import py_common.cryptoUtils as cu
import base64
import string
import sys
from random import SystemRandom as SRand
import urllib.parse as url

class oracle:
    def __init__(self):
        self.key = cu.createRandomString(16)

    def createProfile(self, email):
        plain = cu.pad(self.profile_for(email).encode('ascii'), 16)
        return cu.encryptAES_ECB(plain, self.key)

    def logIn(self, data):
        plain = cu.decryptAES_ECB(data, self.key)
        print(plain)
        plain = plain[:-plain[-1]].decode('ascii')
        user = self.getUserInfo(plain)
        print("----------------\nUser:",user['role'],"\nUid:",user['uid'])
        print("email:",user['email'])
        return user['role'] == 'admin'

    def getUserInfo(self, data):
        user = dict()
        for i in data.split('&'):
            j = i.split('=')
            if len(j) != 2:
                continue
            user[j[0]] = url.unquote(j[1])
        return user

    def profile_for(self, email):
        email = url.quote(email)
        uid = SRand().randint(0, 1000000)
        role = 'user'
        return "email={}&uid={}&role={}".format(email, uid, role)

if __name__ == "__main__":
    o = oracle()
    cookie = o.createProfile("test@test.com")
    o.logIn(cookie)
    cookie = o.createProfile("test@test.com&role=admin")
    o.logIn(cookie)
    cryptxt = o.createProfile("")
    test = bytearray()
    while True:
        c1 = o.createProfile(bytes(test))
        if len(cryptxt) < len(c1):
            break
        test.append(ord('A'))
    blockLen = len(c1) - len(cryptxt)
    print("[>] Block len:",blockLen)
    test = b"A"*3*blockLen
    test += b"@gmail.com"
    if not cu.isECB(o.createProfile(test)):
        print("[-] Non ECB oracle")
        sys.exit(1)
    print("[+] ECB mode discovered")
    #   generate chunk: "admin&uid=YYYYYY&role", where YYYYYY is in <0, 10^7>, most likely 6 digits
    #                   "email=aaaaaaaaa@"
    #                   "aaaaa.admin&uid="
    #                   "YYYYYY&role=user"
    #                   "PPPPPPPPPPPPPPPP"
    #   @ gets urlencoded to %40
    e1 = "a" * (blockLen-len("email=")-3) + "@" + "admin"
    c1 = o.createProfile(e1)[16:32]
    #   generate padding chunk(\x10 * 16)
    e1 = "a" * (blockLen-len("email=")-3) + "@" + "aaaaa.admin"
    while True:
        #   ensure that padding chunk is what we want it to be
        p1 = o.createProfile(e1)[48:]
        if len(p1) == 16:
            break
    #   generate chunk: "uid=XXXXXX&role="
    #   we can't read id, so need to guess, but probability is with us
    #   few tries might be needed
    e1 = "b" * (blockLen - len("email=") - 3)  + "@" + "A"*11 + ".com"
    while True:
        c2 = o.createProfile(e1)[:48]
        fake = c2 + c1 + p1
        if o.logIn(fake):
            break
