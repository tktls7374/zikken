 #include <stdlib.h>
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
	char *outputfile={"output.ppm"};
	char *inputfile={"zoneplate256.ppm"};

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
    /* ここにプログラムを挿入する */
}

/* 拡大2（最近傍法） */
void scale_up_NN(ColorImage *outimage, const ColorImage *inimage)
{
	/* ここにプログラムを挿入する */
}

/* 拡大3（線形補間） */
void scale_up_IP(ColorImage *outimage, const ColorImage *inimage)
{
    /* ここにプログラムを挿入する */
}

/* 縮小1（ダウンサンプリング） */
void scale_down_DS(ColorImage *outimage, const ColorImage *inimage)
{
    /* ここにプログラムを挿入する */
}

/* 縮小2（平均値） */
void scale_down_Mean(ColorImage *outimage, const ColorImage *inimage)
{
    /* ここにプログラムを挿入する */
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
