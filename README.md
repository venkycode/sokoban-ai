#Artificial Intelligence Project

#Anton : A memory ifficient sokoban solver

Requirements:

    - Preferable to have linux based OS
    - Must have Tkinter and Python 3 installed

Use:

We need to first find solution of a level using the C++ binary.

There are two binaries namely main.out and main1.out.
```
main.out : has strict checks for deadlocks

main1.out : has relaxed checks for deadlocks
```
Example of running the binary to find solution:
Consider the interesting level107.
It can only be solved with strict deadlock checks as the result shown in standard_level_results.txt

Run following command.

```
./main.out < ./standard_levels/lvl107.txt &>solution.txt
```

It requires nearly 130 seconds to run 
Note: Make sure solution.txt only contains the encoted solution string. If the fails to find solution it may contain extra logging info.
After findind the solution we can run the UI with -runWithSolution flag.

Run the following command to get live Demo of solution to level.

```
python3 sokoban.pyw -runWithSolution <solution.txt
```


While launching the UI select file lvl107.txt from standard_levels folder in the file selector UI.





