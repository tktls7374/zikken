/*
	2023年度第7回のプログラム
	imageprocess_4_ans.c
*/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <ctype.h>

/* カラー画像の構造体 */
typedef struct{
    int width;
    int height;
    float **r;
    float **g;
    float **b;
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

/* ガウシアンフィルタ　*/
void gaussian_filter(ColorImage *outimage, ColorImage *inimage, float sigma, int win_size);
/* バイラテラルフィルタ */
void bilateral_filter(ColorImage *outimage, ColorImage *inimage, float sigma_s, float sigma_r, int win_size);
/* ラプラシアンフィルタによる鮮鋭化 */
void laplacian_sharpening(ColorImage *outimage, ColorImage *inimage, float k);
/* バイラテラルフィルタによる微小振動の強調 */
void bilateral_sharpening(ColorImage* outimage, ColorImage* inimage, float s, float sigma_s, float sigma_r, int win_size);

int main(int argc, const char * argv[]) {
    /* 変数宣言 */
    ColorImage cinimage, coutimage;
    GrayImage ginimage, Edge;
    char *inputfile, *outputfile;
    int win_size = 5; // フィルタサイズ

    int pros = 0;
    /*	0:ガウシアンフィルタ,
        1:バイラテラルフィルタ
        2:ラプラシアンフィルタによる鮮鋭化
        3:バイラテラルフィルタによる微小振動の強調
	*/
	printf("\n0:ガウシアンフィルタ");
	printf("\n1:バイラテラルフィルタ");
	printf("\n2:ラプラシアンフィルタによる鮮鋭化");
    printf("\n3:バイラテラルフィルタによる微小振動の強調");
    printf("\npros = ");
    scanf("%d", &pros);

    printf("\nFilter window : %d x %d\n", win_size, win_size);

    if (pros == 0) {
    /* ガウシアンフィルタ */
      inputfile = "lena256.ppm";
      outputfile = "out_gauss.ppm";
      float sigma = 20.0f; // ガウス分布の標準偏差
      printf("Sigma for spatial : %0.1f\n ", sigma);
      /* PPMファイルを読み込む*/
      loadPPM(inputfile, &cinimage);
      /* 結果画像を格納するメモリを確保　*/
      coutimage.width = cinimage.width;
      coutimage.height = cinimage.height;
      createColorImage(&coutimage);

      /* ガウシアンフィルタによるフィルタリング*/
      gaussian_filter(&coutimage, &cinimage, sigma, win_size);

      /* 結果画像をPPMファイルとして出力 */
      savePPM(outputfile, &coutimage);
      /* 結果画像用のメモリを解放 */
      freeColorImage(&coutimage);
    } else if (pros == 1) {
    /* バイラテラルフィルタ */
        inputfile="lena256.ppm";
        outputfile="out_BLF.ppm";
        float sigma_s = 50.0f; // 空間方向のガウス分布の標準偏差
        float sigma_r = 30.0f; // 画素値方向のガウス分布の標準偏差
        printf("Sigma for spatial : %0.1f\n", sigma_s);
        printf("Sigma for intensity : %0.1f\n", sigma_r);
        /* PPMファイルを読み込む */
        loadPPM(inputfile, &cinimage);
        /* 結果画像を格納するメモリを確保 */
        coutimage.width = cinimage.width;
        coutimage.height = cinimage.height;
        createColorImage(&coutimage);

        /* バイラテラルフィルタによるフィルタリング */
        bilateral_filter(&coutimage, &cinimage, sigma_s, sigma_r, win_size);

        /* 結果画像をPPMファイルとして出力 */
        savePPM(outputfile, &coutimage);
        /* 結果画像用のメモリを解放 */
        freeColorImage(&coutimage);
    } else if (pros == 2) {
    /* ラプラシアンフィルタによる鮮鋭化 */
        inputfile = "Boats.ppm"; // "lena256.ppm";
        outputfile = "out_Lapsharpen.ppm";
        float k = 0.5f; // 鮮鋭化パラメータ
        printf("sharpning scale k : %0.1f\n", k);
        /* PPMファイルを読み込む */
        loadPPM(inputfile, &cinimage);
        /* 結果画像を格納するメモリを確保 */
        coutimage.width = cinimage.width;
        coutimage.height = cinimage.height;
        createColorImage(&coutimage);

        /* ラプラシアンフィルタによる鮮鋭化処理 */
        laplacian_sharpening(&coutimage, &cinimage, k);

        /* 結果画像をPPMファイルとして出力 */
        savePPM(outputfile, &coutimage);
        /* 結果画像用のメモリを解放 */
        freeColorImage(&coutimage);
    } else if (pros == 3) {
    /* バイラテラルフィルタによる微小振動の強調 */
        inputfile = "Boats.ppm"; // "lena256.ppm";
        outputfile = "out_BLFsharpen.ppm";
        float sigma_s = 50.0f; // 空間方向のガウス分布の標準偏差
        float sigma_r = 30.0f; // 画素値方向のガウス分布の標準偏差
        float s = 2.5f; // 微小振動強調パラメータ
        printf("Sigma for spatial : %0.1f\n", sigma_s);
        printf("Sigma for intensity : %0.1f\n", sigma_r);
        printf("sharpning scale s : %0.1f\n", s);
        // scanf("%f", &s);
        /* PPMファイルを読み込む */
        loadPPM(inputfile, &cinimage);
        /* 結果画像を格納するメモリを確保 */
        coutimage.width = cinimage.width;
        coutimage.height = cinimage.height;
        createColorImage(&coutimage);

        /* バイラテラルフィルタによる微小振動の強調処理 */
        bilateral_sharpening(&coutimage, &cinimage, s, sigma_s, sigma_r, win_size);

        /* 結果画像をPPMファイルとして出力 */
        savePPM(outputfile, &coutimage);
        /* 結果画像用のメモリを解放 */
        freeColorImage(&coutimage);
    } else if (pros == 99) {
        inputfile = "peppers.ppm";
        outputfile = "peppers.ppm";
        loadPPM(inputfile, &cinimage);
        savePPM(outputfile, &cinimage);
        freeColorImage(&cinimage);
    }
    return 0;
}

void gaussian_filter(ColorImage *outimage, ColorImage *inimage, float sigma, int win_size) {
    /* 入力カラー画像inimageに対してガウシアンに基づく平滑化フィルタを計算し，出力用outimageに格納する．*/
    /* 必要であれば、変数宣言を適宜追加すること */
    /* ここにプログラムを挿入する */
    int i, j, k, l, L = (win_size - 1) / 2;
    float sumR = 0.0f, sumG = 0.0f, sumB = 0.0f;
    float w1 = 0.0f, sumW = 0.0f;

    for (i=0; i<outimage->height; i++) {
        for (j=0; j<outimage->width; j++) {
            sumR = 0.0f; sumG = 0.0f; sumB = 0.0f; sumW = 0.0f;
            for (k=-L; k<=L; k++) {
                for (l=-L; l<=L; l++) {
                  int x= fmax(0,fmin(inimage->height - 1, i＋k));
                  int y= fmax(0,fmin(inimage->width - 1, j＋l));

                  float h = sqrt(k*k+l*l);
                  w1 = exp(-(h*h)/(2*sigma *sigma));

                  sumR+=inimage->r[x][y]*w1;
                  sumG+=inimage->g[x][y]*w1;
                  sumB+=inimage->b[x][y]*w1;
                  sumW+= w1;


                }
            }
        /* outimage[i][j]にフィルタの出力を格納する */
        outimage->r[x][y] = sumR/sumW;
        outimage->g[x][y] = sumG/sumW;
        outimage->b[x][y] = sumB/sumW;

        }
    }
}

void bilateral_filter(ColorImage *outimage, ColorImage *inimage, float sigma_s, float sigma_r, int win_size) {
  int i, j, k, l, L = (win_size - 1) / 2;
  float sumR = 0.0f, sumG = 0.0f, sumB = 0.0f;
  float w1 = 0.0f, sumW = 0.0f;

  for (i=0; i<outimage->height; i++) {
      for (j=0; j<outimage->width; j++) {
          sumR = 0.0f; sumG = 0.0f; sumB = 0.0f; sumW = 0.0f;
          for (k=-L; k<=L; k++) {
              for (l=-L; l<=L; l++) {
                int ni=i+k;
                int nj=j+l;

                if (ni>=0 && ni > inimage->height && nj>=0 && nj>inimage->width){
                  float dR =inimage->r[i][j] - inimage->r[ni][nj];
                  float dG =inimage->g[i][j] - inimage->g[ni][nj];
                  float dB =inimage->b[i][j] - inimage->b[ni][nj];
                  float kukan = exp(-(k*k+l*l)/(2*sigma_s*sigma_s));
                  float gasochi = exp(-(dR*dR+dG*dG+dB*dB)/(2*sigma_r*sigma_r));
                  w1=kukan*gasochi;

                  sumR+=inimage->r[i][j]*w1;
                  sumG+=inimage->g[i][j]*w1;
                  sumB+=inimage->b[i][j]*w1;
                  sumW+= w1;

                }

              }
          }
          outimage->r[i][j] = sumR/sumW;
          outimage->g[i][j] = sumG/sumW;
          outimage->b[i][j] = sumB/sumW;
      }
  }




}

void laplacian_sharpening(ColorImage *outimage, ColorImage *inimage, float k) {
    /* 入力カラー画像inimageに対してラプラシアンフィルタによる鮮鋭化結果を計算し，出力用outimageに格納する．*/
    /* ここにプログラムを挿入する */

}

void bilateral_sharpening(ColorImage* outimage, ColorImage* inimage, float s, float sigma_s, float sigma_r, int win_size) {
    /* 入力カラー画像inimageに対してバイラテラルフィルタによる鮮鋭化結果を計算し，出力用outimageに格納する．*/
    /* ここにプログラムを挿入する */

}

/* ここから下は修正する必要はありません */

void clipping(ColorImage *cimage)
{
  int i,j;

	/* ここに画素値を0～255に制限するプログラムを挿入する */
	for (i=0; i<cimage->height; i++) {
		for (j=0; j<cimage->width; j++) {
            if (cimage->r[i][j] > 255.0f) cimage->r[i][j] = 255.0f;
            if (cimage->g[i][j] > 255.0f) cimage->g[i][j] = 255.0f;
            if (cimage->b[i][j] > 255.0f) cimage->b[i][j] = 255.0f;
            if (cimage->r[i][j] < 0.0f) cimage->r[i][j] = 0.0f;
            if (cimage->g[i][j] < 0.0f) cimage->g[i][j] = 0.0f;
            if (cimage->b[i][j] < 0.0f) cimage->b[i][j] = 0.0f;
		}
	}
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
