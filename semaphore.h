#ifndef SEMAPHORE_H_
#define SEMAPHORE_H_



#include "commons/collections/list.h"
#include "program.h"
#ifdef DEBUG
	#include "console.h"
#else
	#include <stdio.h>
#endif



typedef struct {
	int value;
	t_list* bloked; //Lista de t_program
} t_semaphore;


void semaphores_init(void);
t_semaphore*semaphore_get(char);
const int	semaphore_set(char, int);
const int	semaphore_inc(char, int);
void 		semaphores_destroy(void);
#ifdef DEBUG
	void		semaphore_status(void);
#endif

#endif
