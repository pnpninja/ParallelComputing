#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include <time.h>
#include <limits.h>
#include <string.h>

int main()
{
	long double randomnum[2][100000],prefixsum[100000];
	int i=0,len=0;
	FILE *myFile = fopen("RNG.txt", "r");
	while (!feof(myFile))
	{
    long double number;
    fscanf(myFile, "%Lg", &number);
    randomnum[0][len++]=number;
	}
	fclose(myFile);
	int n=len, toread, size=0, j;
	long double *t;
  //printf("Enter length:");
  //scanf("%d", &n);
  //printf("Enter numbers:\n", n);
  //for(i = 0; i < n; i++)
  //  scanf("%d", &ar[0][i]);
  /*set up complement to array that holds values*/
  i = len;
  toread = 1;
  /*copy first value, since it is not copied by the code*/
  randomnum[1][0] = randomnum[0][0];
  /*following loop aims to get log2 of size, but can be avoided as in 2nd program*/
  while(i) {
    size++;
    i >>= 1;
  }
  /*following code implements algorithm*/
  for(j = 0; j < size; j++) {
    toread = !toread;
    if(toread) t = randomnum[0];
    else t = randomnum[1];
#pragma omp parallel for default(none) private(i) shared(n, j, t, randomnum, toread)
    for(i = 1; i < n; i++) {
      if(i - (1 << j) >= 0)
    t[i] = randomnum[toread][i] + randomnum[toread][i - (1 << j)];
      else t[i] = randomnum[toread][i];
    }
  }
  toread = !toread;
  FILE *fp = fopen("RNG2.txt", "w");
  for(i = 0; i < n; i++)
    printf("%.12Lg\n",randomnum[toread][i]);
  for(i=0;i<len-1;i++)
    {
    fprintf(fp, "%.12Lg", randomnum[toread][i]);
    fprintf(fp, "\n");
    }
    fprintf(fp, "%.12Lg", randomnum[toread][len-1]);
    fclose(fp);
    return 0;
}



