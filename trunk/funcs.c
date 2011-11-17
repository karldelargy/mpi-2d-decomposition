/*
   *
   *
   *
   */


#include <stdio.h>
#include <stdlib.h>

void findnbrs(int comm2d, int* nbrleft, int* nbrright, int* nbrtop, int* nbrbottom)
{
//	printf("Working ok!");
	
	MPI_Cart_shift(comm2d, 0, 1, nbrleft, nbrright);
	MPI_Cart_shift(comm2d, 1, 1, nbrtop, nbrbottom);
}

