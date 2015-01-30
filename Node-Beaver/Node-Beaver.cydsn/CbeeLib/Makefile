all: transmitorTest

transmitorTest: transmitorTest.c TransmitorSimu.o TransmitorSimu.h
	gcc -o transmitorTest transmitorTest.c TransmitorSimu.o 


TransmitorSimu.o: TransmitorSimu.c TransmitorSimu.h data.h
	gcc -c TransmitorSimu.c

clean:
	rm -rf *o transmitorTest