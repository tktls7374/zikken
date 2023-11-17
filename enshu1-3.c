#include <stdio.h>

void swap(int *a,int *b) {
  int c;

  c=*a;
  *a=*b;
  *b=c;

}




int main(void) {
  int a=1;
  int b=2;
  swap(&a,&b);

  printf("a=%d\n",a);
  printf("b=%d\n",b );





  return 0;
}
