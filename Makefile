CC=gcc
CFLAGS=-Wall -g 

example: icli.o main.o gfx.o input.o window.o scene.o
	$(CC) $(CFLAGS) ./main.o ./window.o ./icli.o ./gfx.o ./input.o ./scene.o -o path_finder -ledit -lX11

scene.o:
	$(CC) $(CFLAGS) -c ./scene.c
window.o:
	$(CC) $(CFLAGS) -c ./window.c
input.o:
	$(CC) $(CFLAGS) -c ./input.c
gfx.o:
	$(CC) $(CFLAGS) -c ./gfx.c
icli.o:
	$(CC) $(CFLAGS) -c ./icli.c
main.o:
	$(CC) $(CFLAGS) -c ./main.c
clean:
	rm path_finder *.o icli_history cli.log path.dot
clean_data:
	rm icli_history cli.log path.dot
