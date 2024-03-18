#!/usr/bin/env python3

def bytes_to_arr(data, lowercase=True):
    return [format(b, '#04x' if lowercase else '#04X') for b in data]

def dump_hex(path, name):
    from binascii import b2a_hex
     
    with open(path, 'rb') as f:
        raw = f.read()
        size = len(raw)
        print("unsigned char {}[] = {{\n{}\n}};".format(name, ", ".join(bytes_to_arr(raw))))
        print("unsigned int {}_len = {};".format(name, size))

if __name__=="__main__":
    from sys import argv
    from os import path

    if not len(argv) == 2:
        exit(1)
    if not path.isfile(argv[1]):
        exit(1)



    dump_hex(argv[1], path.basename(argv[1]).replace('.', '_'))

