#include "executioner.h"

//#define DEBUG Esta como directiva en compilacion -D

int main(int argc, char **argv) {
	semaphores_init();
	semaphore_set('a', 1);
	semaphore_set('b', 0);
	semaphore_set('c', 1);	//Mi mutex

	t_programBulk* testCase = programBulk_create(2,
								program_create('1', 5,	(t_operation){ WAIT,	'a'},
														(t_operation){ WAIT,	'c'},
														(t_operation){ PRINT,	'Y'},
														(t_operation){ SIGNAL,	'c'},
														(t_operation){ SIGNAL,	'b'}
											),
								program_create('2', 5,	(t_operation){ WAIT,	'b'},
														(t_operation){ WAIT,	'c'},
														(t_operation){ PRINT,	't'},
														(t_operation){ SIGNAL,	'c'},
														(t_operation){ SIGNAL,	'a'}
											)
	);

	 evaluate( testCase );

	 //Clean up crew!
	 programBulk_destroy( testCase );
	 semaphores_destroy();

	return EXIT_SUCCESS;
}
