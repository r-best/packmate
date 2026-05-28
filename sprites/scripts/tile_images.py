#!/usr/bin/env python3
import sys
import os
from PIL import Image


def tile_vertical(image_paths: list[str], output_path: str) -> None:
    images = [Image.open(p) for p in image_paths]
    size = images[0].size[0]
    result = Image.new("RGBA", (size, size * len(images)))
    for i, img in enumerate(images):
        result.paste(img.convert("RGBA"), (0, i * size))
    result.save(output_path)


if __name__ == "__main__":
    if len(sys.argv) < 3:
        print(f"Usage: {sys.argv[0]} output.png image1.png image2.png ...")
        sys.exit(1)
    if os.path.exists(sys.argv[1]):
        print(f"Error: {sys.argv[1]} already exists")
        sys.exit(1)
    tile_vertical(sys.argv[2:], sys.argv[1])
    print(f"Saved {sys.argv[1]}")
