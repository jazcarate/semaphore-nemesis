#ifndef PROGRAM_H_
#define PROGRAM_H_

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>

#ifdef DEBUG
	#include "console.h"
#endif


typedef enum {PRINT, SIGNAL, WAIT} t_action;
typedef struct {
	t_action action;
	char argument;
} t_operation;

typedef struct {
	char name;
	int size;
	int pc;				//Program Counter
	int loop;			//-1: loopea infinito, 0 no loopea, N (>0) loopea N veces
	t_operation *operations;	//Un programa es un array de operaciones
	enum {ACTIVE, FINISHED, INTERUPTED} state;	//Y un estado

#ifdef DEBUG
	int positionX;	//Solo para el GUI dice donde ponerlo
#endif
} t_program;

#define LOOP_NO 0
#define LOOP_INF -1
#define LOOP_N(A) A

typedef struct {
	int size;
	t_program **programs;
} t_programBulk;


const char 		*getActionName(t_action);
t_program		*program_create(char, int, int, ...);
void			program_destroy(t_program*);

t_programBulk	*programBulk_create(int, ...);
void			programBulk_print(t_programBulk*);
void 			programBulk_destroy(t_programBulk*);

#ifdef DEBUG
void programBulk_print(t_programBulk*);
#endif

#endif
