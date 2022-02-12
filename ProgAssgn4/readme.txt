Input: The input to the program will be a file, named input.txt, consisting of all the
parameters described above:n,X, λ , r , Γ.

n: number of customers
X: number of seats
λ: exponentially distributed avg of delay times of customer arrivals
X: number of customers arriving is uniformly distributed between 1 and r.X
Γ: exponentially distributed avg of eating time of each customer

A sample input file is provided in root of the directory


Compiling source files: ($make Compile)
$ g++ -pthread SrcAssgn4-CS19B1017.cpp -o korean_restro.out;


Running compiled binaries: ($make run)
$ ./korean_restro.out;
dumps logs to a file named log.txt

Cleanup: ($make clean)
$ rm log.txt *.out;