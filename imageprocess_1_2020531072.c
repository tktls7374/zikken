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


/* クリッピング */
void clipping(ColorImage *cimage);
/* ネガポジ変換 */
void nega_posi(ColorImage *cimage);
/* コントラスト強調 */
void contrast(ColorImage *cimage);
/* ガンマ補正 */
void gamma_ps(ColorImage *cimage);
/* 量子化 */
void quantize(ColorImage *cimage);
/* ２値化 */
void blackwhite(ColorImage *cimage);
/* クロマキー */
void chroma_key(ColorImage *fg, ColorImage *bg);

int main(void)
{
	char *outputfile={"output.ppm"};
	char *inputfile={"peppers.ppm"};

	ColorImage cimage[2];
	int pros=0;
	/* 0:ネガポジ、1:コントラスト強調、2:ガンマ補正、
	   3:量子化、4:２値化、5：クロマキー */

	printf("0:ネガポジ\n1:コントラスト強調\n2:ガンマ補正\n3:量子化\n4:２値化\n5:クロマキー\nprocess =");
	scanf("%d",&pros);

	if(pros==0){
        // ネガポジ
        loadPPM(inputfile, cimage);
		nega_posi(cimage);
		savePPM(outputfile, cimage);
	} else if(pros==1){
        // コントラスト強調
        loadPPM(inputfile, cimage);
		contrast(cimage);
		savePPM(outputfile, cimage);
	} else if(pros==2){
        // ガンマ補正
        loadPPM(inputfile, cimage);
		gamma_ps(cimage);
		savePPM(outputfile, cimage);
	} else if(pros==3){
        // 量子化
        loadPPM(inputfile, cimage);
		quantize(cimage);
		savePPM(outputfile, cimage);
	} else if(pros==4){
        // ２値化
        loadPPM(inputfile, cimage);
		blackwhite(cimage);
		savePPM(outputfile, cimage);
	} else if(pros==5){
        // クロマキー
		loadPPM("foreground.ppm", cimage);
		loadPPM("background.ppm", &cimage[1]);
		chroma_key(cimage, &cimage[1]);
		savePPM(outputfile, cimage);
	} else{
        loadPPM(inputfile, cimage);
        savePPM(outputfile, cimage);
	}

	return 0;
}

void clipping(ColorImage *cimage)
{
  int i,j;
  for (i = 0; i < cimage -> height; i++) {
    for (j = 0; j < cimage -> width; j++) {
     if(cimage -> r[i][j] > 255){
       cimage -> r[i][j]=255;
     }
     else if(cimage -> r[i][j] < 0){
       cimage -> r[i][j]=0;
     }
     else if(cimage -> g[i][j] > 255){
       cimage -> g[i][j]=255;
     }
     else if(cimage -> g[i][j] < 0){
       cimage -> g[i][j]=0;
     }
     else if(cimage -> b[i][j] > 255){
       cimage -> b[i][j]=255;
     }
     else if(cimage -> b[i][j] < 0){
       cimage -> b[i][j]=0;
     }
    }
  }
}

void nega_posi(ColorImage *cimage)
{
    int i,j;
    for (i = 0; i < cimage -> height; i++) {
      for (j = 0; j < cimage -> width; j++) {
        cimage -> r[i][j] = 255 - cimage -> r[i][j];
        cimage -> b[i][j] = 255 - cimage -> b[i][j];
        cimage -> g[i][j] = 255 - cimage -> g[i][j];

    }
  }
}

void contrast(ColorImage *cimage)
{
  int i,j;
  float a,b;
  printf("a=");
  scanf("%f",&a );
  printf("b=");
  scanf("%f",&b );

  for (i = 0; i < cimage -> height; i++) {
    for (j = 0; j < cimage -> width; j++) {
      cimage -> r[i][j]= ((1+a)*(cimage -> r[i][j]))-b;
      cimage -> b[i][j]= ((1+a)*(cimage -> b[i][j]))-b;
      cimage -> g[i][j]= ((1+a)*(cimage -> g[i][j]))-b;

    }
  }
}

void gamma_ps(ColorImage *cimage)
{
  int i,j;
  float v;
  printf("v=");
  scanf("%f",&v );



  for (i = 0; i < cimage -> height; i++) {
    for (j = 0; j < cimage -> width; j++) {
      cimage -> r[i][j]= 255*pow(((cimage -> r[i][j])/255),v);
      cimage -> b[i][j]= 255*pow(((cimage -> b[i][j])/255),v);
      cimage -> g[i][j]= 255*pow(((cimage -> g[i][j])/255),v);

    }
  }
}

void quantize(ColorImage *cimage)
{
  int i,j;
  float n;
  printf("n=");
  scanf("%f",&n );




  for (i = 0; i < cimage -> height; i++) {
    for (j = 0; j < cimage -> width; j++) {
      cimage -> r[i][j]= (255/(pow(2,n)-1))*round(((pow(2,n)-1)/255)*(cimage -> r[i][j]));
      cimage -> b[i][j]= (255/(pow(2,n)-1))*round(((pow(2,n)-1)/255)*(cimage -> b[i][j]));
      cimage -> g[i][j]= (255/(pow(2,n)-1))*round(((pow(2,n)-1)/255)*(cimage -> g[i][j]));

    }
  }
}

void blackwhite(ColorImage *cimage)
{
  int i,j;
  float th;
  printf("th=");
  scanf("%f",&th);
  double k=(cimage -> r[i][j]+cimage -> g[i][j]+cimage -> b[i][j])/3;



  for (i = 0; i < cimage -> height; i++) {
    for (j = 0; j < cimage -> width; j++) {
      if (k<th) {
        cimage -> r[i][j]=0;
        cimage -> g[i][j]=0;
        cimage -> b[i][j]=0;
      }
      else if(k>th){
        cimage -> r[i][j]=255;
        cimage -> g[i][j]=255;
        cimage -> b[i][j]=255;
      }
    }
  }
}

// insert forground image into background.
void chroma_key(ColorImage *fg, ColorImage *bg)
{

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

/* ここから下は修正する必要はありません */

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
