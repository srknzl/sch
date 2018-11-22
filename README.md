# CMPE 322 Programming Project 1 Documentation

#### Author: Serkan Özel 
#### Student No: 2015400123
#### Email: serkan.ozel@boun.edu.tr

## Description 

This project is about simulation of a **preemptive priority based scheduler**. Our program's aim is to take 4 files:

1- definition.txt which includes incoming processes' names, priorities, arrival times and code files that they are executable of.

2- code1.txt

3- code2.txt

4- code3.txt

5- code4.txt

And outputs scheduling information to a file named output.txt.

## Platform used to write the program

* Ubuntu linux 18.10
* gcc/g++ 7.3.0
* CLion 
* Cmake version 3.10.2
## How to run the program:

1 - Put code files and definition files in correct format into the directory where you create the executable form main.cpp file. 

2 - After you have the files and the executable in the same directory, execute executable by :

`$ ./(exectable name)`

3 - If you don't have the executable, compile main.cpp by:

`$ g++ main.cpp`