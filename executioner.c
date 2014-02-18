#include "executioner.h"

#define BUS_SIZE 9
char bus[BUS_SIZE] = { 0 };
int busPointer = 0;


void exec(t_program* self){
#ifdef DEBUG
	printf("\tDo: %c[%d]\t->\t%s %c", self->name, self->pc, getActionName(self->operations[self->pc].action), self->operations[self->pc].argument);
#endif

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
				t_program* luckyYou = (t_program*)list_remove(semaphore_get( arg )->bloked, 0);
				luckyYou->state = ACTIVE;	//No tengo cola de listos, porqque evaluo todas las posibilidades, si tengo el estado de listo
				#ifdef DEBUG
					printf("\tREADY UP: %c", luckyYou->name);
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


	//if(self->state == ACTIVE) //TODO: Siempre tiene que avanzar? 99% seugro que si
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
}

void rollback(t_program* self){
	//TODO: Siempre que rollbackea esta activo, sino no podria haberlo ejecutado... no? :S
			self->pc--;
		//self->state = ACTIVE;

#ifdef DEBUG
		printf("\tUnDo: %c[%d]\t->\t%s %c", self->name, self->pc, getActionName(self->operations[self->pc].action), self->operations[self->pc].argument);
#endif


	char arg = self->operations[self->pc].argument;
	switch (self->operations[self->pc].action) {
		case PRINT:
				bus[busPointer--] = '\0';
			break;
		case  SIGNAL:
			//TODO No creo que sea tan facil como CopyPasta
			if( semaphore_inc( arg, -1) < 0 ){ //evaluacion pos incremento
				list_add(semaphore_get( arg )->bloked, self);
				self->state = INTERUPTED;
					#ifdef DEBUG
						printf("\tINTERRUPTED");
					#endif
			}
			break;
		case WAIT:
			//TODO: Vladria una revision
			if( semaphore_inc( arg, 1 ) <= 0 ){ //evaluacion pos incremento
				t_program* luckyYou = (t_program*)list_remove(semaphore_get( arg )->bloked, 0);
				luckyYou->state = ACTIVE;	//No tengo cola de listos, porqque evaluo todas las posibilidades, si tengo el estado de listo
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

void evaluateR( t_programBulk* test, int who ){
	exec( test->programs[who] );

	int i;
	for(i=0; i<test->size; i++){
		if(test->programs[i]->state == ACTIVE){
			evaluateR( test, i);
		}
	}
	if(!anyActive( test )){	//Es una funciona aparte, y no un flag porque no podes inferir segun lo que pasa adentro de los evaluate recursivos
		printf("Bus:\t%s\n", bus);
		semaphore_status();
	}
	rollback( test->programs[who] );
}

void evaluate( t_programBulk* test ){
	printf("Estado inicial:\n");
	semaphore_status();

	printf("\nEvaluacion:\n");
	int i;
	for(i=0; i<test->size; i++)
		evaluateR( test, i );
}
