#include "program.h"

const char* getActionName(t_action a){
   switch (a){
      case PRINT: return "PRINT";
      case SIGNAL: return "SIGNAL";
      case WAIT: return "WAIT";
      default: return "N/A";
   }
}


t_program *program_create(char name, int size, ...){
	t_program *new = malloc( sizeof( t_program ) );
				new->name		= name;
				new->size		= size;
				new->pc			= 0;
				new->state		= ACTIVE;
				new->operations	= malloc( size * sizeof( t_operation ) );

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
	t_programBulk *new = malloc( sizeof( t_programBulk ) );
				new->size = size;
				new->programs = malloc( size * sizeof( t_program* ) );

	va_list programs;
	va_start( programs, size );
	int i;
	for(i=0; i<size; i++)
		new->programs[i] = va_arg( programs, t_program* );
	return new;
}

void programBulk_destroy(t_programBulk *self){
	int i;
	for(i=0; i<self->size; i++ )
		program_destroy(self->programs[i]);

	free( self->programs );
	free( self );
}
