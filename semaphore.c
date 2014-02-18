#include "semaphore.h"


t_semaphore semaphores['z'-'a'];

void semaphores_init( void ){
	int i;
	for(i=0; i<'z'-'a'; i++){
		semaphores[i].value=0;
		semaphores[i].bloked = list_create();
	}
}
t_semaphore* semaphore_get( char who ){
	return &semaphores[who-'a'];
}
const int semaphore_set( char who, int value ){
	semaphore_get( who )->value=value;
	return value;
}
const int semaphore_inc( char who, int relativeValue ){	//Retorna el NUEVO valor
	return semaphore_set( who, semaphore_get( who )->value+relativeValue);
}
void semaphores_destroy( void ){
	char i;
	for(i='a'; i<'z'; i++)
		list_destroy( semaphore_get( i )->bloked);
}
void semaphore_status( void ){

	void _program_get_name(t_program* x){
		printf("%c -> ", x->name);
	}

	char i;
	printf("Sem:\t");
	for(i='a'; i<'z'; i++)	//Cicla para imprimir valores
		printf("[%c] %+0d\t", i, semaphore_get( i )->value);
	printf("\n");

	for(i='a'; i<'z'; i++){	//Cicla para imprimir estados
		t_semaphore* cache = semaphore_get( i );
		if( list_size( cache->bloked) > 0 ){
			printf("\nBlk [%c]: ", i);
			list_iterate( cache->bloked, (void*)_program_get_name);
			printf("\n\n");
		}
	}
}
