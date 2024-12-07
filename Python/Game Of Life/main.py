#!/usr/bin/env python3


import random
import time
import os

def random_state(width: int, height: int) -> list[list[int]]:
	"""
	This function will create the board state, it will take a width and height
	It will return a boarch where each cell was initialized by either being dead or alive
	"""
	
	if width <= 0 or height <= 0:
		raise ValueError("width and height must be postive values")
	
	grid = [[random.randint(0, 1) for i in range(width)] for j in range(height)]
	
	return grid

def pretty_print_2(grid: list[list[int]]) -> str:
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

def pretty_print(grid: list[list[int]]) -> str:
	"""
	This function prints the board state using color and Unicode characters.
	"""
	
	# ANSI color codes
	GREEN = '\033[32m'  # green text
	RESET = '\033[0m'
	
	# Unicode characters
	LIVE_CELL = '◼'  # solid block
	DEAD_CELL = '·'  # middle dot
	
	# top border
	render = '┌' + '─' * len(grid[0]) + '┐\n'
	
	for row in grid:
		render += '│' + ''.join(f'{GREEN}{LIVE_CELL}{RESET}' if cell == 1 else DEAD_CELL for cell in row) + '│\n'
		
	# bottom border
	render += '└' + '─' * len(grid[0]) + '┘'
	
	print(render)
	return render

def next_state(grid: list[list[int]]) -> list[list[int]]:
	new_grid = [[0 for i in range(len(grid[0]))] for j in range(len(grid))]
	live_neighbour = 0
	
	directions = [
		(-1, -1), (-1, 0), (-1, 1),
		(0, -1), (0, 1),
		(1, -1), (1, 0), (1, 1)
	]
	
	for i in range(len(grid)):
		for j in range(len(grid[0])):
			live_neighbour = 0
			
			for d in directions:
				ni, nj = i + d[0], j + d[1]
				if 0 <= ni < len(grid) and 0 <= nj < len(grid[0]):
					live_neighbour += grid[ni][nj]
			
			# apply the rules:
			if grid[i][j] == 1: # alive
				if 0 <= live_neighbour <= 1:
					new_grid[i][j] = 0
				elif 2 <= live_neighbour <= 3:
					new_grid[i][j] = 1
				elif 3 < live_neighbour:
					new_grid[i][j] = 0
			else: # dead
				if live_neighbour == 3:
					new_grid[i][j] = 1
			
			
	
	#	print(new_grid)
	return new_grid

if __name__ == '__main__':
	grid = random_state(55, 10)
	pretty_print(grid)
	time.sleep(1)
	os.system('clear')
	
	while True:
		grid = next_state(grid)
		pretty_print(grid)
		time.sleep(1)
		os.system('clear')
		
	