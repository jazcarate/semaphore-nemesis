#ifndef PROGRAM_H_
#define PROGRAM_H_

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>


typedef enum {PRINT, SIGNAL, WAIT} t_action;
typedef struct {
	t_action action;
	char argument;
} t_operation;

typedef struct {
	char name;
	int size;
	int pc;				//Program Counter
	t_operation *operations;	//Un programa es un array de operaciones
	enum {ACTIVE, FINISHED, INTERUPTED} state;	//Y un estado
} t_program;

typedef struct {
	int size;
	t_program **programs;
} t_programBulk;


const char 		*getActionName(t_action);
t_program		*program_create(char, int, ...);
void			program_destroy(t_program*);
t_programBulk	*programBulk_create(int, ...);
void 			programBulk_destroy(t_programBulk*);


#endif
