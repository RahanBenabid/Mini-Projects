#!/usr/bin/env python3


import random

def random_state(width: int, height: int) -> list[list[int]]:
	"""
	This function will create the board state, it will take a width and height
	It will return a boarch where each cell was initialized by either being dead or alive
	"""
	
	if width <= 0 or height <= 0:
		raise ValueError("width and height must be postive values")
	
	grid = [[random.randint(0, 1) for i in range(width)] for j in range(height)]
	
	return grid

def pretty_print(grid: list[list[int]]) -> str:
	"""
	This function prints the board state in a readable format.
	Live cells are represented by 'O' and dead cells by a space.
	"""
	
	# top border
	render = '|' + '-' * len(grid[0]) + '|\n'
	
	for row in grid:
		render += '|' + ''.join(' ' if cell == 0 else 'O' for cell in row) + '|\n'
	
	# bottom border
	render += '|' + '-' * len(grid[0]) + '|'
		
	print(render)
	return render

if __name__ == '__main__':
	grid = random_state(25, 10)
	render = pretty_print(grid)
	