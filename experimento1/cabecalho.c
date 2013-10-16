#include<stdio.h>
#include<stdlib.h>	

int main (int argc, char* argv[])

{
	int largura;//=950;
	int altura;//=350;
	int linha, coluna;
	
	
	largura = 512;//atoi(argv[1]);
	altura = 512;//atoi(argv[2]);

	int imagem[altura][largura];
char pixel;



	FILE *FileID;
	FileID=fopen("img3_512_512.y","rb");

	FILE *FileID2;
	FileID2=fopen("foto.pgm","a");

	int max=255;
	fprintf(FileID2,"P5 %d %d 255\n", largura,altura);
	
	char pix;
/*
	while(!feof(FileID)){	
	fscanf(FileID, "%c", &pix);	
	fwrite(&pix, sizeof(char), 1, FileID2);
	}

	fclose(FileID2);
	fclose(FileID);*/


	for(linha=0;linha<altura;linha++)
	{
		for(coluna=0;coluna<largura;coluna++)
		{
			fscanf(FileID,"%c",&pixel);
			fwrite(&pixel, sizeof(char), 1, FileID2);
			imagem[linha][coluna]=pixel;
		}
	}

	fclose(FileID);
	fclose(FileID2);
}


/*	
	for(linha=0;linha<altura;linha++)
	{
		for(coluna=0;coluna<largura;coluna++)
		{
			fscanf(FileID,"%c",&pixel);
			imagem[linha][coluna]=pixel;
		}
	}
*/

