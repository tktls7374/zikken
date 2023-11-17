#include <stdio.h>
#include <stdlib.h>

int main(void){
  int n;
  double *p;
  printf("n:");
  scanf("%d",&n);
  p=(double*)malloc(sizeof(double)*n);

  for (int i = 0; i < n; i++) {
    p[i]=(double)i;
    printf("p[%d]=%lf\n",i,p[i]);
  }
  free(p);

  return 0;
}
