//Program to calculate Pi by integrating area of a quadrant of a circle 
//Header Files
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
  double iterations,eachintegration,integrationsum,pi,start,total_time,actualsum;
	//Initialization on MPI Variables
  MPI_Init(&argc,&argv);
  MPI_Status status;
  MPI_Comm_size(MPI_COMM_WORLD,&numprocs);
  MPI_Comm_rank(MPI_COMM_WORLD,&myid);
  //Get number of iterations in MASTER Process / Process 0
  if(myid == 0)
  {
      
      cout<<"Enter number of iterations : ";
      cin>>iterations;
      start = MPI_Wtime();
  }
  //Broadcast it to all other processes
  MPI_Bcast(&iterations,1,MPI_DOUBLE,0,MPI_COMM_WORLD);
  //Parallel Random Generator with seed as Process ID
  sitmo::prng_engine eng(MPI_Wtime());
  eachintegration=0;
  for(int a=1;a<=iterations/numprocs;a++)
  {
  	double point = (double(eng())/(double(sitmo_rand_max)));
  	eachintegration+=sqrt(1-pow(point,2));
  }
  rc = MPI_Reduce(&eachintegration,&integrationsum,1, MPI_DOUBLE, MPI_SUM,0, MPI_COMM_WORLD);
  if(myid == 0)
  {
    
    total_time = MPI_Wtime() - start;
    cout<<"\nThe value of Pi by Circle Quadrant Integration method is : "<<(4*integrationsum/iterations);
    actualsum = 3.14159265358979323846;
    cout<<"\nThe error is : "<<(4*integrationsum/iterations)-actualsum;
    cout<<"\nThe time taken is : "<<total_time;
  }
  MPI_Finalize();
}