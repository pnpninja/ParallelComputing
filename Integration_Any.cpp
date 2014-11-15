#include <iostream>
#include <math.h>
#include <stdlib.h>
#include "/home/pnpninja/PC/Monte Carlo/prng_engine.hpp"
#include "mpi.h"
#define sitmo_rand_max 4294967295
using namespace std;
double sine_reduction(int power,double lower_limit,double upper_limit)
{
	if(power==0)
		return (upper_limit - lower_limit);
	else if(power==1)
		return (-cos(upper_limit)+cos(lower_limit));
	else
		return ((-1/power)*(cos(upper_limit)*pow(sin(upper_limit),power-1)))-((-1/power)*(cos(lower_limit)*pow(sin(lower_limit),power-1)))+(((power-1)/power)*sine_reduction(power-2,lower_limit,upper_limit));
}
double cos_reduction(int power,double lower_limit,double upper_limit)
{
	if(power==0)
		return (upper_limit - lower_limit);
	else if(power==1)
		return (sin(upper_limit)-sin(lower_limit));
	else
		return ((1/power)*(sin(upper_limit)*pow(cos(upper_limit),power-1)))-((1/power)*(sin(lower_limit)*pow(cos(lower_limit),power-1)))+(((power-1)/power)*cos_reduction(power-2,lower_limit,upper_limit));
}
double tan_reduction(int power,double lower_limit,double upper_limit)
{

	if(power==0)
		return (upper_limit - lower_limit);
	else if(power ==1)
		return (-log(abs(cos(upper_limit)))+log(abs(cos(lower_limit))));
	else
		return (((1/(n-1))*pow(tan(upper_limit),n-1))-((1/(n-1))*pow(tan(lower_limit),n-1))+tan_reduction(power-2,lower_limit,upper_limit))
}
double 
int main(int argc,char* argv[])
{
	//Initialization of variables
  int numprocs,myid,rc;
  double iterations,eachintegration,integrationsum,pi,start,total_time,actualsum,llimit,rlimit,eachllimit,eachrlimit,coefficients[26],highest_coefficient,sine_coeffecients[26],sine_highest_coeffecient,cos_coeffecients[26],cos_highest_coeffecient,tan_coeffecients[26],tan_highest_coeffecients;
	//Initialization on MPI Variables
  MPI_Init(&argc,&argv);
  MPI_Status status;
  MPI_Comm_size(MPI_COMM_WORLD,&numprocs);
  MPI_Comm_rank(MPI_COMM_WORLD,&myid);
  //Get number of iterations in MASTER Process / Process 0
  if(myid == 0)
  {
      cout<<"Enter the highest coeffecient of x (highest is 25): ";
      cin>>highest_coefficient;
      for(int temp=highest_coefficient;temp>=0;temp--)
      {
        printf("Enter the coefficient of degree %d : ",temp);
        cin>>coefficients[temp];
      }
      cout<<"Enter the highest coeffecient of sine (highest is 25): ";
      cin>>sine_highest_coefficient;
      for(int temp=sine_highest_coefficient;temp>=1;temp--)
      {
        printf("Enter the coefficient of degree %d : ",temp);
        cin>>sine_coefficients[temp];
      }
      cout<<"Enter the highest coeffecient of cos (highest is 25): ";
      cin>>cos_highest_coefficient;
      for(int temp=cos_highest_coefficient;temp>=1;temp--)
      {
        printf("Enter the coefficient of degree %d : ",temp);
        cin>>cos_coefficients[temp];
      }
      cout<<"Enter the highest coeffecient of tan (highest is 25): ";
      cin>>tan_highest_coefficient;
      for(int temp=tan_highest_coefficient;temp>=1;temp--)
      {
        printf("Enter the coefficient of degree %d : ",temp);
        cin>>tan_coefficients[temp];
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
  MPI_Bcast(&sine_highest_coefficient,1,MPI_DOUBLE,0,MPI_COMM_WORLD);
  MPI_Bcast(&cos_highest_coefficient,1,MPI_DOUBLE,0,MPI_COMM_WORLD);
  MPI_Bcast(&tan_highest_coefficient,1,MPI_DOUBLE,0,MPI_COMM_WORLD);
	MPI_Bcast(coefficients,26,MPI_DOUBLE,0,MPI_COMM_WORLD);
	MPI_Bcast(sin_coefficients,26,MPI_DOUBLE,0,MPI_COMM_WORLD);
	MPI_Bcast(cos_coefficients,26,MPI_DOUBLE,0,MPI_COMM_WORLD);
	MPI_Bcast(tan_coefficients,26,MPI_DOUBLE,0,MPI_COMM_WORLD);
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
  	for(int b=0;b<=highest_coefficient;b++)
    {
      eachintegration+=(coefficients[b]*(pow(point,b)));
    }
    for(int b=1;b<=sine_highest_coefficient;b++)
    {
      eachintegration+=(sine_coefficients[b]*(pow(sin(point),b)));
    }
    for(int b=1;b<=cos_highest_coefficient;b++)
    {
      eachintegration+=(cos_coefficients[b]*(pow(cos(point),b)));
    }
    for(int b=1;b<=tan_highest_coefficient;b++)
    {
      eachintegration+=(tan_coefficients[b]*(pow(tan(point),b)));
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
    for(int c=1;c<=sine_highest_coefficient;c++)
    {
      actualsum+=(sine_coefficients[c]*sine_reduction(c,llimit,rlimit));
    }
    for(int c=1;c<=cos_highest_coefficient;c++)
    {
      actualsum+=(cos_coefficients[c]*cos_reduction(c,llimit,rlimit));
    }
    for(int c=1;c<=tan_highest_coefficient;c++)
    {
      actualsum+=(tan_coefficients[c]*tan_reduction(c,llimit,rlimit));
    }
    cout<<"\nThe error is : "<<((integrationsum/iterations)*(pow(rlimit-llimit,1)))-actualsum;
    cout<<"\nThe time taken is : "<<total_time<<"\n";
  }
  MPI_Finalize();
}
