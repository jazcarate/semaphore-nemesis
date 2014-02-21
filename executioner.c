#include "executioner.h"

#define BUS_SIZE 9
char bus[BUS_SIZE] = { 0 };
int busPointer = 0;

int programsSize = 0;

void increment_pc(t_program *self, int amount){
#ifdef DEBUG
	printf_coordinates(self->positionX,
						self->pc,
						"  ");	//Borra el marcador de PC viejo
#endif

	self->pc += amount;

	if(self->pc < 0)
		self->pc = 0;

	if(self->pc >= self->size){
		self->state = FINISHED;

		if(self->loop != LOOP_NO){	//Si tiene que loopear
			if(self->loop != LOOP_INF)
				self->loop--;	//Desconta  si no es infinito
			self->pc = 0;		//Volve a empezar (pero con estado de Finished)
		}
	}
		#ifdef DEBUG
			printf_coordinates(self->positionX,
								self->pc,
								"->");	//Marca el PC actual
		#endif
}

t_program *exec(t_program* self){
#ifdef DEBUG
	printf_status("  Do: %c[%d] -> %s %c", self->name, self->pc, getActionName(self->operations[self->pc].action), self->operations[self->pc].argument);
#endif

	if( self->state == INTERUPTED ){	//Con el auto
		#ifdef DEBUG
			printf_coordinates(0, 0, "%c is interupted. Can't exec", self->name);
		#endif
		return NULL;
	}

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
					printf_coordinates(ret->positionX,
										ret->pc,
										"R ");	//Levalta al otro
					printf_status("EXEC) %c READY UP: %c", self->name, ret->name);
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
						printf_coordinates(self->positionX,
											self->pc,
											"%c)", arg);	//Se interumpio
						printf_status("EXEC) WAIT INTERRUPT: %c", self->name);
					#endif
			}
			break;
		default:
			break;
	}
	#ifdef DEBUG
		semaphore_status();	//Actualiza los semaforos
	#endif


	//Siempre tiene que avanzar? 99% seugro que si
	increment_pc(self, +1);
	return ret;
}

void rollback(t_program* self, t_program* lynch){
	//Siempre que rollbackea esta activo, sino no podria haberlo ejecutado... no? :S

	if(self->state == FINISHED)	//Tendria que evaluar tambien que el loop sea o 0 o -1, pero si fuese un programa que esta en finished y no loopeaba, el PC no cambiaba, y ya queda al final por el exec
		self->pc = self->size;

	increment_pc(self, -1);
	self->state = ACTIVE;

#ifdef DEBUG
	printf_status("UnDo: %c[%d]-> %s %c", self->name, self->pc, getActionName(self->operations[self->pc].action), self->operations[self->pc].argument);
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
						printf_coordinates(lynch->positionX,
											lynch->pc,
											"%c)", arg);	//Levalta al otro
						printf_status("ROLLBACK) %c INTERRUPTED: %c", self->name, lynch->name);
					#endif
			}
			break;
		case WAIT:
			//Identico al exec del SIGNAL, pero saca alrevez de la lista
			if( semaphore_inc( arg, 1 ) <= 0 ){
				t_program* luckyYou = (t_program*)list_remove(semaphore_get( arg )->bloked, list_size( semaphore_get( arg )->bloked )-1 );	//Si t0do funciona bien, siempre tendrias que ser vos
				luckyYou->state = ACTIVE;
				#ifdef DEBUG
					printf_coordinates(luckyYou->positionX,
										luckyYou->pc,
										"R ");	//Levalta al otro
					printf_status("ROLLBACK) %c READY UP: %c", self->name, luckyYou->name);
				#endif
			}
			break;
		default:
			break;
	}
#ifdef DEBUG
	semaphore_status();	//Actualiza los semaforos
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
#ifdef DEBUG
	getchar();
#endif

	t_program* aux=exec( test->programs[who]);

	bool anyActive = false;
	int i;
	for(i=0; i<test->size; i++){
		if(test->programs[i]->state == ACTIVE){
			anyActive = true;
			evaluateR( test, i);
		}
	}

	//FIXME: Esto esta "mal", porque el CPU ejecuta como queire, pero intentamos hacer en shecudler que le de "prioridad" a los que no esten terminados
	for(i=0; i<test->size; i++){
		if(!anyActive && test->programs[i]->state == FINISHED && test->programs[i]->loop != LOOP_NO){
			anyActive = true;
			evaluateR( test, i);
		}
	}

	if( !anyActive ){	//Es una funciona aparte, y no un flag porque no podes inferir segun lo que pasa adentro de los evaluate recursivos
#ifdef DEBUG
		printf_status("Bus:\t%s\n", bus);	//Imprimirlo en rojo
#else
		printf("Bus:\t%s\n", bus);	//Lo unico que hace si esta sin debug
#endif
	}

	rollback( test->programs[who], aux); //Ni me impota si esta activo, porque si no estuviese, hubiese retornado lienas antes
}

void evaluate( t_programBulk* test ){	system("clear");	//Nada de portabilida but w/e
#ifdef DEBUG
	console_init(test->size);
	programBulk_print( test );
	semaphore_status();
#endif

	int i;
	for(i=0; i<test->size; i++)
		evaluateR( test, i );

#ifdef DEBUG
	semaphore_status();
#endif
}
