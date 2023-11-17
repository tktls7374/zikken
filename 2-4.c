#include <stdio.h>
#include <string.h>

struct clip{
  int thre1;
  int thre2;
  double num[5];
};

void clipping(struct clip*l){
  int i;
  for (i = 0; i < 5; i++) {

    if ((*l).num[i]<=(*l).thre1) {
      (*l).num[i]=(*l).thre1;
    }
    else if ((*l).num[i]>=(*l).thre2) {
      (*l).num[i]=(*l).thre2;
    }
  }
}

int main(void){
  int i;
  struct clip a;

  printf("thre1=");
  scanf("%d",&a.thre1);
  printf("thre2=");
  scanf("%d",&a.thre2);

  for (i = 0; i < 5; i++) {
    printf("num[%d]=",i);
    scanf("%lf",&a.num[i]);
  }

  clipping(&a);

  printf("thre1=%d\n",a.thre1);
  printf("thre2=%d\n",a.thre2);
  for (i = 0; i < 5; i++) {
    printf("num[%d]=%lf\n",i,a.num[i]);
  }

  return 0;
}
