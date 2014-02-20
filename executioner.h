#ifndef EXECUTIONER_H_
#define EXECUTIONER_H_

#include <stdio.h>

#include "semaphore.h"
#include "program.h"

t_program *exec(t_program*, int);
void rollback(t_program*, int, t_program*);
bool anyActive(t_programBulk*);
void evaluateR(t_programBulk*, int, int);
void evaluate(t_programBulk*);

#endif
