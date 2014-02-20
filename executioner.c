#include "executioner.h"

#define BUS_SIZE 9
char bus[BUS_SIZE] = { 0 };
int busPointer = 0;


void _tab(int amount){
	int i;
	for(i=0; i<amount; i++)
		printf("  ");
}

t_program *exec(t_program* self, int step){
#ifdef DEBUG
	_tab( step );
	printf("  Do: %c[%d] -> %s %c", self->name, self->pc, getActionName(self->operations[self->pc].action), self->operations[self->pc].argument);
#endif

	t_program* ret = NULL;
	char arg = self->operations[self->pc].argument;
	switch (self->operations[self->pc].action) {
		case PRINT:
				bus[busPointer++] = arg;
			break;
		case  SIGNAL:
			/*
			 * http://en.wikipedia.org/wiki/Semaphore_(programming)
			 * signal(): Increments the value of semaphore variable by 1. After the increment, if the pre-increment value was negative (meaning there are processes waiting for a resource), it transfers a blocked process from the semaphore's waiting queue to the ready queue
			 */
			if( semaphore_inc( arg, 1 ) <= 0 ){ //evaluacion pos incremento
				ret = (t_program*)list_remove(semaphore_get( arg )->bloked, 0);	//La onda es cuadno libero a uno, me lo acuerdo para lyncharlo caundo haces el rollback
				ret->state = ACTIVE;	//No tengo cola de listos, porqque evaluo todas las posibilidades, si tengo el estado de listo
				#ifdef DEBUG
					printf("\tREADY UP: %c", ret->name);
				#endif
			}
			break;
		case WAIT:
			/*
			 * http://en.wikipedia.org/wiki/Semaphore_(programming)
			 * wait(): Decrements the value of semaphore variable by 1. If the value becomes negative, the process executing wait() is blocked, i.e., added to the semaphore's queue.
			 */
			if( semaphore_inc( arg, -1) < 0 ){ //evaluacion pos incremento
				list_add(semaphore_get( arg )->bloked, self);
				self->state = INTERUPTED;
					#ifdef DEBUG
						printf("\tINTERRUPTED");
					#endif
			}
			break;
		default:
			break;
	}


	//Siempre tiene que avanzar? 99% seugro que si
		self->pc++;
	if(self->pc == self->size){
		self->state = FINISHED;
		#ifdef DEBUG
			printf("\tFINISHED!");
		#endif
	}

#ifdef DEBUG
	printf("\n");
#endif
	return ret;
}

void rollback(t_program* self, int step, t_program* lynch){
	//Siempre que rollbackea esta activo, sino no podria haberlo ejecutado... no? :S
	self->pc--;
	self->state = ACTIVE;

#ifdef DEBUG
	_tab( step );
	printf("UnDo: %c[%d]-> %s %c", self->name, self->pc, getActionName(self->operations[self->pc].action), self->operations[self->pc].argument);
#endif


	char arg = self->operations[self->pc].argument;
	switch (self->operations[self->pc].action) {
		case PRINT:
				bus[busPointer--] = '\0';
			break;
		case  SIGNAL:
			//Tengo forma de saber cuando aplique Signal, a quien levante. Ese es el lynch
			if( semaphore_inc( arg, -1) < 0 && lynch != NULL ){	//Nunca seguras punteros a NULL
				list_add(semaphore_get( arg )->bloked, lynch);	//Creo que tendria que agregarlo al final, no al principio
				lynch->state = INTERUPTED;
					#ifdef DEBUG
						printf("\tINTERRUPTED");
					#endif
			}
			break;
		case WAIT:
			//Identico al exec del SIGNAL, pero saca alrevez de la lista
			if( semaphore_inc( arg, 1 ) <= 0 ){
				t_program* luckyYou = (t_program*)list_remove(semaphore_get( arg )->bloked, list_size( semaphore_get( arg )->bloked )-1 );	//Si t0do funciona bien, siempre tendrias que ser vos
				luckyYou->state = ACTIVE;
				#ifdef DEBUG
					printf("\tREADY UP: %c", luckyYou->name);
				#endif
			}
			break;
		default:
			break;
	}
#ifdef DEBUG
	printf("\n");
#endif

}

bool anyActive(t_programBulk* b){
	int i;
	for(i=0; i<b->size; i++){
		if(b->programs[i]->state == ACTIVE){
			return true;	//early out
		}
	}
	return false;
}

void evaluateR( t_programBulk* test, int who, int step ){
	t_program* aux=exec( test->programs[who], step );

	int i;
	for(i=0; i<test->size; i++){
		if(test->programs[i]->state == ACTIVE)
			evaluateR( test, i, step+1);
	}

	if( !anyActive( test ) ){	//Es una funciona aparte, y no un flag porque no podes inferir segun lo que pasa adentro de los evaluate recursivos
		printf("Bus:\t%s\n", bus);
		semaphore_status();
		printf("\n\n");	//Prolijidad
	}

	rollback( test->programs[who], step, aux); //Ni me impota si esta activo, porque si no estuviese, hubiese retornado lienas antes
}

void evaluate( t_programBulk* test ){
	printf("Estado inicial:\n");
	semaphore_status();

	printf("\nEvaluacion:\n");
	int i;
	for(i=0; i<test->size; i++)
		evaluateR( test, i, 0 );

	printf("\nEstado final:\n");
	semaphore_status();
}
