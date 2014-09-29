#include <iostream>
#include <math.h>
#include <stdlib.h>
#include "/home/pnpninja/PC/Monte Carlo/prng_engine.hpp"
#include "mpi.h"
#define sitmo_rand_max 5000000000
#define MASTER 0

int main(int argc,char* argv[])
{
	int numprocs,myid,rc;
  long int all_valid_points;
  double iterations,space;
	//Initialization code
  MPI_Init(&argc,&argv);
  MPI_Status status;
  MPI_Comm_size(MPI_COMM_WORLD,&numprocs);
  MPI_Comm_rank(MPI_COMM_WORLD,&myid);
  if(myid == 0)
  {
      cout<<"Enter the number of iterations in each thread: ";
      cin>>iterations;
  }
  MPI_Bcast(&iterations,1,MPI_DOUBLE,0,MPI_COMM_WORLD);
  sitmo::prng_engine eng(myid);
  space = double(double(1)/double(sqrt(numprocs)));
  int rem = myid%(int(sqrt(numprocs)));
  int quot = myid/(int(sqrt(numprocs)));
  long int valid_points = 0;
  for(int a=1;a<=iterations;a++)
  {
    double x = space*(double(rem))+((double(eng())/double(sitmo_rand_max))*space);
    double y = space*(double(quot))+((double(eng())/double(sitmo_rand_max))*space);
    if((x*x)+(y*y)<=1.0)
      valid_points++;  
  }
  rc = MPI_Reduce(&all_valid_points,&valid_points, 1, MPI_LONG, MPI_SUM,0, MPI_COMM_WORLD);
  if(myid == 0)
  {
    double pi = (4.0 * (double)all_valid_points / (double)(iterations*numprocs))/(double)(10000000000); 
    printf("\n %f",pi);
  }
  MPI_Finalize();
  
}