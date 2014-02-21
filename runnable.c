#include "executioner.h"
#ifdef DEBUG
	#include "console.h"
#endif

//#define DEBUG //Esta como directiva en compilacion -D

int main(int argc, char **argv) {

	semaphores_init();
	semaphore_set('a', 1);


	/*t_programBulk* testCase = programBulk_create(3,
								program_create('A', 2, 3,
										(t_operation){ WAIT,	'a'},
										(t_operation){ PRINT,	'A'},
										(t_operation){ SIGNAL,	'd'}
								),
								program_create('B', 0, 5,
										(t_operation){ WAIT,	'b'},
										(t_operation){ WAIT,	'd'},
										(t_operation){ PRINT,	'B'},
										(t_operation){ SIGNAL,	'c'},
										(t_operation){ SIGNAL,	'a'}
								),
								program_create('C', 0, 5,
										(t_operation){ WAIT,	'c'},
										(t_operation){ WAIT,	'd'},
										(t_operation){ PRINT,	'C'},
										(t_operation){ SIGNAL,	'b'},
										(t_operation){ SIGNAL,	'a'}
								)
	);*/

	t_programBulk* testCase = programBulk_create(2,
									program_create('A', 0, 3,
											(t_operation){ WAIT,	'a'},
											(t_operation){ PRINT,	'A'},
											(t_operation){ SIGNAL,	'a'}
									),
									program_create('B', 0, 4,
											(t_operation){ WAIT,	'a'},
											(t_operation){ PRINT,	'w'},
											(t_operation){ PRINT,	't'},
											(t_operation){ SIGNAL,	'a'}
									)
		);

#ifdef DEBUG
	programBulk_print( testCase );
#endif

	 evaluate( testCase );

	 //Clean up crew!
	 programBulk_destroy( testCase );
	 semaphores_destroy();

	return EXIT_SUCCESS;
}
