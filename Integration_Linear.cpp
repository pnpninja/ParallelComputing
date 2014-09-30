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
  double iterations,eachintegration,integrationsum,pi,start,total_time,actualsum,deg1,deg0,llimit,rlimit,eachllimit,eachrlimit;
  //Initialization on MPI Variables
  MPI_Init(&argc,&argv);
  MPI_Status status;
  MPI_Comm_size(MPI_COMM_WORLD,&numprocs);
  MPI_Comm_rank(MPI_COMM_WORLD,&myid);
  //Get number of iterations in MASTER Process / Process 0
  if(myid == 0)
  {
      cout<<"Enter the coefficient of degree 1: ";
      cin>>deg1;
      cout<<"Enter the constant : ";
      cin>>deg0;
      cout<<"Enter lower limit : ";
      cin>>llimit;
      cout<<"Enter upper limit : ";
      cin>>rlimit;
      cout<<"Enter number of iterations : ";
      cin>>iterations;
      start = MPI_Wtime();
  }
  //Broadcast it to all other processes
  MPI_Bcast(&deg1,1,MPI_DOUBLE,0,MPI_COMM_WORLD);
  MPI_Bcast(&deg0,1,MPI_DOUBLE,0,MPI_COMM_WORLD);
  MPI_Bcast(&llimit,1,MPI_DOUBLE,0,MPI_COMM_WORLD);
  MPI_Bcast(&rlimit,1,MPI_DOUBLE,0,MPI_COMM_WORLD);
  MPI_Bcast(&iterations,1,MPI_DOUBLE,0,MPI_COMM_WORLD);
  //Parallel Random Generator with seed as Process ID
  sitmo::prng_engine eng(MPI_Wtime());
  eachintegration=0;
  eachllimit = llimit+(((rlimit-llimit)/numprocs)*myid);
  eachrlimit = llimit+(((rlimit-llimit)/numprocs)*(myid+1));
  for(int a=1;a<=iterations/numprocs;a++)
  {
    double point = eachllimit + ((double(eng())/(double(sitmo_rand_max)))*(eachrlimit-eachllimit));
    eachintegration+=(deg1*point)+deg0;
  }
  rc = MPI_Reduce(&eachintegration,&integrationsum,1, MPI_DOUBLE, MPI_SUM,0, MPI_COMM_WORLD);
  if(myid == 0)
  {
    
    total_time = MPI_Wtime() - start;
    cout<<"\nThe integration by Monte Carlo method is : "<<((integrationsum/iterations)*(rlimit-llimit));
    actualsum = ((deg1*((pow(rlimit,2))-(pow(llimit,2))))/(double(2)))+(deg0*(rlimit-llimit));
    cout<<"\nThe error is : "<<((integrationsum/iterations)*(rlimit-llimit))-actualsum;
    cout<<"\nThe time taken is : "<<total_time;
  }
  MPI_Finalize();
}