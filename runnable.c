#include "executioner.h"

//#define DEBUG Esta como directiva en compilacion -D

int main(int argc, char **argv) {
	semaphores_init();
	semaphore_set('a', 0);

	t_programBulk* testCase = programBulk_create(2,
								program_create('1', 3,	(t_operation){ WAIT, 'a'},
														(t_operation){ PRINT, 'a'},
														(t_operation){ SIGNAL, 'a'}
											),
								program_create('2', 3,	(t_operation){ WAIT, 'a'},
														(t_operation){ PRINT, '1'},
														(t_operation){ SIGNAL, 'a'}
											)
	);


	 evaluate( testCase );

	 //Clean up crew!
	 programBulk_destroy( testCase );
	 semaphores_destroy();

	return EXIT_SUCCESS;
}
