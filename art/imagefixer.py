#!/usr/bin/env python3

import sys
from pathlib import Path
from optparse import OptionParser
from PIL import Image
from functools import reduce
from math import sqrt

bkg_palette  = [(0xff, 0xff, 0xff), (0xc0, 0xc0, 0xc0), (0x55, 0x55, 0x55), (0x00, 0x00, 0x00)]
spr_palette1 = [(0x55, 0x55, 0x55), (0xff, 0xff, 0xff), (0xaa, 0xaa, 0xaa), (0x00, 0x00, 0x00)]
spr_palette2 = [(0xaa, 0xaa, 0xaa), (0xff, 0xff, 0xff), (0x55, 0x55, 0x55), (0x00, 0x00, 0x00)]

def nearest(color, palette, transp):
    r1, g1, b1, a = color
    if (transp == True) and (a == 0):
        return 0
    distances = []
    for i in range(len(palette)):
        r2, g2, b2 = palette[i]
        distances.append(sqrt((r2-r1) ** 2 + (g2-g1) ** 2 + (b2-b1) ** 2))
    return  min(range(len(distances)), key=distances.__getitem__)

def pal2bytes(palette):
    res = bytearray()
    for i in range(len(palette)):
        r, g, b = palette[i]
        res.append(r)
        res.append(g) 
        res.append(b)
    return bytes(res)
	

def main(argv=None):
    parser = OptionParser("Usage: png2font.py [options] INPUT_FILE_NAME.PNG")
    parser.add_option("-o", '--out',        dest='outfilename',                                      help='output file name')

    (options, args) = parser.parse_args()

    if (len(args) == 0):
        parser.print_help()
        parser.error("Input file name required\n")
    
    infilename = Path(args[0])
    
    if options.outfilename == None:
        outfilename = infilename.with_suffix('.fixed.png')
    else:
        outfilename = Path(options.outfilename)
        
    palette = bkg_palette
    transparency = False

    dest_pixels = bytearray()
        
    with Image.open(infilename).convert('RGBA') as source:
        w, h = source.size

        pixels = source.load()

        for y in range(h):
            for x in range(w):
                dest_pixels.append(nearest(pixels[x, y], palette, transparency))

    with Image.new('P', (w,  h)) as dest:
        dest.putdata(dest_pixels)
        dest.putpalette(pal2bytes(palette))
        dest.save(outfilename)


if __name__=='__main__':
    main()
