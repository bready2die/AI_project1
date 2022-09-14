example: icli.o main.o graphics.o gfx.o input.o
	gcc ./main.o ./icli.o ./graphics.o ./gfx.o ./input.o -o path_finder -ledit -lX11


input.o:
	gcc -c ./input.c
graphics.o: 
	gcc -c ./graphics.c 
gfx.o:
	gcc -c ./gfx.c
icli.o: 
	gcc -c ./icli.c
main.o:
	gcc -c ./main.c
clean:
	rm path_finder *.o icli_history cli.log path.dot
