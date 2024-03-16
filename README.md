# MPI Parallel Computing Assignment

This repository contains solutions for the MPI parallel computing assignment, which includes two problems: Counting Prime Numbers and Caesar Cipher Encryption.

## Problem 1: Counting Prime Numbers

### Description
The task is to write a parallel C program to count prime numbers between a given range using MPI_Send and MPI_Receive only.

### Parallelization Scenario
- Master Process:
  - Calculates the sub-range size `r = (y - x) / p`.
  - Broadcasts `x` and `r` to each slave process using MPI_Send.
  - Receives sub-count from each slave process using MPI_Receive.
  - Prints the total count of primes between `x` and `y`.

- Slave Process:
  - Receives `x` and `r` through MPI_Receive.
  - Calculates the lower bound `a` and upper bound `b` according to its rank.
  - Counts primes in its sub-range (between `a` and `b`).
  - Prints the partial count.
  - Sends this partial count to the master process using MPI_Send.

### How to Run
1. Compile the program using MPI:
   ```bash mpicc counting_primes.c -o counting_primes -lm```
Run the program using mpiexec:
`mpiexec -n <num_processes> ./counting_primes`
