#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#include <pthread.h>
#include <signal.h>
#include <error.h>

#define numero_de_threads 100
int imagem[9000][8192];
//int imagem_filtrada[1000][1000];
int mediana[numero_de_threads];
//int max=0;



int calcula_mediana(int v[], int tam){
    
    int flag, i,k;
    int medi, t0;
//	printf("\n");
	//printf("tamanh0:%d\n",tam);
/*    for(k=0;k<tam;k++){
	printf("v[%d]: %d\t",k,v[k]);
	}
*/

    do
    {
        flag=0;
        for(i=1;i<tam;i++)
        {
            if(v[i-1]>v[i])
            {
                t0=v[i-1];
                v[i-1]=v[i];
                v[i]=t0;
                flag=1;
            }
        }
    }
    while (flag==1);
/*
	printf("\n");
	for(k=0;k<tam;k++){
		printf("v_o[%d]: %d\t",k,v[k]);
	}
*/	
    medi=v[(tam/2)];
//	printf("mediana: %d\n",medi);
	
/*	if(medi>max){
		max=medi;}
*/    return medi;

}




struct variaveis
{
	int f;
	int l;
	int c;
	int length;
	int i;

};


void* calcula_pixel (void* parameters)
{
	struct variaveis* p = (struct variaveis*) parameters;

 	int i,med,k;
    	int vetor[p->f];
    	printf("linha: %d\t coluna:%d\n",p->l,p->c);
	if (((p->f)%2)!=0)
	{	
		if ((p->c)<((p->f)/2))
		{
			k=2*(p->c)+1;
			for (i=0;i<k;i++)
			{
				vetor[i]=imagem[p->l][i];
			}
		med=calcula_mediana(vetor,k);
		mediana[p->i]=med;
		}	
    
		 else if ((p->c)>((p->length)-((p->f)/2+1)))
		{	
			k=(((p->length)-(p->c+1))*2+1);
			for (i=(p->length-1);i>(p->length-1-k);i--)
			{
				vetor[i]=imagem[p->l][i];
			}
		med=calcula_mediana(vetor,k);
		mediana[p->i]=med;
		}

		else {	

			for (i=0;i<(p->f);i++)
			{
				vetor[i]=imagem[p->l][(p->c)-(p->f)/2+i];
			}
		med=calcula_mediana(vetor,p->f);

		mediana[p->i]=med;

		}
        }
	else{
		printf("filtro par");
	}	    
    	return 0;
}






int main (int argc, char* argv[])

{
	int largura;
	int altura;
	int filtro;
	

	largura = atoi(argv[2]);
	altura = atoi(argv[3]);
	filtro = atoi(argv[4]);
	
	int linha;
	int coluna;
	unsigned char pixel;
	int i=0,k=0;
	int id;

	
	FILE *FileID;
	FileID=fopen(argv[1],"rb");
	

	FILE *FileID2;
	FileID2=fopen(argv[5],"wb");


	fprintf(FileID2,"P5 %d %d 255\n", largura,altura);



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

	pthread_t thread1_id[numero_de_threads];
	
	struct variaveis thread1_args[numero_de_threads];

	FILE *FileID3;
	FileID3=fopen(argv[6],"wb");
	fprintf(FileID3,"P5 %d %d 255\n", largura,altura);
	
	
	for(linha=0;linha<altura;linha++)
	{
		for(coluna=0;coluna<largura;coluna++)
		{
			thread1_args[i].l=linha;
			thread1_args[i].c=coluna;
			thread1_args[i].f=filtro;
			thread1_args[i].length=largura;
			thread1_args[i].i=i;

			id=pthread_create (&thread1_id[i], NULL, &calcula_pixel, &thread1_args[i]);

			if(id){
				printf("Erro na criação da thread1_id[%d]\n",(linha*largura+coluna));
				printf("Erro : %d\n",id);
				exit(1);
			}
			i++;
			
			if (i==numero_de_threads){
				for (k=0;k<i;k++){
					if(pthread_join(thread1_id[k], NULL)==-1)
					perror("join");
				}

				for (k=0;k<i;k++){
					if(!fwrite(&mediana[k], sizeof(char), 1, FileID3))
					perror("write");

					
				}
				i=0;
				
			}
		}
	}
		fclose(FileID3);



	return 0;
}
