#! /usr/bin/env python3
from PIL import Image, ImageDraw, ImageFont

if __name__ == "__main__":
    with open("font.txt", "w") as f:
        im = Image.open("font.png")
        data = [0]*(im.size[0]*im.size[1]//8)
        print(len(data), im.size[0], im.size[1])
        pix = im.load()
        for row in range(im.size[1]//8):
            for x in range(im.size[0]):
                for y in range(8):
                    if pix[x, y+row*8] > 128:
                        data[x+im.size[0]*row] |= 1<<(y%8)

        c = 0
        for i in data:
            f.write('0x%02x, ' % i)
            c += 1
            if c == 16:
                c = 0
                f.write("\n")
