#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <math.h>
#include <pthread.h>
#include <signal.h>

/*----------------------Variaveis------------------*/
int  w_video, h_video, w_imagem, h_imagem, num_bits;
int  h=0,i_img=0, j_img=0,num_frame=0;
unsigned char **frame, **imagem;
char nome_video[100],nome_chave[100];
FILE *video, *chave, *foto;


/*----------------------Funcoes------------------*/
unsigned char** aloca_matriz(int i, int j){

	int k;
	unsigned char **matriz = (unsigned char **) malloc (i*sizeof(unsigned char*));
	if(matriz == NULL)
		{
			system("clear");
			fprintf(stderr, "Erro na alocacao de memoria.\n\n");
			exit(1);	
		}
	for(k=0 ; k<i;k++){
		matriz[k] = (unsigned char *) malloc (j*sizeof(unsigned char));
		if(matriz[k] == NULL)
		{
			system("clear");
			fprintf(stderr, "Erro na alocacao de memoria.\n\n");
			exit(1);	
		}
	}
		

	return matriz;
}
void colher_dados()
{
	int i, j;

	FILE* parametros;

	parametros=fopen("parametros.txt","rb");


	fscanf(parametros,"%s ",nome_video);

	fscanf(parametros,"%s ",nome_chave);

	fscanf(parametros, "%d ",&w_imagem );

	fscanf(parametros, "%d ",&h_imagem );
	
	fscanf(parametros,"%d ",&w_video);

	fscanf(parametros,"%d ",&h_video);

	
	fscanf(parametros,"%d\n",&num_bits);
	printf("%d\n",num_bits );
	sleep(5);
	
	
	fclose(parametros);

	
}

void *desembaralha(){

	int i,j,vet[]= {9,8,7,6,5,4,3,2,1,0,0,0,0,0,0,0};
	int valor=0,borda_h,borda_w;
	borda_w = w_video - w_video%3;
	borda_h = h_video - h_video%3;

	for(i=0 ; i<borda_h*num_frame; i=i+3)
		for(j=0 ; j<borda_w;j=j+3){
					
			fscanf(chave,"%d ",&valor);
			if(feof(chave)){
				rewind(chave);

			}
	
			if(i_img==(h_imagem))return; 			

			switch(valor){
				case 0: 
					break;
				case 1:
					imagem[i_img][j_img]=frame[i][j];
					j_img++; break;
				case 2:
					imagem[i_img][j_img]=frame[i+1][j];
					j_img++; break;
				case 3:
					imagem[i_img][j_img]=frame[i+2][j];
					j_img++; break;
				case 4:
					 imagem[i_img][j_img]=frame[i][j+1];
					j_img++;break;
				case 5:
					imagem[i_img][j_img]=frame[i+1][j+1];
					j_img++; break;
				case 6:
					imagem[i_img][j_img]=frame[i+2][j+1];
					j_img++; break;
				case 7:
					imagem[i_img][j_img]=frame[i][j+2];
					j_img++; break;
				case 8:
					 imagem[i_img][j_img]=frame[i+1][j+2];
					j_img++;break;
				case 9:
					imagem[i_img][j_img]=frame[i+2][j+2];
					j_img++; break;
			}
			
			if(j_img>=(w_imagem)){
				i_img++;
				j_img=0;			
			}
			
		}	
}


void *flip(){
	
	int i,j,k;
	
	unsigned char temp, temp1 =0,temp2=0,mask1 = 0x1,mask2=0x80;

	for(i=0; i<h_imagem; i++)	
		for(j=0; j<w_imagem; j++){

			temp=(unsigned char )imagem[i][j];			


			temp1 =0;
			temp2 = 0;
			for(k=0; k<=3; k++){

				temp1|=(imagem[i][j]&(mask1<<k))<<(7-2*k);
				temp2|=(imagem[i][j]&(mask2>>k))>>(7-2*k);			

			}
			temp= temp1|temp2;
			temp = temp>>(8-num_bits);
			temp = temp<<(8-num_bits);
			
			imagem[i][j]=temp;
	
		}
}

void interromper(){
	int i,j;
	char temp[100] ;
	sprintf(temp,"./cabecalho result.y %d %d",w_imagem,h_imagem);
	system(temp);
	
	foto = fopen("result.y", "a+");
	if(!foto)
	{
		system("clear");
		fprintf(stderr, "Erro na abertura do frame da imagem de saida.\n\n");
		exit(1);
	}
	flip();

	for(i=0; i<h_imagem; i++)
		for(j=0; j<w_imagem; j++)
			fputc(imagem[i][j], foto);
	printf("Imagem parcialmente gerada com sucesso!\n");

	fclose(foto);
	exit(0);
}
int main(int argc, unsigned char *argv[])
{

	
	int i,j,tam_arq=0;
	pthread_t thread1;
	pthread_t thread2;


	signal(SIGINT,interromper);
	
	colher_dados();

	video = fopen(nome_video, "rb");
	if(!video)
	{
		
		fprintf(stderr, "Erro na abertura do frame, verifique o nome dos frame de entrada. \n\n");
		exit(1);	
	}
	chave = fopen(nome_chave, "rb");
	if(!chave)
	{
		
		fprintf(stderr, "Erro na abertura da chave. \n\n");
		exit(1);	
	}

	while(fgetc(video)!=	EOF) tam_arq++;
		rewind(video);


	num_frame= tam_	arq/(w_video*h_video);

	imagem = alocaMatriz(h_imagem,w_imagem);
	frame = alocaMatriz(h_video*num_frame,w_video);
	
	
	
	for(i=0; i<h_video*num_frame; i++)
		for(j=0; j<w_video; j++){
			frame[i][j] = fgetc(video);				
		}
		fclose(video);			




	pthread_create (&thread1, NULL, (void *)desembaralha, NULL);
	pthread_join (thread1, NULL);
	pthread_create (&thread2, NULL, (void *)flip, NULL);	
	pthread_join (thread2, NULL);

	fclose(chave);



	char temp[100] ;
	sprintf(temp,"./cabecalho result.y %d %d",w_imagem,h_imagem);
	system(temp);
	
	foto = fopen("result.y", "a+");
	if(!foto)
	{
		system("clear");
		fprintf(stderr, "Erro na abertura do frame da imagem de saida.\n\n");
		exit(1);
	}


	for(i=0; i<h_imagem; i++)
		for(j=0; j<w_imagem; j++)
			fputc(imagem[i][j], foto);

	printf("Imagem gerada com sucesso!\n");

	fclose(foto);



	
	for(i=0; i<h_video; i++)
		free(frame[i]);
	free(frame);

	for(i=0; i<h_imagem; i++)
		free(imagem[i]);
	free(imagem);
	
}
