#include "program.h"

const char* getActionName(t_action a){
   switch (a){
      case PRINT: return "PRINT";
      case SIGNAL: return "SIGNAL";
      case WAIT: return "WAIT";
      default: return "N/A";
   }
}


t_program *program_create(char name, int loopCount, int size, ...){
#ifdef DEBUG
	static int position = 0;
#endif
	t_program *new = malloc( sizeof( t_program ) );
				new->name		= name;
				new->size		= size;
				new->pc			= 0;
				new->loop		= loopCount;
				new->state		= ACTIVE;
				new->operations	= malloc( size * sizeof( t_operation ) );
				#ifdef DEBUG
					new->positionX	= position++;
				#endif

	va_list operations;
	va_start( operations, size );
	int i;
	for(i=0; i<size; i++)
		new->operations[i] = va_arg( operations, t_operation );
	return new;
}

void program_destroy(t_program *self){
	free( self->operations );
	free( self );
}

t_programBulk* programBulk_create(int size, ...){
#ifdef DEBUG
	console_init(size);
#endif

	t_programBulk *new = malloc( sizeof( t_programBulk ) );
				new->size = size;
				new->programs = malloc( size * sizeof( t_program* ) );

	va_list programs;
	va_start( programs, size );
	int i;
	for(i=0; i<size; i++){
		new->programs[i] = va_arg( programs, t_program* );
		#ifdef DEBUG
			new->programs[i]->positionX = split_console(size, i, maxX, 2);
		#endif
	}
	return new;
}

#ifdef DEBUG
void programBulk_print(t_programBulk* pb){
	int prog, pc;
	t_program* program; //Para cachearlo y que sea mas legible
	for(prog=0; prog<pb->size; prog++){
		program = pb->programs[prog];

		printf_coordinates(program->positionX+2, 0, "Program %c", program->name);
		for(pc=0; pc<program->size; pc++){
			printf_coordinates(program->positionX+2, pc+2, "%s %c", getActionName(program->operations[pc].action), program->operations[pc].argument);
			//Offset de 2 en X para tener el ->, y offset de 2 en Y para tener el nombre y un espacio
		}
	}
}
#endif

void programBulk_destroy(t_programBulk *self){
	int i;
	for(i=0; i<self->size; i++ )
		program_destroy(self->programs[i]);

	free( self->programs );
	free( self );
}
