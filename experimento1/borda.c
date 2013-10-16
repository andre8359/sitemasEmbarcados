#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>

#include <pthread.h>
#include <sys/socket.h>
#include <signal.h>
#include <netinet/in.h>

int calcula_mediana(int* v, int tam){
    
    int flag, i;
    int medi, t0;
    
    
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

    if((tam%2)==0)
        {
            medi=(v[tam/2]+v[tam/2-1])/2;
        }
        else
            medi=v[(i-1)/2];
            
    return medi;
}




int main()
{
 	int i,med,k,c;
    	int vetor[5];
	int t1;
    	int L_ruidoso[8];
	int L_filtrado[8];
	int f=5;
	L_ruidoso[0]=47;
	L_ruidoso[1]=135;
	L_ruidoso[2]=83;
	L_ruidoso[3]=59;
	L_ruidoso[4]=55;
	L_ruidoso[5]=92;
	L_ruidoso[6]=29;
	L_ruidoso[7]=0;

for(c=0;c=7;c++){

	if (((f)%2)!=0)
	{			
		if ((c)<((f)/2))
		{
			k=2*(c)+1;
			
			for (i=0;i<k;i++)
			{
				vetor[i]=L_ruidoso[i];
			}
		med=calcula_mediana(vetor,k);
		L_filtrado[c]=med;

		}	    
		if ((c)>((7)-((f)/2)-1))
		{
			k=(((7)-(c))*2+1);
			t1=(7)-(c)-1;
			for (i=0;i<k;i++)
			{
				vetor[i]=L_ruidoso[(c)-t1+i];
			}
		med=calcula_mediana(vetor,k);
		L_filtrado[c]=med;
		}
		else{
			for (i=0;i<(f);i++)
			{
				vetor[i]=L_ruidoso[(c)-(f)/2+i];
			}
		med=calcula_mediana(vetor,f);
		L_filtrado[c]=med;
		}
        }
	else{
		printf("filtro par");
	}
}    
	printf("[%d %d %d %d %d %d %d %d]",L_filtrado[0],L_filtrado[1],L_filtrado[2],L_filtrado[3],L_filtrado[4],L_filtrado[5],L_filtrado[6],L_filtrado[7]);
;
    	return 0;
}

