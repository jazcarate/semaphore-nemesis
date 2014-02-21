#ifndef EXECUTIONER_H_
#define EXECUTIONER_H_

#include <stdio.h>

#include "semaphore.h"
#include "program.h"

#ifdef DEBUG
	#include "console.h"
#endif

t_program *exec(t_program*);
void rollback(t_program*, t_program*);

bool anyActive(t_programBulk*);
void evaluateR(t_programBulk*, int);
void evaluate(t_programBulk*);

#endif
