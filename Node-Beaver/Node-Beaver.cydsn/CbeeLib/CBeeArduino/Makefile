all: CBeeTest

CBeeTest: cbeetest.o CBeeArduinoLib.o
	gcc cbeetest.o CBeeArduinoLib.o -o CBeeTest

cbeetest.o: cbeetest.c
	gcc -c cbeetest.c

CBeeArduinoLib.o: CBeeArduinoLib.h CBeeArduinoLib.c
	gcc -c CBeeArduinoLib.c


clean:
	rm -rf *o CBeeTest