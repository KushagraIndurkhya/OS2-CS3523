*Extra credit code is commented between "//*********** "
............................................................
Compiling Assgn2-RMSCS19B1017.cpp:
$ g++ Assgn2-RMSCS19B1017.cpp -o rms.o

Compiling Assgn2-EDFCS19B1017.cpp:
$ g++ Assgn2-EDFCS19B1017.cpp -o edf.o

Running Method-1:
$ ./rms.o

Running Method-2:
$ ./edf.o

OR

$ make

...............................................................

Input: Input is taken from the file inp-params.txt , consisting of the following parameters: n, the number of processes; followed by n lines consisting of 
Pi(process id), t (processing time), p (period) and k (the number of times each process repeats)

a sample input is already given in the file

................................................................

Output

For ./rms.o

Two files are generated
RMS-Log.txt   - contains the log of discrete events
RMS-Stats.txt - contains the stats (Number of processes,completed deadlines, missed deadlines, avg waiting time in seconds)

For ./edf.o

Two files are generated
EDF-Log.txt   - contains the log of discrete events
EDF-Stats.txt - contains the stats (Number of processes,completed deadlines, missed deadlines, avg waiting time in seconds)

...................................................................


