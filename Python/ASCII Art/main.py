#!/usr/bin/env python3

from PIL import Image

imageURL = './image_resized.jpg'



def average(rgb_matrix):
	brightness_matrix = []
	for rgb in rgb_matrix:
		brightness = (rgb[0] + rgb[1] + rgb[2]) / 3
		brightness_matrix.append(brightness)
	return brightness_matrix

def lightness():
	pass

def luminosity():
	pass

with Image.open(imageURL) as img:
	# step 1: read the image and get the dimentions
	print(f'Image size: {img.width} x {img.height}')
	print(img.mode)
	rgb_img = img.convert('RGB')
	rgb_matrix = []

	# step 2: load the images pixels RGB data into a 2d array
	rgb_matrix.append(rgb_img.getpixel((0, 1)))
	print(rgb_matrix)
	
	print('Iterating through pixel content...')
	for x in range(img.width):
		for y in range(img.height):
			rgb_matrix.append(rgb_img.getpixel((x, y)))
	print('RGB matrix created!')
	
	# convert the RGB matrix into a brightness matrix
	print('creating the brightness matrix...')
	brightness_matrix = average(rgb_matrix)
	print('Brightness matrix created!')
	
	