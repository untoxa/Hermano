#!/usr/bin/env python3

import sys
from pathlib import Path
from optparse import OptionParser
from PIL import Image
from functools import reduce
from math import sqrt	

tiles = []
objects = []

bkg_palette  = [(0xff, 0xff, 0xff), (0xc0, 0xc0, 0xc0), (0x80, 0x80, 0x80), (0x00, 0x00, 0x00)]
def pal2bytes(palette):
    res = bytearray()
    for i in range(len(palette)):
        r, g, b = palette[i]
        res.append(r)
        res.append(g) 
        res.append(b)
    return bytes(res)

def extract_tile(pixels, x, y):
    tile = []
    for dy in range(8):
        a = 0; b = 0
        for dx in range(8):
            tile.append(pixels[(x * 8) + dx, (y * 8) + dy])
    return tile

def append_face(storage, exclude, face, dedup=True, trim=None):
    if (trim):
        if len(storage) >= int(trim):
            return True, -1
    if (exclude):
        for i in range(len(exclude)):
            if exclude[i] == face: 
                return True, -1
    if (dedup == True):
        for i in range(len(storage)):
            if storage[i] == face: 
                return True, i
    storage.append(face)
    return False, len(storage) - 1

def main(argv=None):
    parser = OptionParser("Usage: png2font.py [options] INPUT_FILE_NAME.PNG")
    parser.add_option("-o", '--out',        dest='outfilename',                                      help='output file name')
    parser.add_option("-t", '--tileset',    dest='srctileset',                                       help='source tileset name')
    parser.add_option("-j", '--objects',    dest='objtileset',                                       help='object tileset name')
    parser.add_option("-c", '--cut',        dest='cutsrctileset',                                    help='set maximum sorce tileset length')    

    (options, args) = parser.parse_args()

    if (len(args) == 0):
        parser.print_help()
        parser.error("Input file name required\n")
    
    infilename = Path(args[0])
    
    if options.outfilename == None:
        outfilename = infilename.with_suffix('.tileset.png')
    else:
        outfilename = Path(options.outfilename)
        
    if (options.objtileset):
        with Image.open(Path(options.objtileset)) as source:
            w, h = source.size
            w //= 8; h //= 8

            pixels = source.load()

            for y in range(h):
                for x in range(w):
                    tile = extract_tile(pixels, x, y)
                    append_face(objects, None, tile, False)
        print("Objects found: {}".format(len(objects)))


    if (options.srctileset):
        with Image.open(Path(options.srctileset)) as source:
            w, h = source.size
            w //= 8; h //= 8

            pixels = source.load()

            for y in range(h):
                for x in range(w):
                    tile = extract_tile(pixels, x, y)
                    append_face(tiles, objects, tile, False, options.cutsrctileset)
        print("Source tiles: {}".format(len(tiles)))
        
    with Image.open(infilename) as source:
        w, h = source.size
        w //= 8; h //= 8

        pixels = source.load()

        for y in range(h):
            for x in range(w):
                tile = extract_tile(pixels, x, y)
                append_face(tiles, objects, tile)

    print("Total tiles: {}".format(len(tiles)))


    dest_pixels = bytearray()
    for i in tiles:
        dest_pixels.extend(i)

    with Image.new('P', (8,  len(tiles) * 8)) as dest:
        dest.putdata(dest_pixels)
        dest.putpalette(pal2bytes(bkg_palette))
        dest.save(outfilename)

if __name__=='__main__':
    main()
