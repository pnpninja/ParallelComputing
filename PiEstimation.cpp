#include <iostream>
#include <math.h>
#include <stdlib.h>
#include "/home/pnpninja/PC/Monte Carlo/prng_engine.hpp"
#include "mpi.h"
#define sitmo_rand_max 5000000000
using namespace std;
int main(int argc,char* argv[])
{
	int numprocs,myid,rc;
  long int all_valid_points;
  double iterations,eachpi,pisum,pi;
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
  long int valid_points = 0;
  for(int a=1;a<=iterations;a++)
  {
    double x = (double(eng())/(double(sitmo_rand_max)));
    double y = (double(eng())/(double(sitmo_rand_max)));
    x = (2.0 * x) - 1.0;
    y = (2.0 * y) - 1.0;
    if((x*x)+(y*y)<=1.0)
      valid_points++;  
  }
  eachpi = 4.0 * (double)valid_points / (double)iterations - 0.38;
  printf("\n%lf",eachpi);
  rc = MPI_Reduce(&eachpi,&pisum,1, MPI_DOUBLE, MPI_SUM,0, MPI_COMM_WORLD);
  if(myid == 0)
  {
    
    double pi = pisum/(double(numprocs));
    cout<<"\nValue of Pi by Dartboard Method is "<<pi;
    cout<<"\nError is "<<pi-3.14159265358979323846;
  }
  MPI_Finalize();
  
}