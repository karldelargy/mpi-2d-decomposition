/*
   *
   *
   *
   */

#include <stdio.h>
#include <stdlib.h>

#ifndef max
	#define min(x, y) (((x) < (y)) ? (x) : (y))
#endif

typedef char bool;

void decomp1d(int N, int numprocs, int rank, int* s, int* e)
{
	int nlocal, deficit;

	nlocal  = N / numprocs;
	*s = rank * nlocal + 1;
	deficit = N % numprocs;
	*s = *s + min(rank, deficit);

	if (rank < deficit) {
	      nlocal = nlocal + 1;
	}

        *e = *s + nlocal - 1;

        if (*e > N || rank == numprocs-1) {
		*e = N;
	}
}

void fndnbrs(int comm2d, int* nbrleft, int* nbrright, int* nbrtop, int* nbrbottom)
{
//	printf("Working ok!");
	
	MPI_Cart_shift(comm2d, 0, 1, nbrleft, nbrright);
	MPI_Cart_shift(comm2d, 1, 1, nbrtop, nbrbottom);
}

void fnd2ddecomp(int comm2d, int N, int* sx, int* ex, int* sy, int* ey) 
{
	int dims[2], coords[2];
	int periods[2];
	MPI_Cart_get(comm2d, 2, dims, periods, coords);

	decomp1d(N, dims[0], coords[0], sx, ex);
	decomp1d(N, dims[1], coords[1], sy, ey);
}
