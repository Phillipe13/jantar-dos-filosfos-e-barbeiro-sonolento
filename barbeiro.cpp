#include <cstdio>
#include <cstdlib>
#include <pthread.h>
#include <unistd.h>
#include <semaphore.h>

int n_clientes = 15;
sem_t sem_cadeira;					//semaforo cadeiras livres
sem_t sem_cadeira_atendimento;		//semaforo cadeira do barbeiro
sem_t sem_cabelo_cortado;			//semaforo para cabelo cortado ou nao
pthread_t barbeiro;					//thread para barbeiro
pthread_t cliente[15];				//thread para clientes
int id[15];

void * vida_cliente(void *var){
	int id = *(int*) var;
	usleep(id*2);
	if(sem_trywait(&sem_cadeira) == 0){		//ocupa cadeira
		printf("Tem cadeiras para o cliente %d\n",id);
		sem_wait(&sem_cadeira_atendimento);		//aguarda barbeiro
		sem_post(&sem_cadeira);					//eh atendido
		sem_wait(&sem_cabelo_cortado);			//espera cortar cabelo
		printf("O cliente %d cortou o cabelo\n\n",id);
		sem_post(&sem_cadeira);					//libera cadeira
		sem_post(&sem_cadeira_atendimento);		//barbeiro liberado
	}
	else{
		printf("Nao havia cadeiras para o cliente %d\n\n", id);
	}
	pthread_exit(NULL);
}
void * vida_barbeiro( void * var ){
	usleep(10);
	while(1){
		sem_wait(&sem_cadeira);			//espera ter cliente e dorme enqunato isso
		printf("Barbeiro atendendo cliente\n");
		usleep(500);					//realizando atendimento
		sem_post(&sem_cabelo_cortado);	//terminou de atender		
	}
	pthread_exit(NULL);
}
int main(){
	sem_init(&sem_cadeira,0,3);						//inicia semaforo
	sem_init(&sem_cadeira_atendimento,0,1);			//inicia semaforo
	sem_init(&sem_cabelo_cortado,0,0);				//inicia semaforo
	int j=0;
	pthread_create(&barbeiro,NULL,&vida_barbeiro,(void *) &id[j]); //cria thread
	for(j=0; j<n_clientes; j++){
		id[j]=j;
		pthread_create(&cliente[j],NULL,&vida_cliente,(void *) &id[j]); //cria threads
		usleep(rand()%100);												//tempo para ver outro cliente
	}
	for(int i=0 ;i < n_clientes; i++){
		pthread_join(cliente[i],NULL);
	}
	printf("Acabou o espediente e barbeiro foi para casa dormir\n");
	return 0;
}
