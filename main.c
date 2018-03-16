#include <stdio.h>

typedef struct _pixel {
	unsigned short int r;
	unsigned short int g;
	unsigned short int b;
} Pixel;

typedef struct _image {
	unsigned short int pixel[512][512][3];
	// [width][height][rgb]
	// 0 -> r
	// 1 -> g
	// 2 -> b
	unsigned int w;
	//Width
	unsigned int h;
	//Height
} Image;

/*
int max(int a, int b) {
if (a > b)
return a;
return b;
}

int pixel_igual(Pixel p1, Pixel p2) {
if (p1.r == p2.r &&
p1.g == p2.g &&
p1.b == p2.b)
return 1;
return 0;
}
*/

Image escala_de_cinza(Image img) {

	for (unsigned int i = 0; i < img.h; ++i) {
		for (unsigned int j = 0; j < img.w; ++j) {
			int media = img.pixel[i][j][0] + img.pixel[i][j][1] + img.pixel[i][j][2];
	    		media /= 3;
	    		img.pixel[i][j][0] = media;
	    		img.pixel[i][j][1] = media;
	    		img.pixel[i][j][2] = media;
		}
	}

	return img;
}

void blur(unsigned int height, unsigned short int pixel[512][512][3], int T, unsigned int width) {
	for (unsigned int i = 0; i < height; ++i) {
		for (unsigned int j = 0; j < width; ++j) {
			Pixel media = {0, 0, 0};

		    	int min_h = (height - 1 > i + T/2) ? i + T/2 : height - 1;
		    	int min_w = (width  - 1 > j + T/2) ? j + T/2 : width  - 1;

		    	for(int x = (0 > i - T/2 ? 0 : i - T/2); x <= min_h; ++x) {
				for(int y = (0 > j - T/2 ? 0 : j - T/2); y <= min_w; ++y) {
				    	media.r += pixel[x][y][0];
				    	media.g += pixel[x][y][1];
				    	media.b += pixel[x][y][2];
				}
		 	   }

		    	media.r /= T * T;
		    	media.g /= T * T;
		    	media.b /= T * T;

		    	pixel[i][j][0] = media.r;
		    	pixel[i][j][1] = media.g;
		    	pixel[i][j][2] = media.b;
		}
	}
}

Image rotacionar90direita(Image img) {
	Image rotacionada;

	rotacionada.w = img.h;
	rotacionada.h = img.w;

	for (unsigned int i = 0, y = 0; i < rotacionada.h; ++i, ++y) {
		for (int j = rotacionada.w - 1, x = 0; j >= 0; --j, ++x) {
		    	rotacionada.pixel[i][j][0] = img.pixel[x][y][0];
		    	rotacionada.pixel[i][j][1] = img.pixel[x][y][1];
		    	rotacionada.pixel[i][j][2] = img.pixel[x][y][2];
		}
	}

	return rotacionada;
}

void inverter_cores(unsigned short int pixel[512][512][3], unsigned int width, unsigned int height) {
	for (unsigned int i = 0; i < height; ++i) {
		for (unsigned int j = 0; j < width; ++j) {
		    	pixel[i][j][0] = 255 - pixel[i][j][0];
		    	pixel[i][j][1] = 255 - pixel[i][j][1];
		    	pixel[i][j][2] = 255 - pixel[i][j][2];
		}
	}
}

Image cortar_imagem(Image img, int x, int y, int width, int height) {
	Image cortada;

	cortada.w = width;
	cortada.h = height;

	for(int i = 0; i < height; ++i) {
		for(int j = 0; j < width; ++j) {
		    	cortada.pixel[i][j][0] = img.pixel[i + y][j + x][0];
	    		cortada.pixel[i][j][1] = img.pixel[i + y][j + x][1];
	    		cortada.pixel[i][j][2] = img.pixel[i + y][j + x][2];
		}
	}

	return cortada;
}

Image sepia(Image img, unsigned short int pixel[512][512][3] ){
	for (unsigned int x = 0; x < img.h; ++x) {
		for (unsigned int j = 0; j < img.w; ++j) {
		        unsigned short int pixel[3];
		        pixel[0] = img.pixel[x][j][0];
		        pixel[1] = img.pixel[x][j][1];
		        pixel[2] = img.pixel[x][j][2];

		        int p =  pixel[0] * .393 + pixel[1] * .769 + pixel[2] * .189;
		        int menor_r = (255 >  p) ? p : 255;
		        img.pixel[x][j][0] = menor_r;

		        p =  pixel[0] * .349 + pixel[1] * .686 + pixel[2] * .168;
		        menor_r = (255 >  p) ? p : 255;
		        img.pixel[x][j][1] = menor_r;

		        p =  pixel[0] * .272 + pixel[1] * .534 + pixel[2] * .131;
		        menor_r = (255 >  p) ? p : 255;
		        img.pixel[x][j][2] = menor_r;
		}
	}
	return img;
}

Image espelhamento(Image img, int horizontal){
	int width = img.w, height = img.h;
	if (horizontal == 1) width /= 2;
	else height /= 2;

	for (int i = 0; i < height; ++i) {
		for (int j = 0; j < width; ++j) {
			int x = i, y = j;

		        if (horizontal == 1) y = img.w - 1 - j;
		        else x = img.h - 1 - i;

		        Pixel aux1;
		        aux1.r = img.pixel[i][j][0];
		        aux1.g = img.pixel[i][j][1];
		        aux1.b = img.pixel[i][j][2];

		        img.pixel[i][j][0] = img.pixel[x][y][0];
		        img.pixel[i][j][1] = img.pixel[x][y][1];
		        img.pixel[i][j][2] = img.pixel[x][y][2];

		        img.pixel[x][y][0] = aux1.r;
		        img.pixel[x][y][1] = aux1.g;
		        img.pixel[x][y][2] = aux1.b;
		    }
		}
	return img;
}


int main() {
	Image img;

	// read type of image
	char p3[4];
	scanf("%s", p3);

	// read width height and color of image
	int max_color;
	scanf("%u %u %d", &img.w, &img.h, &max_color);

	// read all pixels of image
	for (unsigned int i = 0; i < img.h; ++i) {
		for (unsigned int j = 0; j < img.w; ++j) {
	    		scanf("%hu %hu %hu", &img.pixel[i][j][0], &img.pixel[i][j][1], &img.pixel[i][j][2]);
		}
	}

	int n_opcoes;
	scanf("%d", &n_opcoes);

	for(int i = 0; i < n_opcoes; ++i) {
		int opcao;
		scanf("%d", &opcao);

		switch(opcao) {
			case 1: { // Escala de Cinza
				img = escala_de_cinza(img);
				break;
		    	}
		    	case 2: { // Filtro Sepia
				img = sepia(img,img.pixel);
				break;
		    	}
		    	case 3: { // Blur
				int tamanho = 0;
				scanf("%d", &tamanho);
				blur(img.h, img.pixel, tamanho, img.w);
				break;
		    	}
		    	case 4: { // Rotacao
				int quantas_vezes = 0;
				scanf("%d", &quantas_vezes);
				quantas_vezes %= 4;
				for (int j = 0; j < quantas_vezes; ++j) {
			    		img = rotacionar90direita(img);
				}
				break;
		    	}
		    	case 5: { // Espelhamento
				int horizontal = 0;
				scanf("%d", &horizontal);
				img = espelhamento(img,horizontal);
				//TODO FUNCTION
				break;
		    	}
		    	case 6: { // Inversao de Cores
				inverter_cores(img.pixel, img.w, img.h);
				break;
		    	}
		    	case 7: { // Cortar Imagem
				int x, y;
				scanf("%d %d", &x, &y);
				int w, h;
				scanf("%d %d", &w, &h);

				img = cortar_imagem(img, x, y, w, h);
				break;
		    	}
		}
	}

	// print type of image
	printf("P3\n");
	// print width height and color of image
	printf("%u %u\n255\n", img.w, img.h);

	// print pixels of image
	for (unsigned int i = 0; i < img.h; ++i) {
		for (unsigned int j = 0; j < img.w; ++j) {
		    printf("%hu %hu %hu ", img.pixel[i][j][0], img.pixel[i][j][1], img.pixel[i][j][2]);
		}
		printf("\n");
	}

	return 0;
}
