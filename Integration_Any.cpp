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
  double iterations,eachintegration,integrationsum,pi,start,total_time,actualsum,llimit,rlimit,coefficients[26],highest_coefficient;
	//Initialization on MPI Variables
  MPI_Init(&argc,&argv);
  MPI_Status status;
  MPI_Comm_size(MPI_COMM_WORLD,&numprocs);
  MPI_Comm_rank(MPI_COMM_WORLD,&myid);
  //Get number of iterations in MASTER Process / Process 0
  if(myid == 0)
  {
      cout<<"Enter the highest coeffecient (highest is 25): ";
      cin>>highest_coefficient;
      for(int temp=highest_coefficient;temp>=0;temp--)
      {
        printf("Enter the coefficient of degree %d : ",temp);
        cin>>coefficients[temp];
      }
      cout<<"Enter the lower limit : ";
      cin>>llimit;
      cout<<"Enter the upper limit : ";
      cin>>rlimit;
      cout<<"Enter the number of iterations : ";
      cin>>iterations;
      start = MPI_Wtime();
  }
  //Broadcast it to all other processes
  MPI_Bcast(&highest_coefficient,1,MPI_DOUBLE,0,MPI_COMM_WORLD);
	MPI_Bcast(coefficients,26,MPI_DOUBLE,0,MPI_COMM_WORLD);
  MPI_Bcast(&llimit,1,MPI_DOUBLE,0,MPI_COMM_WORLD);
  MPI_Bcast(&rlimit,1,MPI_DOUBLE,0,MPI_COMM_WORLD);
  MPI_Bcast(&iterations,1,MPI_DOUBLE,0,MPI_COMM_WORLD);
  //Parallel Random Generator with seed as Process ID
  sitmo::prng_engine eng(myid);
  eachintegration=0;
  //eachllimit = llimit+(((rlimit-llimit)/numprocs)*myid);
  //eachrlimit = llimit+(((rlimit-llimit)/numprocs)*(myid+1));
  for(int a=1;a<=iterations/numprocs;a++)
  {
  	double point = llimit + ((double(eng())/(double(sitmo_rand_max)))*(rlimit-llimit));
  	for(int b=0;b<=highest_coefficient;b++)
    {
      eachintegration+=(coefficients[b]*(pow(point,b)));
    }
  }
  rc = MPI_Reduce(&eachintegration,&integrationsum,1, MPI_DOUBLE, MPI_SUM,0, MPI_COMM_WORLD);
  if(myid == 0)
  {
    
    total_time = MPI_Wtime() - start;
    cout<<"\nThe integration by Monte Carlo method is : "<<((integrationsum/iterations)*(pow(rlimit-llimit,1)));
    actualsum=0;
    for(int c=0;c<=highest_coefficient;c++)
    {
      actualsum+=(coefficients[c]*((pow(rlimit,c+1))-(pow(llimit,c+1)))/(double(c+1)));
    }
    cout<<"\nThe error is : "<<((integrationsum/iterations)*(pow(rlimit-llimit,1)))-actualsum;
    cout<<"\nThe time taken is : "<<total_time<<"\n";
  }
  MPI_Finalize();
}
