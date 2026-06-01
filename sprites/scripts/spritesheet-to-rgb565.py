##########
## This script reused from pimoroni-pico example repo:
## https://github.com/pimoroni/pimoroni-pico/blob/main/micropython/modules/picographics/spritesheet-to-rgb332.py
##
## Based on this thread:
## https://www.reddit.com/r/raspberrypipico/comments/zqyptx/displaying_sprites_on_pimoroni_lcd_displays_with/
##########

#!/bin/env python3
from PIL import Image
import numpy
import sys
import pathlib

# Run with `./filename.py source-image.jpg`
IMAGE_PATH = pathlib.Path(sys.argv[1])
OUTPUT_PATH = IMAGE_PATH.with_suffix(".rgb565")

CHROMAKEY = 0xF81F  # RGB565 magenta, used as transparency replacement

def image_to_data(image):
    """Generator function to convert a PIL image to 16-bit 565 RGB bytes."""
    # NumPy is much faster at doing this. NumPy code provided by:
    # Keith (https://www.blogger.com/profile/02555547344016007163)

    pb = numpy.array(image.convert('RGBA')).astype('uint16')

    r = pb[:, :, 0] >> 3   # 5 bits
    g = pb[:, :, 1] >> 2   # 6 bits
    b = pb[:, :, 2] >> 3   # 5 bits
    a = pb[:, :, 3]

    color = (r << 11) | (g << 5) | b
    color[a == 0] = CHROMAKEY
    return color.astype('>u2').flatten().tobytes()


img = Image.open(IMAGE_PATH)
w, h = img.size
data = image_to_data(img)

print(f"Converted: {w}x{h} {len(data)} bytes")

with open(OUTPUT_PATH, "wb") as f:
    f.write(data)

print(f"Written to: {OUTPUT_PATH}")
