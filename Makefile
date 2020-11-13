build:
	gcc battleship.c -lncurses -Wall -o battleship
clean:
	rm battleship
run:
	./battleship map1.dat map2.dat map3.dat
