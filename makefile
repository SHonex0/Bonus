
all: Bonusexe
    
Bonusexe: Bonusobj
	gcc -o ./main ./main.o -L/usr/lib/x86_64-linux-gnu/ -lgsl -lgslcblas -lm 
	rm -f ./main.o
    
Bonusobj: ./src/main.c
	gcc -c -o ./main.o ./src/main.c -I/usr/include 
     
clean:
	rm -f  ./main

