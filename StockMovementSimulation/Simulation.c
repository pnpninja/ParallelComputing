/*
12it64
S.GOPINATH
PC-stock variation

use ubuntu
*/

#include <GL/glut.h>
#include <stdio.h>
#include <math.h>
#define MAXI 999999
/*int xCenter=0,yCenter=0,r=100;*/
char c, str[100]; 
double arr[10]/*={45,67,46,55,87}*/; int size=0,xwidth;
double min=MAXI,max=-1*MAXI;//,
int i=0,j=0;
double findwidth(int n)
{
  int i=0;
  while(n!=0) {i++; n/=10;}
  printf(" i= %d\n",(int)pow(10,i-1) );
  return pow(10,i-1);
}
void init(void)
{
  glClearColor(1.0,1.0,1.0,0.0);
  glMatrixMode(GL_PROJECTION);
  glOrtho(-10.0, size+1, -10,(max-min)/100 + 1,  -1.0, 1.0);
}

void setPixel()
{

  glColor3f(0.5,1,0.5);
  glBegin(GL_LINES);
    for(i=-10;i<=size+1;i+=(size/12/10))
      {
        glVertex2f(i,-1);
        glVertex2f(i,(max-min)/100 + 1);
      }
  glEnd();
    glColor3f( 1 ,0, 0);
  glBegin(GL_LINES);
     glVertex2f(-10,0);
     glVertex2f(size+1,0);
     glVertex2f(0,-1);
     glVertex2f(0,(max-min)/100 + 1);

  glEnd();
}

void Circle(){



  glClear(GL_COLOR_BUFFER_BIT);
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  setPixel();
  glColor3f( 1 ,0, 0);
  glEnable(GL_POINT_SMOOTH);
  glBegin(GL_LINE_STRIP);
    for(i=0,j=0;i<size;i++,j++)
      glVertex2f(j,(arr[i]-min)*10/xwidth);
  glEnd();

      glPointSize(10);
      glBegin(GL_POINTS);



    for(j=0;j<=size;j+=(size)/12)
      glVertex2f(j,(arr[j]-min)*10/xwidth);
  glEnd();
  glFlush();
}

int main(int argc,char **argv){
  FILE *fp=fopen("StockPrice.txt","r");
  while(1)
  {
    c = fscanf(fp,"%s",str);
    if( feof(fp) )
    {
      break ;
    }
    //printf("%s \n",str );
    arr[size++]=atof(str);
    printf("%f \n", atof(str)) ;
  }
  xwidth = findwidth(arr[0]);
  for(i=0;i<size;i++){
    if(arr[i]<min) min=arr[i];
    if(arr[i]>max) max=arr[i];

  }
   printf("size %d min=%f  max=%f \n",size,min,max);

  fclose(fp);
    glutInit(&argc,argv);
    glutInitDisplayMode(GLUT_SINGLE|GLUT_RGB);
    glutInitWindowPosition(0,0);
    glutInitWindowSize(500,500);
    glutCreateWindow("Sample Stock Price MOvement");
    init();
    glutDisplayFunc(Circle);

    glutMainLoop();
    return 0;
}
