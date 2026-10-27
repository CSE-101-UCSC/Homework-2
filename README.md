# CSE 101 Programming Assignment 2: AVL Airport Flight Manager

## 1. Setup

Create your own private repository from the course template, then open a Codespace on your repository to edit `avl.cpp`. Refer to the Assignment 1 setup guide (https://docs.google.com/document/d/15nf5pQMh1fvXAnq6iFQXAIGstj0MyIgf/edit?usp=sharing&ouid=117641783637175621557&rtpof=true&sd=true) for setup instructions.

If working locally, you need a C++17 compiler, make, Python 3, and Bash. On Windows, use WSL with Ubuntu.

## 2. Build, run, and test

After implementing your functions in `avl.cpp`, you can build, run and test. Run all commands from the repository root.

**Build:**

```text
make
```

This checks the provided source files and compiles `FlightManager`. The starter code builds but is intentionally incomplete. Run `make` again after every code change.

**Run manually:**

```text
./FlightManager tests/01_add_printall/input.txt --printall
./FlightManager tests/10_add_range/input.txt --range 0000 2359
```

Exactly one query flag is required: `--printall`, `--find`, `--earliest`, or `--range`. Without redirection, the program prints to the terminal.

If the AVL balance check fails, the input line number and original update where the failure happens will be shown, for example:

```text
AVL balance check failed after input line 17.
ADD UA101 1445 Seattle
```

The check uses actual subtree heights independently of your cached heights. 

**Run public tests:**

```text
./run_tests.sh            # run all 40 public cases
./run_tests.sh add_find   # run names containing "add_find"
./run_tests.sh 06         # run names containing "06"
```

If the executable is missing, the script asks you to run `make`. `run_tests.sh` uses the existing executable; it does not rebuild the program. Run `make` again after every code change. `run_tests.sh` saves combined standard output and standard error in `tests/<case>/output.txt`, compares it with `expected.txt`, and prints PASS or FAIL. The summary reports your public score out of 40 and lists failed cases; filtered runs report only the selected score. In a difference report, `-` lines are expected and `+` lines are your output. Passing public cases does not guarantee passing the hidden cases.

## 3. Clean, save, and submit

1. After passing all the public cases, fill in `citations.md`. Ensure that only `avl.cpp` and `citations.md` have been changed, and do not add additional source files.

2. Clean generated executables and case outputs:

   ```text
   make clean
   ```

3. Commit and push to your private GitHub repository by following Step 4 of the Assignment 1 setup guide, making the necessary changes for Assignment 2.

4. Open the assignment on Gradescope, choose **GitHub**, connect your account if prompted, and select your repository and branch.

5. Check the grading results. After making and pushing further changes, resubmit on Gradescope. Your latest submission is the one that counts.
