#ifndef EXECUTIONER_H_
#define EXECUTIONER_H_

#include <stdio.h>

#include "semaphore.h"
#include "program.h"


void exec(t_program*, int);
void rollback(t_program*, int);
bool anyActive(t_programBulk*);
void evaluateR(t_programBulk*, int, int);
void evaluate(t_programBulk*);

#endif
