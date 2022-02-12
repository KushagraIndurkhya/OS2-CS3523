Input:
Input to the program will be a file, named input_params.txt, consisting of the number of
threads followed by the no. of vertices in the graph. From the next line, there should be the
representation of the graph in adjacency matrix format. The sample input file is as follows:

2 5
1 2 3 4 5
1 0 0 0 1 0
2 0 0 0 1 1
3 0 0 0 0 1
4 1 1 0 0 1
5 0 1 1 1 0

Large inputs can be generated using a helper script inp_gen.cpp

N,K can be specified in line 18,19 in inp_gen..cpp

$ g++ inp_gen.cpp -o inp.o;
$ ./inp.o;

                    OR
$ make generate

Coarse Grained:

Compiling:
$ g++ -pthread SrcAssgn5_coarse_CS19B1017.cpp -o c.o

Run:
./c.o


Output:
an output file named output-c.txt is generated containing the
number of colours used, the time taken and the colour of each vertex for each variant.


Sample Output file:
Coarse-grained Lock
Colors:
v1-0, v2-1, v3-1, v4-2, v5-0, 
Time taken by the algorithm using: 0 Millisecond
No. of colours used: 3



Fine Grained:


Compiling:
$ g++ -pthread SrcAssgn5_fine_CS19B1017.cpp -o f.o


Run:
$ ./f.o


Output:
an output file named output-f.txt is generated containing the
number of colours used, the time taken and the colour of each vertex for each variant.


Sample Output file:
fine-grained Lock
Colors:
v1-0, v2-1, v3-1, v4-2, v5-0, 
Time taken by the algorithm using: 0 Millisecond
No. of colours used: 3

                                                OR
#Compile
$ make
#generate input
$ make generate
#Run
$ make run

