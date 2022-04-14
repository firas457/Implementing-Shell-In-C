all: SERVER.o main.o


main.o: client.o main.c
	gcc -o main.o client.o main.c


client.o:
	gcc -o client.o -c client.c


	
SERVER.o: SERVER.c
	gcc -o SERVER.o SERVER.c

clean:
	rm -rf *.o client