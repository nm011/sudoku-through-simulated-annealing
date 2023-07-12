# sudoku-through-simulated-annealing
 A sudoku solver which uses simulated annealing, coded in C++

## How it works 

- Takes an input sudoku puzzle string through input.txt, e.g. `040100050107003960520008000000000017000906800803050620090060543600080700250097100`

- Randomly fills out each of the 9 3x3 boxes with random numbers, with 9 distinct numbers in each box.

- At each turn, swaps two random, unfixed numbers in a random box.

- Proceeds with some probability even if cost increases. This probability decreases with more iterations.

- If cost increases over reheatPeriod, restarts with the input grid.

## How to Use

1. Compile and run the program.

```
$ g++ solver.cpp -o solver.o; ./solver.o

Input Grid : 
0 4 0 1 0 0 0 5 0 
1 0 7 0 0 3 9 6 0 
5 2 0 0 0 8 0 0 0 
0 0 0 0 0 0 0 1 7 
0 0 0 9 0 6 8 0 0 
8 0 3 0 5 0 6 2 0 
0 9 0 0 6 0 5 4 3 
6 0 0 0 8 0 7 0 0 
2 5 0 0 9 7 1 0 0 

Populated Grid : 
9 4 8 1 7 2 4 5 7 
1 3 7 6 4 3 9 6 3 
5 2 6 5 9 8 8 2 1 
1 2 9 7 8 4 9 1 7 
7 4 6 9 3 6 8 5 4 
8 5 3 2 5 1 6 2 3 
1 9 4 4 6 5 5 4 3 
6 7 8 1 8 3 7 2 6 
2 5 3 2 9 7 1 9 8 

Perfect Solution!

Final Grid :
3 4 6 1 7 9 2 5 8 
1 8 7 5 2 3 9 6 4 
5 2 9 6 4 8 3 7 1 
9 6 5 8 3 2 4 1 7 
4 7 2 9 1 6 8 3 5 
8 1 3 7 5 4 6 2 9 
7 9 8 2 6 1 5 4 3 
6 3 1 4 8 5 7 9 2 
2 5 4 3 9 7 1 8 6 

Cost = 0
```

 Final Cost might be greater than zero. 
 
You can test the solver with Sudoku puzzles from the [Kaggle Sudoku dataset](https://www.kaggle.com/datasets/bryanpark/sudoku).

## To Do: 

- [ ] Find Optimal reheatPeriod and iterations through testing with different datasets.
- [ ] Check if SEED parameter can be adjusted for better performance.
 
## Reference

[Metaheuristics can solve Sudoku puzzles](https://www.researchgate.net/publication/220403361_Metaheuristics_can_solve_Sudoku_puzzles)