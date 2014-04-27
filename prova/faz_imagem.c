#include <stdio.h>
#include <stdlib.h>	
#include <string.h>


int main (int argc, char* argv[])

{
	FILE* foto,*imagem,*valores;
	int i,j;

	char teste[100];
	
	unsigned char dado,dado2;

	imagem=fopen("imagem_final.y","r");
	foto=fopen("imagem_final_sem_eerro.y","r");
	fprintf(foto, "P5 2580 1120 255 ");

	for(i=0 ; i<654;i++){
		for(j=0 ; j<1469;j++){
			fread(&dado,sizeof(unsigned char),1,imagem);
			fread(&dado2,sizeof(unsigned char),1,foto);
			if(dado != dado2) puts("Aleluia!!!");
		

			

		}
	}

	fclose(imagem);		
	fclose(foto);
}	
