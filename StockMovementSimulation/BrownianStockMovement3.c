#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include <time.h>
#include <math.h>
#include <limits.h>
#include <string.h>

int main()
{
	long double drift_year,volatility_year,volatility_day,drift_day,drift_mean,stockprice_initial,temp,temp2,stockprice[100000],prefixsum[100000];
	int i,len=0;
	printf("Enter the yearly drift in percentage : ");
	scanf("%Lg",&drift_year);
	printf("Enter the yearly volatility in percentage : ");
	scanf("%Lg",&volatility_year);
	printf("Enter initial stock price : $");
	scanf("%Lg",&stockprice_initial);
	FILE *myFile = fopen("RNG2.txt", "r");
	while (!feof(myFile))
	{
    long double number;
    fscanf(myFile, "%Lg", &number);
    prefixsum[len++]=number;
	}
	fclose(myFile);
  drift_day = drift_year/(100*252);
  volatility_day = volatility_year/(100*sqrt(252));
  drift_mean = drift_day - (0.5*pow(volatility_day,2));
  printf(" Drift mean is : %Lg\n",drift_mean);
#pragma omp parallel for default(none) private(i) shared(drift_mean,volatility_day,stockprice,stockprice_initial,len,prefixsum)
	for(i=0;i<len;i++)
	{		
		stockprice[i] = (long double)stockprice_initial*pow(2.71828,(drift_mean*(i+1))+(volatility_day*prefixsum[i]));		
	}

	for(i=0;i<len;i++)
	{
		printf("%d : %Lg\n",i,stockprice[i]);
	}
	FILE *fp = fopen("StockPrice.txt", "w");
  for(i=0;i<len-1;i++)
    {
    fprintf(fp, "%Lg", stockprice[i]);
    fprintf(fp, "\n");
    }
    fprintf(fp, "%Lg", stockprice[len-1]);
    fclose(fp);
	return 0;
}
