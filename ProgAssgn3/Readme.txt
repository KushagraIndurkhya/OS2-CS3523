Input: The input to the program will be a file, named inp-params.txt, consisting of all the
parameters described above:n, k, λ1, λ2.

n: number of threads
k: number of critical section requests
λ1: exponentially distributed avg of time required in critical section
λ2: exponentially distributed avg of time required in remaining section

A sample input file is provided in root of the directory


Compiling source files: ($make Compile)
$ g++ -pthread SrcAssgn3-tas-CS19B1017.cpp -o tas.out
$ g++ -pthread SrcAssgn3-cas-CS19B1017.cpp -o cas.out
$ g++ -pthread SrcAssgn3-cas-bounded-CS19B1017.cpp -o cas_b.out


Running compiled binaries: ($make run)

$ ./tas.out
dumps logs to a file named TAS-log.txt

$ ./cas.out
dumps logs to a file named CAS-log.txt

$ ./cas_b.out
dumps logs to a file named CAS_B-log.txt


Cleanup: ($make clean)
$ rm CAS-log.txt TAS-log.txt CAS_B-log.txt *.out