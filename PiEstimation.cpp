#include <iostream>
#include <math.h>
#include <stdlib.h>
#include "/home/pnpninja/PC/Monte Carlo/prng_engine.hpp"
#include "mpi.h"
#define sitmo_rand_max 4294967295
using namespace std;

int main(int argc,char* argv[])
{
  //Initialization of variables
  int numprocs,myid,rc;
  long int all_valid_points;
  double iterations,eachpi,pisum,pi,start,total_time;
	//Initialization on MPI Variables
  MPI_Init(&argc,&argv);
  MPI_Status status;
  MPI_Comm_size(MPI_COMM_WORLD,&numprocs);
  MPI_Comm_rank(MPI_COMM_WORLD,&myid);
  //Get number of iterations in MASTER Process / Process 0
  if(myid == 0)
  {
      cout<<"Enter the number of iterations in each thread: ";
      cin>>iterations;
      start = MPI_Wtime();
  }
  //Broadcast it to all other processes
  MPI_Bcast(&iterations,1,MPI_DOUBLE,0,MPI_COMM_WORLD);
  //Parallel Random Generator with seed as MPI_Wtime()
  sitmo::prng_engine eng(myid);
  long int valid_points = 0;
  //Calculate how many points are inside
  for(int a=1;a<=iterations;a++)
  {
    double x = (double(eng())/(double(sitmo_rand_max)));
    double y = (double(eng())/(double(sitmo_rand_max)));
    x = (2.0 * x) - 1.0;
    y = (2.0 * y) - 1.0;
    if((x*x)+(y*y)<=1.0)
      valid_points++;  
  }
  //Calculate each Pi generated in each process and print it
  eachpi = 4.0 * (double)valid_points / (double)iterations;
  printf("\nThread %d : %lf",myid,eachpi);
  rc = MPI_Reduce(&eachpi,&pisum,1, MPI_DOUBLE, MPI_SUM,0, MPI_COMM_WORLD);
  if(myid == 0)
  {
    //Average of Pi
    double pi = pisum/(double(numprocs));
    total_time = MPI_Wtime() - start;
    cout<<"\nValue of Pi by Dartboard Method is "<<pi;
    cout<<"\nError is : "<<pi-3.14159265358979323846;
    cout<<"\nTime of execution : "<<total_time<<"\n";
  }
  MPI_Finalize();
  
}