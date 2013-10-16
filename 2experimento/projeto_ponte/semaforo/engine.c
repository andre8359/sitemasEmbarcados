#include <stdio.h>
#include <string.h>

void cabecalho_da_tabela()
{
	printf("Crianca | N Travessias |    Lado    | Tempo de Decisao |    Status\n");
	printf("-----------------------------------------------------------------------\n");
}

void printCrianca(int id, int num_travessias, char lado[9],	
					int tempoDecisao, char estado[13])
{
	if(strcmp(lado,"Esquerdo")==0)
	{
		printf("   %2d           %2d         %s          %2d           %s\n", 
			id, num_travessias, lado, tempoDecisao, estado);
	}
	else	
	{
		printf("   %2d           %2d         %s           %2d           %s\n", 
			id, num_travessias, lado, tempoDecisao, estado);	
	}
}
 
