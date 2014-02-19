#include "executioner.h"

//#define DEBUG Esta como directiva en compilacion -D

int main(int argc, char **argv) {
	semaphores_init();
	semaphore_set('a', 0);
	semaphore_set('b', 8);
	semaphore_set('c', 3);
	semaphore_set('d', 100);

	/*t_programBulk* testCase = programBulk_create(2,
								program_create('1', 4,	(t_operation){ WAIT, 'a'},
														(t_operation){ PRINT, 'T'},
														(t_operation){ PRINT, 'Y'},
														(t_operation){ SIGNAL, 'a'}
											),
								program_create('2', 4,	(t_operation){ WAIT, 'a'},
														(t_operation){ PRINT, 'q'},
														(t_operation){ PRINT, 'w'},
														(t_operation){ SIGNAL, 'a'}
											)
	);*/

	t_programBulk* testCase = programBulk_create(2,
									program_create('1', 2,	(t_operation){ WAIT, 'a'},
															(t_operation){ WAIT, 'b'}
												),
									program_create('2', 2,	(t_operation){ WAIT, 'c'},
															(t_operation){ WAIT, 'd'}
												)
		);

	 evaluate( testCase );

	 //Clean up crew!
	 programBulk_destroy( testCase );
	 semaphores_destroy();

	return EXIT_SUCCESS;
}
