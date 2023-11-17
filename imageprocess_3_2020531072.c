/*
	2023年度第6回のプログラム
	imageprocess_3.c
*/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <ctype.h>

/* カラー画像の構造体 */
typedef struct {
    int width;
    int height;
    float** r;
    float** g;
    float** b;
}ColorImage;

/* 濃淡画像の構造体 */
typedef struct {
    int width;
    int height;
    float** x;
}GrayImage;

/* １ワード読み込む関数 */
void getword(FILE* fp, char* word);
/* PGMフォーマットの画像を読み込む関数 */
void loadPGM(char* filename, GrayImage* gi);
/* 画像をPGMフォーマットで書き出す関数 */
void savePGM(char* filename, GrayImage* gi);
/* PPMフォーマットの画像を読み込む関数 */
void loadPPM(char* filename, ColorImage* ci);
/* 画像をPPMフォーマットで書き出す関数 */
void savePPM(char* filename, ColorImage* ci);
/* 出力画像の変数を生成する関数*/
void createColorImage(ColorImage* ci); // 濃淡画像用
void createGrayImage(GrayImage* gi); // カラー画像用
/* 画像の領域を開放する関数 */
void freeColorImage(ColorImage* ci); // 濃淡画像用
void freeGrayImage(GrayImage* gi); // カラー画像用
/* 丸め込み関数*/
float myround(float val);
/* クリッピング */
void clipping(ColorImage *cimage);

/* 平滑化フィルタ（平均） */
void average_filter(ColorImage* outimage, ColorImage* inimage, int ave_window);
/* 水平方向と垂直方向のソーベルフィルタ（dX, dY, エッジ強度） */
void sobel_filter(GrayImage* gimage, GrayImage* dX, GrayImage* dY, GrayImage* Edge);
/* ラプラシアンフィルタ */
void laplacian_filter(GrayImage* gimage, GrayImage* Edge);
/* メディアンフィルタ */
void median_filter(ColorImage* outimage, ColorImage* inimage, int ave_window);
/* 水平方向と垂直方向の微分フィルタ（dX, dY, エッジ強度） */
void derivative_filter(GrayImage* gimage, GrayImage* dX, GrayImage* dY, GrayImage* Edge);


int main(int argc, const char* argv[]) {
    /* 変数宣言 */
    ColorImage cinimage, coutimage;
    GrayImage ginimage, Edge;
    char* inputfile, * outputfile;
    int win_size = 3; // フィルタサイズ

    int pros = 0;
    /*	0:平滑化フィルタ（平均）,
        1:ソーベルフィルタ（dX, dY, エッジ強度）
		2:ラプラシアンフィルタ
        3:メディアンフィルタ
		4:微分フィルタ（dX, dY, エッジ強度）,
	*/
	printf("\n0:平滑化フィルタ（平均）");
	printf("\n1:ソーベルフィルタ（dX, dY, エッジ強度）");
    printf("\n2:ラプラシアンフィルタ");
    printf("\n3:メディアンフィルタ");
	printf("\n4:微分フィルタ（dX, dY, エッジ強度）");
	printf("\npros = ");
    scanf("%d", &pros);

    if (pros == 0) {
        /* 平滑化フィルタ */
        inputfile = "lena256.ppm";
        outputfile = "average.ppm";
        /* PPMファイルを読み込む */
        loadPPM(inputfile, &cinimage);
        /* 平滑化結果を格納するカラー画像のメモリ確保 */
        coutimage.width = cinimage.width;
        coutimage.height = cinimage.height;
        createColorImage(&coutimage);

        /* 平滑化フィルタ（平均） */
        average_filter(&coutimage, &cinimage, win_size);

        /* 結果画像をPPMファイルとして出力 */
        savePPM(outputfile, &coutimage);
        /* 結果画像用のメモリを解放 */
        freeColorImage(&coutimage);
    } else if (pros == 1) {
        /* ソーベルフィルタ（dX, dY, エッジ強度） */
        /* dX:水平方向の微分, dY:垂直方向の微分, Edge:エッジ */
        GrayImage dX, dY;
        char* outputfile_x;
        char* outputfile_y;
        inputfile = "lenagray.pgm";
        outputfile_x = "dx.pgm";
        outputfile_y = "dy.pgm";
        outputfile = "Edge.pgm";
        /* "inputfile.pgm"PGMファイルを読み込む */
        loadPGM(inputfile, &ginimage);
        /* ソーベルフィルタの出力を格納する配列の領域を確保する */
        dX.width = (ginimage.width); dX.height = (ginimage.height);
        dY.width = (ginimage.width); dY.height = (ginimage.height);
        Edge.width = (ginimage.width); Edge.height = (ginimage.height);
        createGrayImage(&dX);
        createGrayImage(&dY);
        createGrayImage(&Edge);

        /* ソーベルフィルタの出力を求める */
        sobel_filter(&ginimage, &dX, &dY, &Edge);

        savePGM(outputfile_x, &dX);
        savePGM(outputfile_y, &dY);
        savePGM(outputfile, &Edge);
        /*メモリ領域を開放する*/
        freeGrayImage(&dX);
        freeGrayImage(&dY);
        freeGrayImage(&Edge);
        freeGrayImage(&ginimage);
    } else if (pros == 2) {
        /* ラプラシアンフィルタ */
        inputfile = "lenagray.pgm";
        outputfile = "laplacian_.pgm";
        loadPGM(inputfile, &ginimage);
        Edge.width = ginimage.width; Edge.height = ginimage.height;
        createGrayImage(&Edge);

        laplacian_filter(&ginimage, &Edge);

        savePGM(outputfile, &Edge);
        freeGrayImage(&ginimage);
        freeGrayImage(&Edge);
    } else if (pros == 3) {
        /* メディアンフィルタ */
        inputfile = "lena256.ppm";
        outputfile = "median.ppm";
        /* PPMファイルを読み込む */
        loadPPM(inputfile, &cinimage);
        /* 平滑化結果を格納するカラー画像のメモリ確保 */
        coutimage.width = cinimage.width;
        coutimage.height = cinimage.height;
        createColorImage(&coutimage);

        /* 平均値に基づく平滑化フィルタリング */
        median_filter(&coutimage, &cinimage, win_size);

        /* 結果画像をPPMファイルとして出力 */
        savePPM(outputfile, &coutimage);
        /* 結果画像用のメモリを解放 */
        freeColorImage(&coutimage);
    } else if (pros == 4) {
        /* 微分フィルタ（dX, dY, エッジ強度） */
        /* dX:水平方向の微分, dY:垂直方向の微分, Edge:エッジ */
        GrayImage dX, dY;
        char* outputfile_x, * outputfile_y;
        inputfile = "lenagray.pgm";
        outputfile_x = "dx.pgm";
        outputfile_y = "dy.pgm";
        outputfile = "Edge.pgm";
        /* "inputfile.pgm"PGMファイルを読み込む */
        loadPGM(inputfile, &ginimage);
        /* 微分フィルタの出力を格納する配列の領域を確保する */
        dX.width = (ginimage.width); dX.height = (ginimage.height);
        dY.width = (ginimage.width); dY.height = (ginimage.height);
        Edge.width = (ginimage.width); Edge.height = (ginimage.height);
        createGrayImage(&dX);
        createGrayImage(&dY);
        createGrayImage(&Edge);

        /* 微分フィルタの出力を求める */
        derivative_filter(&ginimage, &dX, &dY, &Edge);

        savePGM(outputfile_x, &dX);
        savePGM(outputfile_y, &dY);
        savePGM(outputfile, &Edge);
        /*メモリ領域を開放する*/
        freeGrayImage(&dX);
        freeGrayImage(&dY);
        freeGrayImage(&Edge);
        freeGrayImage(&ginimage);
    }


    return 0;
}

void average_filter(ColorImage* outimage, ColorImage* inimage, int win_size) {

int half=ave_window/2;

  for (int i = 0; i < inimage->height; i++) {
    for (int j = 0; j < inimage->width; j++) {
      float sum_r=0;
      float sum_g=0;
      float sum_b=0;
      for (int k = -half; k < half; k++) {
        for (int l = -half; l < half; l++) {
          int m = i + k;
          int n = j + l;
          if (m>=0 && m<inimage->height && n>=0 && n<inimage->width) {
            sum_r += inimage_>r[m][n];
            sum_g += inimage_>g[m][n];
            sum_b += inimage_>b[m][n];
          }
        }
      }
      outimage->r[i][j]=sum_r/(ave_window*ave_window);
      outimage->b[i][j]=sum_b/(ave_window*ave_window);
      outimage->g[i][j]=sum_g/(ave_window*ave_window);
    }
  }
}

void sobel_filter(GrayImage* gimage, GrayImage* dX, GrayImage* dY, GrayImage* Edge)
{
    /* ここにプログラムを挿入する */
    /* dXに水平方向の微分結果をdYに垂直方向の微分結果を格納する。
        また、それらをもとに計算したエッジ強度をEdgeに格納すること。*/

}

void laplacian_filter(GrayImage* gimage, GrayImage* Edge) {
    /* ここにプログラムを挿入する */
    /* 入力カラー画像gimageに対してラプラシアンフィルタを計算し，出力用Edgeに格納する．*/

}

void median_filter(ColorImage* outimage, ColorImage* inimage, int win_size)
{
    /* ここにプログラムを挿入する */
    /* 入力カラー画像inimageに対してメディアンフィルタ
     を計算し，出力用outimageに格納する．
     win_sizeはフィルタサイズが格納されている. */

}

void derivative_filter(GrayImage* gimage, GrayImage* dX, GrayImage* dY, GrayImage* Edge)
{
    /* ここにプログラムを挿入する */
    /* dXに水平方向の微分結果をdYに垂直方向の微分結果を格納する。
        また、それらをもとに計算したエッジ強度をEdgeに格納すること。*/

}

/* ここから下は修正する必要はありません */

void clipping(ColorImage *cimage)
{
  int i,j;

	/* ここに画素値を0～255に制限するプログラムを挿入する */
	for(i=0; i<cimage->height; i++){
		for(j=0; j<cimage->width; j++){
            if(cimage->r[i][j] > 255.0f) cimage->r[i][j] = 255.0f;
            if(cimage->g[i][j] > 255.0f) cimage->g[i][j] = 255.0f;
            if(cimage->b[i][j] > 255.0f) cimage->b[i][j] = 255.0f;
            if(cimage->r[i][j] < 0.0f) cimage->r[i][j] = 0.0f;
            if(cimage->g[i][j] < 0.0f) cimage->g[i][j] = 0.0f;
            if(cimage->b[i][j] < 0.0f) cimage->b[i][j] = 0.0f;
		}
	}
}

void loadPGM(char* filename, GrayImage* gi)
{
    char word[1000];
    int i, j;
    FILE* in;

    in = fopen(filename, "rb");

    if (in == NULL) {
        printf("Cannot open file\n");
        exit(0);
    }

    getword(in, word);

    getword(in, word);
    sscanf(word, "%d", &(gi->width));
    getword(in, word);
    sscanf(word, "%d", &(gi->height));
    getword(in, word);

    gi->x = (float**)malloc(gi->height * sizeof(float*));
    if (gi->x == NULL) {
        printf("Error in malloc");
        exit(0);
    }
    for (i = 0; i < gi->height; i++) {
        gi->x[i] = (float*)malloc(gi->width * sizeof(float));
        if (gi->x[i] == NULL) {
            printf("Error in malloc");
            exit(0);
        }
    }

    /*discard one CR*/
    fgetc(in);

    for (i = 0; i < gi->height; i++) {
        for (j = 0; j < gi->width; j++) {
            int tmp;
            fscanf(in, "%s", word);
            sscanf(word, "%d", &(tmp));
            gi->x[i][j] = (float)tmp;
        }
    }

    fclose(in);
}

void savePGM(char* filename, GrayImage* gi)
{
    int i, j;
    FILE* out;

    out = fopen(filename, "w");

    if (out == NULL) {
        printf("Cannot open file\n");
        exit(0);
    }
    fprintf(out, "P2\n%d %d\n255\n", gi->width, gi->height);

    for (i = 0; i < gi->height; i++) {
        for (j = 0; j < gi->width; j++) {
            if (gi->x[i][j] > 255) gi->x[i][j] = 255;
            else if (gi->x[i][i] < 0) gi->x[i][j] = 0;
            fprintf(out, "%d ", (int)gi->x[i][j]);
        }
    }
    fclose(out);
}

void loadPPM(char* filename, ColorImage* ci)
{
    char word[1000];
    int i, j;
    FILE* in;

    in = fopen(filename, "rb");

    if (in == NULL) {
        printf("Cannot open file\n");
        exit(0);
    }

    getword(in, word);

    getword(in, word);
    sscanf(word, "%d", &(ci->width));
    getword(in, word);
    sscanf(word, "%d", &(ci->height));
    getword(in, word);

    ci->r = (float**)malloc(ci->height * sizeof(float*));
    ci->g = (float**)malloc(ci->height * sizeof(float*));
    ci->b = (float**)malloc(ci->height * sizeof(float*));
    if (ci->r == NULL || ci->g == NULL || ci->b == NULL) {
        printf("Error in malloc");
        exit(0);
    }

    for (i = 0; i < ci->height; i++) {
        ci->r[i] = (float*)malloc(ci->width * sizeof(float));
        ci->g[i] = (float*)malloc(ci->width * sizeof(float));
        ci->b[i] = (float*)malloc(ci->width * sizeof(float));
        if (ci->r[i] == NULL || ci->g == NULL || ci->b == NULL) {
            printf("Error in malloc");
            exit(0);
        }
    }

    /*discard one CR*/
    fgetc(in);

    for (i = 0; i < ci->height; i++) {
        for (j = 0; j < ci->width; j++) {
            int tmpr, tmpg, tmpb;
            fscanf(in, "%s", word); sscanf(word, "%d", &(tmpr));
            fscanf(in, "%s", word); sscanf(word, "%d", &(tmpg));
            fscanf(in, "%s", word); sscanf(word, "%d", &(tmpb));

            ci->r[i][j] = (float)tmpr;
            ci->g[i][j] = (float)tmpg;
            ci->b[i][j] = (float)tmpb;
        }
    }

    fclose(in);
}

void savePPM(char* filename, ColorImage* ci)
{
    /* save file in PGM P3 format */
    int i, j;
    FILE* out;

    /* ファイルをオープン */
    out = fopen(filename, "w");

    /* ファイルが存在しなければエラー */
    if (out == NULL) {
        printf("Cannot open file\n");
        exit(0);
    }

    /* ファイルのヘッダ情報を書き出す */
    fprintf(out, "P3\n%d %d\n255\n", ci->width, ci->height);

    /* [0, 255]に画素値をクリッピング */
	clipping(ci);

    for (i = 0; i < ci->height; i++) {
        for (j = 0; j < ci->width; j++) {
            fprintf(out, "%d ", (int)ci->r[i][j]);
            fprintf(out, "%d ", (int)ci->g[i][j]);
            fprintf(out, "%d ", (int)ci->b[i][j]);
        }
    }

    fclose(out);
}

void getword(FILE* fp, char* word)
{
    fscanf(fp, "%s", word);

    if (word[0] == '#') {
        while (word[0] != 10) {
            word[0] = (int)fgetc(fp);
        }
        fscanf(fp, "%s", word);
    }
}


void createColorImage(ColorImage* ci)
{
    int i, j;

    ci->r = (float**)malloc(ci->height * sizeof(float*));
    ci->g = (float**)malloc(ci->height * sizeof(float*));
    ci->b = (float**)malloc(ci->height * sizeof(float*));
    if (ci->r == NULL || ci->g == NULL || ci->b == NULL) {
        printf("Error in malloc");
        exit(0);
    }

    for (i = 0; i < ci->height; i++) {
        ci->r[i] = (float*)malloc(ci->width * sizeof(float));
        ci->g[i] = (float*)malloc(ci->width * sizeof(float));
        ci->b[i] = (float*)malloc(ci->width * sizeof(float));
        if (ci->r[i] == NULL || ci->g[i] == NULL || ci->b[i] == NULL) {
            printf("Error in malloc");
            exit(0);
        }
    }

    for (i = 0; i < ci->height; i++) {
        for (j = 0; j < ci->width; j++) {
            ci->r[i][j] = 0;
            ci->g[i][j] = 0;
            ci->b[i][j] = 0;
        }
    }
}

void createGrayImage(GrayImage* gi)
{
    int i, j;

    gi->x = (float**)malloc(gi->height * sizeof(float*));
    if (gi->x == NULL) {
        printf("Error in malloc");
        exit(0);
    }

    for (i = 0; i < gi->height; i++) {
        gi->x[i] = (float*)malloc(gi->width * sizeof(float));
        if (gi->x[i] == NULL) {
            printf("Error in malloc");
            exit(0);
        }
    }

    for (i = 0; i < gi->height; i++) {
        for (j = 0; j < gi->width; j++) {
            gi->x[i][j] = 0;
        }
    }
}

void freeColorImage(ColorImage* ci)
{
    int i;

    for (i = 0; i < ci->height; i++)
    {
        free(ci->r[i]);
        free(ci->g[i]);
        free(ci->b[i]);
    }
    free(ci->r);
    free(ci->g);
    free(ci->b);
}

void freeGrayImage(GrayImage* gi)
{
    int i;

    for (i = 0; i < gi->height; i++)
    {
        free(gi->x[i]);
    }
    free(gi->x);
}
