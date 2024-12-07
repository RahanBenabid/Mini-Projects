#!/usr/bin/env python3

import random

rows = 3
cols = 2

my_arr = [[0 for i in range(cols)] for j in range(rows)]

for i in range(rows):
	for j in range(cols):
		my_arr[i][j] = random.randint(0, 1)


print("Updated array:")
for row in my_arr:
	print(row)