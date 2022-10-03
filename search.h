#ifndef SEARCH_H
#define SEARCH_H 1

struct coords;
double h(struct coords position, char alg);

int search(double* path_cost, char alg);

#endif
