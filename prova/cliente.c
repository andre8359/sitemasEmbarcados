/*
Aluno: André Mateus Rodrigues Dantas
Matricula : 11/0008359
*/

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <signal.h>

/*----------------------Estruturas---------------*/

struct bloco
{
	char string[25];
	int ocorr;
};

struct args
{
	int i,j;
	float **m;
};

/*----------------------Variáveis------------------*/
int h_img,w_img,fim=0,flag=0,flag1=0,h1=0;
int w1	=0;
float p[]={4/21.0,2/21.0,1/21.0,2/21.0,4/21.0,2/21.0,1/21.0,1/42.0,1/21.0,2/21.0,1/21.0,1/42.0};
struct bloco* blocos;

char nome_imagem[100];

pthread_mutex_t mut = PTHREAD_MUTEX_INITIALIZER;

/*----------------------Funções------------------*/
void valida_linha(int teste,int linha){
	if(!teste)
	{
		
		fprintf(stderr, "Erro!!! Verifique a linha %d. \n",linha);
		exit(1);	
	}
}

float** aloca_matriz(int i, int j){

	int k;
	float **matriz = (float **) malloc (i*sizeof(float*));
	if(matriz == NULL)
		{
			fprintf(stderr, "Erro na alocacao de memoria.\n\n");
			exit(1);			}
	for(k=0 ; k<i;k++){
		matriz[k] = (float *) malloc (j*sizeof(float));
		if(matriz[k] == NULL)
		{
			fprintf(stderr, "Erro na alocacao de memoria.\n\n");
			exit(1);	
		}
	}
		

	return matriz;
}
void colher_dados()
{

	FILE* parametros;

	parametros=fopen("parametros.txt","rb");
	valida_linha((int)parametros,74);

	fscanf(parametros, "%s ",nome_imagem );
	fscanf(parametros, "%d ",&w_img );
	fscanf(parametros, "%d ",&h_img );

	fclose(parametros);

	
}

float binarizar(float ** m, int i, int j){
	float temp = m[i][j];

	if(temp<0.5)	m[i][j]=0;
	else 	m[i][j]=1;
	return temp;
}

int vb(int i, int j){ // função que devera validar a borda para a propagação do erro
	if(i>=0 && i<h_img){
		if (j>=0 && j<w_img)
			return 1;
		else 
			return 0 ;
	}
	else
		return 0;
}

void erro (float **m, int i, int j , float v){
	float e = m[i][j] - v;

	int k,t,n=2;
	
	if (vb(i,j+1)) m[i][j+1]-=p[0]*e;
	if (vb(i,j+2)) m[i][j+2]-=p[1]*e;
		
	for(k=i+1; k<i+3; k++)
		for (t = j-2; t < j+3; ++t){
			if (vb(k,t)) m[k][t]-=p[n]*e;	
			n++;
		}
}

void parse_m2s(float **m, int i, int j,char *resp){

	int c,l;
	char str[25]="";

	for(l = i-5; l<i; l++)
		for (c = j-5; c < j; ++c)
		{	
			
			if(l==(i-3) && c==(j-3)) continue;
			else if(m[l][c]==1.0)
				strcat(str,"1");
			else strcat(str,"0");
			

		}

	strcpy(resp,str);
	return;
}

void histograma(void *arg){

	struct args * params = (struct args *) arg;
	int i,j,k;
	char str[25] = "";


	for (j=0; j < (w_img - (w_img)%5); j=j+5)
	{
		pthread_mutex_lock(&mut);
		parse_m2s(params->m,params->i,j,str);
		
		for(i=0; i<fim; i++){
			if(!strcmp((blocos+i)->string,str)){
				(blocos+i)->ocorr++;
				flag=1;
				break;
			}
			else	continue;
		}
		if(!flag1){	
				strcpy((blocos+fim)->string,str);
				(blocos+i)->ocorr++;
				fim++;
				flag1=0;
		}
		pthread_mutex_unlock(&mut);
	}
}

void ordena(){

	int i,j;
	struct bloco temp;

	for(i=0;i<fim; i++)
		for (j = 0; j < fim - i; ++j)
		{

			if((blocos+j)->ocorr < (blocos+j+1)->ocorr){
				pthread_mutex_lock(&mut);
				temp = *(blocos+j+1);
				*(blocos+j+1) = *(blocos+j);
				*(blocos+j)=temp;
				pthread_mutex_unlock(&mut);
			}
			else continue;
		}
}

void escreve_img(char* nome,float **img){
	
	FILE *imagem = fopen(nome,"w+");
	unsigned char temp;
	int i,j;
	valida_linha((int) imagem, 138);
	fprintf(imagem, "P5 %d %d 1 ",w_img,h_img);
	for(i=0; i<h_img; i++)
		for(j=0; j<w_img; j++){
			temp=(unsigned char )img[i][j];	
			fwrite(&temp,sizeof(unsigned char),1,imagem);	
			
		}
	fclose(imagem);
}
/*--------------Função Principal---------*/

int main(int argc, unsigned char *argv[])
{

	float **img;	
	int i,j;
	unsigned char temp,temp3;
	char temp2[25];
	pthread_t thread_id[3];
	float temp1;
	struct args arg;
	FILE *imagem;

//	signal(SIGINT,interromper);
	
	colher_dados();


	blocos = (struct blocos*) malloc ((int)(h_img*w_img/25)*sizeof(struct bloco));
	
	img=aloca_matriz(h_img,w_img);

	imagem = fopen(nome_imagem, "rb");
	valida_linha((int)imagem,165);
	
	// Lendo e normalizando a imagem 	
	for(i=0; i<h_img; i++)
		for(j=0; j<w_img; j++){
			fread(&temp,sizeof(unsigned char),1,imagem);					
			img[i][j] = temp/255.0;
		}
	fclose(imagem);			
	
	for(i=0; i<128; i++){
		strcpy((blocos+i)->string,"");
		(blocos+i)->ocorr=0;
	}
	// Binarizando imagem
	for(h1=0; h1<h_img; h1++){
		for(w1=0; w1<w_img; w1++){
			temp1=binarizar(img,h1,w1);
			erro(img,h1,w1,temp1);
		}
		if(h1%5==0 && h1){
			arg.i = h1;
			arg.j = w1;	
			arg.m=img;
			pthread_create(&thread_id[1],NULL,(void*)histograma,(void *)&arg);
			pthread_create(&thread_id[1],NULL,(void*)ordena,(void *)&arg);
		}	
	}
	
	
	
	temp=0;
	temp3=0;
	for(i=5; i<h_img; i+=5){
		for (j = 5; j < w_img; j+=5)
		{
			parse_m2s(img,i,j,temp2);
			if(!strcmp(temp2,(blocos+temp3)->string)){
				img[i+2][j+2]=0;
				temp++;
				if(temp>=128) break;


			}
			if(temp>=(blocos+temp3)->ocorr){
				temp3++;
				i=0;
				j=0;

			}

		}

	}

	
	escreve_img("imagem_final.y",img);
	for(i=0; i<h_img; i++)
		free(img[i]);
	free(img);
	
}