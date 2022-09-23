/*
	The resulting compiled program takes 3 arguments: the width (int), the height (int) and a rate (double, in between 0 and 1).
	It then prints out text for a (hopefully) valid grid, which can then be piped into a file.
	The first 2 arguments determine the grid size, and the rate determines the amount of blocked tiles.
*/

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

int main(int argc, char** argv){
	if (argc != 4){
		printf("Wrong arg count\n");
		return 1;
	}
	int w = atoi(argv[1]);
	int h = atoi(argv[2]);
	double r = strtod(argv[3], NULL);

	if (r > 1 || w < 0 || h < 0){
		printf("Invalid args\n");
		return 1;
	}

	int area = h*w;
	double precount = (double) area * r;
	int count = (int) precount;

	srand(time(0));

	if (((double)rand()) / ((double)RAND_MAX) < precount - count){
		count++;
	}

	for (int i = 0; i < 2; i++){
		printf("%d %d\n", 1 + rand()%(w+1), 1 + rand()%(h+1));
	}

	printf("%d %d\n", w, h);
	char* arr = malloc(area*sizeof(char));
	memset(arr, 0, area*sizeof(char));

	for (int i = 0; i < count; i++){
		int cell = rand()%area;
		if (arr[cell] == (char)1){
			i--;
			continue;
		}
		arr[cell] = (char)1;
	}
	for (int i = 0; i < area; i++){
		printf("%d %d %d\n", i/h+1, i%h+1, arr[i]);
	}

	free(arr);
	//printf("%f, %f, %d\n", r, precount, count);

	return 0;
}
