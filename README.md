# cmsc858d-hw
Homeworks for CMSC858D: Algorithms, Data Structures and Inference for High-throughput Genomics (Spring 2022)

## Homework 2

The codebase for this homework is written in Python. I have used several 3rd party libaries for tasks including processing fasta files (`pysam`) and creating the suffix array (`pydivsufsort`).

To run the code, either execute `python buildsa.py` or `python querysa.py` with their respective options. The `buildsa.py` script looks for its input files in the `data` directory and the `querysa.py` looks for its input files in the `out_sa` directory.

## Homework 1

Each task can be executed by running the following in command line, once navigated to the `homework1` folder. This example is for task_1 (replace `task_1` with `task_2` or `task_3` for the other tasks):
```
cd task_1
mkdir task_1_build
cd task_1_build
cmake ../
cmake --build .
./task_1
```

Tasks 1 and 2 have a command line interface, but task 3 does not.
