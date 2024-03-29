CC=gcc
CFLAGS=-Wall

all: icli.o main.o gfx.o input.o window.o scene.o grid.o heap.o search.o
	$(CC) $(CFLAGS) ./main.o ./window.o ./icli.o ./gfx.o ./grid.o ./heap.o ./input.o ./scene.o ./search.o -o path_finder -ledit -lX11 -pthread -lm 

heap.o:
	$(CC) $(CFLAGS) -c ./heap.c
grid.o:
	$(CC) $(CFLAGS) -c ./grid.c
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
search.o:
	$(CC) $(CFLAGS) -c ./search.c
clean:
	rm path_finder *.o icli_history cli.log path.dot
clean_data:
	rm icli_history cli.log path.dot
