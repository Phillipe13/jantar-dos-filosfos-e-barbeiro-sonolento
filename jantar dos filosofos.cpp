#include <stdio.h>
#include<cstdlib>
#include<pthread.h>
#include<unistd.h>
#include<windows.h>
pthread_mutex_t mutex_garfo[5];
pthread_t filosofo[5];
int id[5];
void pegar_garfo(int * filo){	// filosofo tenta pegar garfo
		pthread_mutex_lock(&(mutex_garfo[*filo]));	//bloqueia um garfo
		if(*filo < 4){
			pthread_mutex_lock(&(mutex_garfo[*filo+1])); //bloqueia outro garfo

		}
		else
			pthread_mutex_lock(&(mutex_garfo[0]));	//se for o quinto filosofo ele bloequeia o garfo da posição 0, e não uma posição a mais que ele
}
void largar_garfos(int * filo){
	pthread_mutex_unlock(&(mutex_garfo[*filo]));		//filoso larga um garfo
	if (*filo < 4){
		pthread_mutex_unlock(&(mutex_garfo[*filo+1])); //filoso larga o garfo
	}
	else
		pthread_mutex_unlock(&(mutex_garfo[0]));	//se for o quinto filosofo, libera o garfo da posição zero
	printf("Filosofo %d terminou de comer.\n",*filo);
}
void * vida_filosofo(void * var){		//representa toda atividade realizada na do filosofo
	int * filo = (int *) (var);			//indentifica filosofo
	while(1){							//vida eterna
		//pensar
		int pensar = (rand()%10+2);		//tempo para pensar
		printf("Filosofo %d: essa mesa nao eh um carro azul, porque ela falha no teste de ser azul...\nPensar por %d segundos\n\n",*filo,pensar);
		Sleep(pensar);
		// filosofo esta com fome, pois parou de pensar
		pegar_garfo(filo);				//tenta pegar os garfos
		int comer = (rand()%5+1);		//tempo para comer
		printf("Filosofo %d esta comendo por %d segundos.\n", *filo,comer);
		Sleep(comer);					//está comendo
		largar_garfos(filo);			//larga os garfos, pois já terminou de comer
	}
	pthread_exit(NULL);
}
int main(){
	for (int i=0; i<=4; i++){
		pthread_mutex_init(&(mutex_garfo[i]),NULL);			//cria areas criticas
	}
	for (int j=0; j<= 4;j++){
		id[j]=j;
		pthread_create(&filosofo[j],NULL,&vida_filosofo,(void *) &id[j]); 		//cria as threads
	}
	while(1){
		//não encerra o codigo
	}
	return 0;
}
