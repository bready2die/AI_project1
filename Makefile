example: icli.o main.o graphics.o gfx.o input.o window.o scene.o
	gcc ./main.o ./window.o ./icli.o ./graphics.o ./gfx.o ./input.o ./scene.o -o path_finder -ledit -lX11

scene.o:
	gcc -Wall -g -c ./scene.c
window.o:
	gcc -Wall -g -c ./window.c
input.o:
	gcc -Wall -g -c ./input.c
graphics.o: 
	gcc -Wall -g -c ./graphics.c 
gfx.o:
	gcc -Wall -g -c ./gfx.c
icli.o: 
	gcc -Wall -g -c ./icli.c
main.o:
	gcc -Wall -g -c ./main.c
clean:
	rm path_finder *.o icli_history cli.log path.dot
