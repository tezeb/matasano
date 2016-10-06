#!/usr/bin/python3

def matchRepetition(chunks):
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

if __name__ == "__main__":
    max_c=0
    substr=""
    max_l=""
    with open("8.txt", 'r') as myfile:
        for line in myfile:
            line = line[:-1]
            (a, b) = matchRepetition([line[i:i+32] for i in range(0, len(line)-31, 32)])
            if a > max_c:
                max_c = a
                substr = b
                max_l = line
    print(max_c, "repetition of", substr, "for possibly AES-ECB encrypted line:")
    print(max_l)
