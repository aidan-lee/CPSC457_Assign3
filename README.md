# CPSC457_Assign3
An assignment completed for the Spring 2017 offering of CPSC457 at the University of Calgary.  

Simulates two process scheduling algorithms, round robin and shortest job first.

The program requires one or two command line inputs.
1) The name of the formatting file, containing the information about the processes
2) The type of scheduling algorithm to simulate.  Type RR for round robin or SJF for shortest job first.
3) The time slice, if simulating round robin.  Type an integer value.

Information Regarding the Formatting File
- Each process is represented as a line in the file
- Each process has two pieces of information, its arrival time and its burst length
 
 eg.  1 10
      3 5
      5 3
 
      This file contains information about three processes.
      - P0 arrives at time 1 and has a burst length of 10, so it will require 10 time units to finish executing
      - P1 arrives at time 3 and has a burst length of 5, so it will require 5 time units to finish executing
      - P2 arrives at time 5 and has a burst length of 3, so it will require 3 time units to finish executing
