/*
 * A simple solution to the Case Study exercise from the EPCC MPI course.
 *
 * Communications is done using the sendrecv routine; a proper solution
 * would use non-blocking communications (ie some combination of issend/recv
 * and ssend/irecv).
 *
 * Note that the special rank of MPI_PROC_NULL is a "black hole" for
 * communications (similar to /dev/null in Unix). The use of this value
 * for processes off the edges of the image means we do not need any
 * additional logic to ensure that processes at the edges do not attempt
 * to send to or receive from invalid ranks (ie rank = -1 and rank = P).
 */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <mpi.h>

#include "pgmio.h"
#include "funcs.h"

#define M 192
#define N 360

#define P 4

#define MP M/P
#define NP N/P

#define MAXITER   1500
#define PRINTFREQ  200

#define TRUE 1
#define FALSE 0
#define ndims 2

int main (int argc, char **argv)
{
  float old[MP+2][NP+2], new[MP+2][NP+2], edge[MP+2][NP+2];

  float masterbuf[M][N];
  float buf[MP][NP];

  int i, j, iter, maxiter;
  char *filename;

  int rank, size, next, prev, tag;
//  int start, stop;//, i;
//  int left, right, up, down; //addon, passon, sum;
  int nbrleft, nbrright, nbrtop, nbrbottom;
  int sx, ex, sy, ey;

  MPI_Status status;
  MPI_Request request;
  MPI_Comm comm;

  // Topology variables
  int comm2d, direction, disp, reorder;
  int dims[ndims], period[ndims];

  comm = MPI_COMM_WORLD;
  tag = 1;

  MPI_Init(&argc, &argv);

  MPI_Comm_size(comm, &size);
//  MPI_Comm_rank(comm, &rank);

  // Cartesian topology
  dims[0] = P/2;
  dims[1] = P/2;
  period[0] = FALSE;	// Non cyclic
  period[1] = FALSE;	// Non cyclic
  reorder = FALSE;

//  direction = 0;	// Shift along the first index
  //disp = 1;		// Shift by 1

  MPI_Dims_create(size, ndims, dims);
  MPI_Cart_create(comm, ndims, dims, period, reorder, &comm2d);
  MPI_Comm_rank(comm2d, &rank);
  

  if(size != P)
    {
      if (rank == 0) printf("ERROR: size = %d, P = %d\n", size, P);
      MPI_Finalize();
      exit(-1);
    }

  next = rank + 1;
  prev = rank - 1;

  if (next >= size)
    {
      next = MPI_PROC_NULL;
    }

  if (prev < 0)
    {
      prev = MPI_PROC_NULL;
    }

  if(rank == 0)
    {
      printf("Processing %d x %d image on %d processes\n", M, N, P);
      printf("Number of iterations = %d\n", MAXITER);

      filename = "edge192x360.pgm";

      printf("\nReading <%s>\n", filename);
      pgmread(filename, masterbuf, M, N);
      printf("\n");

    }
  

  //MPI_Cart_shift(comm2d, 0, disp, &left, &right);
  //MPI_Cart_shift(comm2d, 1, disp, &up, &down);
  //printf("From rank: %d\n", rank);
  fndnbrs(comm2d, &nbrleft, &nbrright, &nbrtop, &nbrbottom);
  //printf("rank = %d, nbrleft = %d, nbrright = %d, nbrtop = %d, nbrbottom = %d\n", rank, nbrleft, nbrright, nbrtop, nbrbottom);
  fnd2ddecomp(comm2d, N, &sx, &ex, &sy, &ey);
  printf("rank = %d, sx = %d, ex = %d, sy = %d, ey = %d\n", rank, sx, ex, sy, ey);

  //MPI_Scatter(masterbuf, MP*NP, MPI_FLOAT, buf, MP*NP, MPI_FLOAT,
  //            0, MPI_COMM_WORLD);

  for (i=0;i<MP+2;i++)
    {
      for (j=0;j<NP+2;j++)
	{
	  edge[i][j]=0.0;
	}
    }
      
  for (i=1;i<MP+1;i++)
    {
      for (j=1;j<NP+1;j++)
	{
	  edge[i][j]=buf[i-1][j-1];
	}
    }

  for (i=0;i<MP+2;i++)
    {
      for (j=0;j<NP+2;j++)
	{
	  old[i][j]=edge[i][j];
	}
    }

  for (iter=1;iter<=MAXITER; iter++)
    {
      if(iter%PRINTFREQ==0)
	{
	  if(rank==0)
	    {
	      printf("Iteration %d\n", iter);
	    }
	}

//      MPI_Sendrecv(&old[MP][1], NP, MPI_FLOAT, next, 1, 
//		   &old[0][1],  NP, MPI_FLOAT, prev, 1,
//		   MPI_COMM_WORLD, &status);
//
//      MPI_Sendrecv(&old[1][1],    NP, MPI_FLOAT, prev, 2, 
//		   &old[MP+1][1], NP, MPI_FLOAT, next, 2,
//		   MPI_COMM_WORLD, &status);

      for (i=1;i<MP+1;i++)
	{
	  for (j=1;j<NP+1;j++)
	    {
	      new[i][j]=0.25*(old[i-1][j]+old[i+1][j]+old[i][j-1]+old[i][j+1]
			      - edge[i][j]);
	    }
	}
	
      for (i=1;i<MP+1;i++)
	{
	  for (j=1;j<NP+1;j++)
	    {
	      old[i][j]=new[i][j];
	    }
	}

      for (i=1;i<MP+1;i++)
	{
	  for (j=1;j<NP+1;j++)
	    {
	      buf[i-1][j-1]=old[i][j];
	    }
	}
    }

  if (rank==0)
    {
      printf("\nFinished %d iterations\n", iter-1);
    }

//  MPI_Gather(buf, MP*NP, MPI_FLOAT, masterbuf, MP*NP, MPI_FLOAT,
//	     0, MPI_COMM_WORLD);

  if (rank == 0)
    {
      filename="image192x360.pgm";
      printf("\nWriting <%s>\n", filename); 
      pgmwrite(filename, masterbuf, M, N);
    }

  MPI_Finalize();
} 
