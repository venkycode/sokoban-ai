# Artificial Intelligence Project

## Anton : A memory-efficient Sokoban solver

Requirements:

    - Preferable to have Linux based OS
    - Must have Tkinter and Python 3 installed

Use:

We need to first find a solution of a level using the C++ binary.

There are two binaries namely solver.out and solverPro.out.
```
solverPro.out : has strict checks for deadlocks

solver.out : has relaxed checks for deadlocks
```
Example of running the binary to find a solution:
Consider the interesting level107.
It can only be solved with strict deadlock checks as the result shown in standard_level_results.txt

Run the following command.

```
./solverPro.out < ./standard_levels/lvl107.txt &>solution.txt
```

It requires nearly 130 seconds to run 
Note: Make sure solution.txt only contains the encoded solution string. If the solver fails to find a solution it may contain extra logging info.
After finding the solution we can run the UI with the -runWithSolution flag.

Run the following command to get a live Demo of the solution to level.

```
python3 sokoban.pyw -runWithSolution <solution.txt
```


While launching the UI select file lvl107.txt from the standard_levels folder in the file selector UI.
