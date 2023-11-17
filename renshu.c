#include <stdio.h>

int main(){
  int i,l,n;
  l=0;n=0;

  printf("気温入力\n" );




  while (scanf("%d",&i )!=EOF) {
    l=l+i;
    n=n+1;

  }

  printf("平均気温＝%d度\n",l/n );

}
