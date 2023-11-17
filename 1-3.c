#include <stdio.h>

int main(void){
  double b[3];
  double *p2;
  p2=b;
  int i;
  printf("要素:\n");
  for (i = 0; i < 3; i++) {
    scanf("%lf", p2+i);

  }
  for (i = 0; i < 3; i++) {
    printf("bの値：%.1lf\n",b[i]);
  }

  return 0;
}
