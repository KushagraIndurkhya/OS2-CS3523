compile:
		g++ sequential.cpp -o seq.o;
		g++ -pthread SrcAssgn5_coarse_CS19B1017.cpp -o c.o;
		g++ -pthread SrcAssgn5_fine_CS19B1017.cpp -o f.o;
		# g++ inp_gen.cpp -o inp.o;
generate:
		g++ inp_gen.cpp -o inp.o;
		./inp.o;
run:
		# ./inp.o;
		./seq.o;
		./c.o;
		./f.o;

run_t:
		time ./inp.o;
		time ./seq.o;
		time ./c.o;
		time ./f.o;

clean:
		rm *.o output*.txt

