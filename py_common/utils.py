#!/usr/bin/env python3

def findMaxRepetitionsCnt(chunks):
    substr=""
    max_c=0
    for i,val in enumerate(chunks):
        c=0
        for j in chunks[i:]:
            if val == j:
                c+=1
        if c > max_c:
            max_c = c
            substr = val
    return (max_c, substr)
