all: main.o list.o
 gcc -Wall -Werror -o run main.o list.o

main.o: main.c List.c
 gcc -c main.c

list.o: main.c List.c
 gcc -c List.c

clean:
 rm -f *.o run
