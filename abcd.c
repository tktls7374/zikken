#include <stdio.h>

int main(void){
  int a;
  int *p1;
  p1=&a;

  printf("aの値は" );
  scanf("%d",&a );
  printf("aのアドレスは %p\n",&a );
  printf("p1の値は %p\n",p1 );
  printf("p1のアドレスは %p\n",&p1 );
  printf("p1の指す値は %d\n",*p1 );

  return 0;
}
