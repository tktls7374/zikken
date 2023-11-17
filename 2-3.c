#include <stdio.h>

struct daily{
  int month;
  int day;
  double temper;

};

int main(void){
  int a;
  int b;
  double c;
  printf("月＝");
  scanf("%d",&a );
  printf("日＝");
  scanf("%d",&b );
  printf("温度＝");
  scanf("%lf",&c );



  struct daily m={
    a,b,c
  };
  printf("month=%d\n",m.month);
  printf("day=%d\n",m.day);
  printf("temper=%f\n",m.temper);

  return 0;
}
