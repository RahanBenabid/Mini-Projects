#!/usr/bin/env python3

from PIL import Image
import math
import os

imageURL = './image.jpg'
# from thinnest to boldest
ASCII = "`^\",:;Il!i~+_-?][}{1)(|\\/tfjrxnuvczXYUJCLQ0OZmwqpdbkhao*#MW&8%B@$"
RATIO = (len(ASCII) - 1) /255

print(RATIO)


def average(rgb):
    return (rgb[0] + rgb[1] + rgb[2]) / 3

def lightness():
    pass
    
def luminosity():
    pass

def resize_image(img, terminal_width):
    aspect_ratio = img.height / img.width
    new_width = terminal_width
    new_height = int(new_width * aspect_ratio * 0.55)
    return img.resize((new_width, new_height))

try:
    with Image.open(imageURL) as img:
        # step 1: read the image and get the dimentins
        print(f'Image size: {img.width} x {img.height}')
        print(f'Image mode: {img.mode}')
        
        # step 2: resize based on the terminal size
        terminal_width, _ = os.get_terminal_size()
        resized_img = resize_image(img, terminal_width)
        print(f'resized image size: {resized_img.width} x {resized_img.height}')
        
        rgb_img = resized_img.convert('RGB')
        
        ascii_drawing = []
        for y in range(resized_img.height):
            for x in range(resized_img.width):
                rgb = rgb_img.getpixel((x, y))
                brightness = average(rgb)
                ascii_index = min(math.floor(brightness * RATIO), len(ASCII) - 1)
                ascii_drawing.append(ASCII[ascii_index])
            ascii_drawing.append('\n')
        ascii_drawing = ''.join(ascii_drawing)
        
        print(ascii_drawing)
        
except FileNotFoundError:
    print("Error: image not found")
except IOError:
    print("Error: invalid image file")