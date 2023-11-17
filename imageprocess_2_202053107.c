#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <ctype.h>

/* 画像の構造体 */
typedef struct{
    int width;
    int height;
    float **r;
    float **g;
    float **b;
}ColorImage;

/* 1ワード読み込む関数 */
void getword(FILE *fp, char *word);
/* PPMフォーマットの画像を読み込む関数 */
void loadPPM(char* filename, ColorImage *ci);
/* 画像をPPMフォーマットで書き出す関数 */
void savePPM(char* filename, ColorImage *ci);
/* 出力画像の変数を生成する関数*/
void createImage(ColorImage *ci);
/* 画像変数のメモリを開放する関数*/
void freeImage(ColorImage *ci);
/* 丸め込み関数*/
float myround(float val);

/* クリッピング */
void clipping(ColorImage *cimage);
/* 拡大1（アップサンプリング） */
void scale_up_US(ColorImage *outimage, const ColorImage *inimage);
/* 拡大2（最近傍法） */
void scale_up_NN(ColorImage *outimage, const ColorImage *inimage);
/* 拡大3（線形補間） */
void scale_up_IP(ColorImage *outimage, const ColorImage *inimage);
/* 縮小1（ダウンサンプリング） */
void scale_down_DS(ColorImage *outimage, const ColorImage *inimage);
/* 縮小2（平均値） */
void scale_down_Mean(ColorImage *outimage, const ColorImage *inimage);
/* トリミング */
void trimming(ColorImage *outimage, const ColorImage *inimage, int ypos, int xpos);

int main(void)
{
	// 変数宣言
	char *outputfile={"output1.ppm"};
	char *inputfile={"lena256.ppm"};

	ColorImage inimage, outimage;
	int pros = 0;
	float scaleH, scaleW;
	int xpos, ypos;

	/* 0:拡大1（アップサンプリング）, 1:拡大2（最近傍法）, 2:拡大3（線形補間）,
	   3:縮小1（ダウンサンプリング）, 4:縮小2（平均値）  , 5:トリミング      */
	printf("\n0:拡大1（アップサンプリング）, \n1:拡大2（最近傍法）, \n2:拡大3（線形補間）, \n3:縮小1（ダウンサンプリング）, \n4:縮小2（平均値）, \n5:トリミング\nprocess =");
	scanf("%d",&pros);

	// ここから処理
	loadPPM(inputfile, &inimage);

	if(pros==0) {
		/* 拡大1（アップサンプリング） */
    scaleH=2; scaleW=2;
		outimage.width = scaleW*(inimage.width);
		outimage.height = scaleH*(inimage.height);
		createImage(&outimage);
		scale_up_US(&outimage, &inimage);
	}
	else if(pros==1) {
		/* 拡大2（最近傍法） */
		scaleH=2; scaleW=2;
		outimage.width = scaleW*(inimage.width);
		outimage.height = scaleH*(inimage.height);
		createImage(&outimage);
		scale_up_NN(&outimage, &inimage);
	}
	else if(pros==2) {
		/* 拡大3（線形補間） */
		scaleH=2; scaleW=2;
		outimage.width = scaleW*(inimage.width);
		outimage.height = scaleH*(inimage.height);
		createImage(&outimage);
		scale_up_IP(&outimage, &inimage);
	}
	else if(pros==3) {
		/* 縮小1（ダウンサンプリング） */
		scaleH=1.0/2; scaleW=1.0/2;
		outimage.width = scaleW*(inimage.width);
		outimage.height = scaleH*(inimage.height);
		createImage(&outimage);
		scale_down_DS(&outimage, &inimage);
	}
	else if(pros==4) {
		/* 縮小2（平均値） */
		scaleH=1.0/2; scaleW=1.0/2;
		outimage.width = scaleW*(inimage.width);
		outimage.height = scaleH*(inimage.height);
		createImage(&outimage);
		scale_down_Mean(&outimage, &inimage);
	}
	else {
		/* トリミング */
		outimage.width = 64; outimage.height = 64;
		xpos = 32; ypos = 32;
		createImage(&outimage);
		trimming(&outimage, &inimage, ypos, xpos);
	}

	savePPM(outputfile, &outimage);
	freeImage(&inimage);
	freeImage(&outimage);
	return 0;
}

/* 拡大1（アップサンプリング） */
void scale_up_US(ColorImage *outimage, const ColorImage *inimage)
{
  int m;
  int n;

  m = outimage->height/inimage->height;
  n = outimage->width/inimage->width;

  for (int i = 0; i < outimage->height; i++) {
    for (int j = 0; j < outimage->width; j++) {
      outimage -> r[i][j] = inimage -> r[i/m][j/n];
      outimage -> b[i][j] = inimage -> b[i/m][j/n];
      outimage -> g[i][j] = inimage -> g[i/m][j/n];
    }
  }


}

/* 拡大2（最近傍法） */
void scale_up_NN(ColorImage *outimage, const ColorImage *inimage)
{

  for (int i = 0; i < outimage->width; i++) {
    for (int j = 0; j < outimage->height; j++) {
      outimage->width=2*inimage->width;
      outimage->height=2*inimage->height;
      outimage -> r[i][j] = inimage -> r[2*i][2*j];
      outimage -> b[i][j] = inimage -> b[2*i][2*j];
      outimage -> g[i][j] = inimage -> g[2*i][2*j];
    }
  }
  for (int i = 0; i < outimage->width; i++) {
    for (int j = 0; j < outimage->height; j++) {
      outimage -> r[2*i+1][2*j+1] = inimage -> r[i+1/2][j+1/2];
      outimage -> b[2*i+1][2*j+1] = inimage -> b[i+1/2][j+1/2];
      outimage -> g[2*i+1][2*j+1] = inimage -> g[i+1/2][j+1/2];
    }
  }




}

/* 拡大3（線形補間） */
void scale_up_IP(ColorImage *outimage, const ColorImage *inimage)
{
    int i,j,k,m,n;
    m=i/k;
    n=j/k;

    printf("倍数=");
    scanf("%d",&k);
    outimage->width=inimage->width*k;
    outimage->height=inimage->height*k;

    for (int i = 0; i < outimage->width; i++) {
      for (int j = 0; j < outimage->height; j++) {
        if (m<inimage->width-1) {
          m=inimage->width-2;
        }
        if (n<inimage->height-1) {
          n=inimage->height-2;
        }
        float x=(i/k)-m;
        float y=(j/k)-n;
        outimage -> r[i][j] = (a*b*inimage -> r[m+1][n+1])+((1-a)*b*inimage -> r[m+1][n])+(a*(1-b)*inimage -> r[m][n+1])+((1-a)*(1-b)*inimage -> r[m][n]);
        outimage -> b[i][j] = (a*b*inimage -> b[m+1][n+1])+((1-a)*b*inimage -> b[m+1][n])+(a*(1-b)*inimage -> b[m][n+1])+((1-a)*(1-b)*inimage -> b[m][n]);
        outimage -> g[i][j] = (a*b*inimage -> g[m+1][n+1])+((1-a)*b*inimage -> g[m+1][n])+(a*(1-b)*inimage -> g[m][n+1])+((1-a)*(1-b)*inimage -> g[m][n]);
      }
    }
}

/* 縮小1（ダウンサンプリング） */
void scale_down_DS(ColorImage *outimage, const ColorImage *inimage)
{
  for (int i = 0; i < outimage->width; i++) {
    for (int j = 0; j < outimage->height; j++) {
      outimage->width=inimage->width/2;
      outimage->height=inimage->height/2;
      outimage -> r[2*i][2*j] = inimage -> r[i][j];
      outimage -> b[2*i][2*j] = inimage -> b[i][j];
      outimage -> g[2*i][2*j] = inimage -> g[i][j];
    }
  }
}

/* 縮小2（平均値） */
void scale_down_Mean(ColorImage *outimage, const ColorImage *inimage)
{
  int i,j,k,l,m;
  float red=0;
  float green=0;
  float blue=0;

  printf("何倍＝");
  scanf("%d",&k);
  outimage->width=inimage->width/k;
  outimage->height=inimage->height/k;

  for (i = 0; i < outimage->width; i++) {
    for (j = 0; j < outimage->height; j++) {
      for (l = 0; l<k; l++) {
        for (m = 0; m < k; m++) {
          int x=(j*k)+l;
          int y=(j*k)+m;
          if (x<inimage->width) {
            x=inimage->width-1;
          }
          if (x<inimage->height) {
            x=inimage->height-1;
          }
          red += inimage->r[x][y];
          green += inimage->g[x][y];
          blue += inimage->b[x][y];

        }
      }
      outimage->r[i][j]=red/(k*k);
      outimage->g[i][j]=green/(k*k);
      outimage->b[i][j]=blue/(k*k);
    }
  }
}







  double k=(inimage -> r[i][j]+inimage -> r[i+1][j]+inimage -> r[i][j+1]+inimage -> r[i+1][j+1])/4;
  double l=(inimage -> b[i][j]+inimage -> b[i+1][j]+inimage -> b[i][j+1]+inimage -> b[i+1][j+1])/4;
  double m=(inimage -> g[i][j]+inimage -> g[i+1][j]+inimage -> g[i][j+1]+inimage -> g[i+1][j+1])/4;

  for (i = 0; i < outimage->width; i++) {
    for (j = 0; j < outimage->height; j++) {
      outimage->width=(1/2)*inimage->width;
      outimage->height=(1/2)*inimage->height;
      outimage -> r[i][j] = k;
      outimage -> b[i][j] = l;
      outimage -> g[i][j] = m;
    }
  }

}

/* トリミング */
void trimming(ColorImage *outimage, const ColorImage *inimage, int ypos, int xpos)
{
    /* ここにプログラムを挿入する */
}

/* ここから下は修正する必要はありません */

void clipping(ColorImage *cimage)
{
  int i,j;

	/* ここに画素値を0～255に制限するプログラムを挿入する */
	for(i=0;i<cimage->height;i++){
		for(j=0;j<cimage->width;j++){
            if(cimage->r[i][j] > 255.0f) cimage->r[i][j]=255.0f;
            if(cimage->g[i][j] > 255.0f) cimage->g[i][j]=255.0f;
            if(cimage->b[i][j] > 255.0f) cimage->b[i][j]=255.0f;
            if(cimage->r[i][j] < 0.0f) cimage->r[i][j]=0.0f;
            if(cimage->g[i][j] < 0.0f) cimage->g[i][j]=0.0f;
            if(cimage->b[i][j] < 0.0f) cimage->b[i][j]=0.0f;
		}
	}
}

void createImage(ColorImage *ci)
{
	int i,j;

	ci->r=(float **)malloc(ci->height*sizeof(float *));
	ci->g=(float **)malloc(ci->height*sizeof(float *));
	ci->b=(float **)malloc(ci->height*sizeof(float *));
	if(ci->r==NULL||ci->g==NULL||ci->b==NULL) {
		printf("Error in malloc");
		exit(0);
	}

	for(i=0;i<ci->height;i++){
		ci->r[i]=(float *)malloc(ci->width*sizeof(float));
		ci->g[i]=(float *)malloc(ci->width*sizeof(float));
		ci->b[i]=(float *)malloc(ci->width*sizeof(float));
		if(ci->r[i]==NULL||ci->g==NULL||ci->b==NULL) {
			printf("Error in malloc");
			exit(0);
		}
	}

	for(i=0;i<ci->height;i++){
		for(j=0;j<ci->width;j++){
			ci->r[i][j]=0;
			ci->g[i][j]=0;
			ci->b[i][j]=0;
		}
	}
}

void savePPM(char* filename, ColorImage *ci)
{
	/* save file in PGM P3 format */
	int i, j;
	FILE *out;

	/* ファイルをオープン */
	out=fopen(filename, "w");

	/* ファイルが存在しなければエラー */
	if (out==NULL){
		printf("Cannot open file\n");
		exit(0);
	}

	/* ファイルのヘッダ情報を書き出す */
	fprintf(out, "P3\n%d %d\n255\n", ci->width, ci->height);

    /* [0, 255]に画素値をクリッピング */
	clipping(ci);

	/* 画素値を書き出す */
	for(i=0;i<ci->height;i++){
		for(j=0;j<ci->width;j++){
			fprintf(out, "%d ", (int)ci->r[i][j]);
			fprintf(out, "%d ", (int)ci->g[i][j]);
			fprintf(out, "%d ", (int)ci->b[i][j]);
		}
	}

	fclose(out);
}

void loadPPM(char* filename, ColorImage *ci)
{
	char word[1000];
	int i,j;
	FILE *in;

	in=fopen(filename, "rb");

	if(in==NULL){
		printf("Cannot open file\n");
		exit(0);
	}

	getword(in, word);

	getword(in, word);
	sscanf(word,"%d",&(ci->width));
	getword(in, word);
	sscanf(word,"%d",&(ci->height));
	getword(in, word);

	ci->r=(float **)malloc(ci->height*sizeof(float *));
	ci->g=(float **)malloc(ci->height*sizeof(float *));
	ci->b=(float **)malloc(ci->height*sizeof(float *));
	if(ci->r==NULL||ci->g==NULL||ci->b==NULL) {
		printf("Error in malloc");
		exit(0);
	}

	for(i=0;i<ci->height;i++){
		ci->r[i]=(float *)malloc(ci->width*sizeof(float));
		ci->g[i]=(float *)malloc(ci->width*sizeof(float));
		ci->b[i]=(float *)malloc(ci->width*sizeof(float));
		if(ci->r[i]==NULL||ci->g==NULL||ci->b==NULL) {
			printf("Error in malloc");
			exit(0);
		}
	}

	/*discard one CR*/
	fgetc(in);

	for(i=0;i<ci->height;i++){
		for(j=0;j<ci->width;j++){
			int tmpr, tmpg, tmpb;
			fscanf(in, "%s", word); sscanf(word,"%d",&(tmpr));
			fscanf(in, "%s", word); sscanf(word,"%d",&(tmpg));
			fscanf(in, "%s", word); sscanf(word,"%d",&(tmpb));

			ci->r[i][j]=(float)tmpr;
			ci->g[i][j]=(float)tmpg;
			ci->b[i][j]=(float)tmpb;
		}
	}

	fclose(in);
}

void getword(FILE *fp, char *word)
{
	fscanf(fp, "%s", word);

	/*	while(word[0]=='#'){
		fscanf(fp, "%s", word);
		}*/
	if(word[0]=='#'){
		while(word[0]!=10){
			word[0]=(int)fgetc(fp);
		}
		fscanf(fp, "%s", word);
	}
}

void freeImage(ColorImage *ci)
{
	int i;

	for(i=0;i<ci->height;i++)
	{
		free(ci->r[i]);
		free(ci->g[i]);
		free(ci->b[i]);
	}
	free(ci->r);
	free(ci->g);
	free(ci->b);
}

float myround(float val)
{
	float fval=0.0;
	fval = floor(val + 0.5);
	return fval;
}
