loop: 
	#g++ -Wno-write-strings dual_chan.c -lncurses -L./usr/lib64/ -livivisa -o loop.o 
	g++ -Wno-write-strings dc_arb.c alb_tools.cxx -L./usr/lib64/ -livivisa -o arb.o 
	#g++ -Wno-write-strings dc_func.c -lncurses -L./usr/lib64/ -livivisa -o arb.o 
manual: 
	g++ -Wno-write-strings dual_chan_manual.c -lncurses -L./usr/lib64/ -livivisa -o manual.o
err: 
	g++ -Wno-write-strings errors.c -lncurses -L./usr/lib64/ -livivisa -o err.o 
run:
	g++ -Wno-write-strings run.c -L./usr/lib64/ -livivisa -o run.o
 
test:
	g++ test.c -L./usr/lib64/ -lncurses -livivisa -o test.o
	./test.o
 
clean:
	rm *.o
