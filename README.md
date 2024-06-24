# Multi-threaded Sudoku Solver and Validator

## Description
A multithreaded application to validate and solve Sudoku puzzles. Phase I validates Sudoku solutions, while Phase II provides and validates solutions for puzzles with invalid entries.

## Repository Contents
- `Task.pdf`: The assignment document detailing the project phases and requirements.
- `Phase-I.c`: C program for validating a Sudoku puzzle solution using multithreading.
- `Phase-II.c`: C program for solving a Sudoku puzzle with invalid entries using multithreading.

## Assignment Phases

### Phase I: Sudoku Solution Validator
- Validates a Sudoku puzzle solution using multiple threads.
- Each thread checks:
  - Columns for digits 1 through 9.
  - Rows for digits 1 through 9.
  - Each of the nine 3x3 sub-grids for digits 1 through 9.
- Results from each thread are passed back to the main thread to determine overall puzzle validity.

### Phase II: Sudoku Puzzle Solution
- Initializes a Sudoku grid with at least two invalid entries and provides a solution.
- Threads are assigned to rows and columns to correct invalid entries.
- Uses synchronization techniques and mutex locks to ensure thread-safe operations.
- Validates the corrected puzzle and displays the result.

## Compilation and Execution
### Phase I: Sudoku Solution Validator
1. Compile the program:
   ```bash
   gcc -o phase1 Phase-I.c -pthread
    ```
2. Run the program:
  ```bash
  ./phase1
  ```
### Phase II: Sudoku Puzzle Solution
1. Compile the program:
  ```bash
  gcc -o phase2 Phase-II.c -pthread
  ```
2. Run the program:
   ```bash
   ./phase2
   ```
   
## Expected Outputs
### Phase I
- Invalid boxes in the puzzle (values and indexes).
- Invalid rows (values and indexes).
- Invalid columns (values and indexes).
- Thread ID with each invalid output.
- Cancelled thread IDs.
- Comparison results of invalid entries.
- Total count of invalid entries.
- Reason for invalidation (invalid digits or invalid locations).

### Phase II
- Original and resultant puzzle in matrix format.
- Total invalid entries.
- Threads created for each invalid entry.
- Number of moves for each invalid entry.
- IDs of all threads with their indexes.
- Location of invalid entries and their new positions.
