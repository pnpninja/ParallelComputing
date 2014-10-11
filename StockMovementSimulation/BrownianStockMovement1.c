#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include <time.h>
#include <limits.h>
#include <string.h>
#include "/home/pnpninja/PC/Monte Carlo/normsinv.h"

int main()
{
	int len,i;
	long double randomnum[100000];
	printf("Enter length : ");
	scanf("%d",&len);
    unsigned int myseed = rand();
#pragma omp parallel default(none) shared(randomnum,len,myseed)
    {	
    int i;    
#pragma omp for
    for(i=0; i<len; i++)
    	{
    		long double temp = ((long double)rand_r(&myseed))/((long double)RAND_MAX);
	   		randomnum[i] = normsinv(temp);	   		
    	}
            
    }
    for(i=0; i<len; i++)
    {
        printf("\n %d : %.12Lg",i,randomnum[i]);
    }
    FILE *fp = fopen("RNG.txt", "w");
    for(i=0;i<len-1;i++)
    {
    fprintf(fp, "%.12Lg", randomnum[i]);
    fprintf(fp, "\n");
    }
    fprintf(fp, "%.12Lg", randomnum[len-1]);
    fclose(fp);

}

