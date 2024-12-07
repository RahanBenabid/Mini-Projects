# Conway’s game of life:

A 2d grid, they contain cells, each one can be either dead or alive, they evolve according to the rules of their world.

At the start, each cell starts the game as either “dead“ or “alive“, the rounds continue and during each one, each cell looks at its surrounding neighbours, which are 8, and counts how many of them are alive, then the cell updates itself according to them, here are the rules:
1. a live cell with 0 to 1 becomes dead because of underpopulation
2. a live cell with 2 to 3 live neighbor stays alive 
3. a live cell with more than 3 live neighbours becomes dead because of overpopulation
4. a dead cell with 3 live neighbours becomes alive