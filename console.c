#include "console.h"

void error(const char* e){
	//system("clear");	//Nada de portabilida but w/e	TODO: Descomentarlo
	printf("[ERROR] %s\n", e);
	exit(1);
}

void console_init(int _columns){
#ifdef ECLIPSE_MOCK
	maxX = 200;
	maxY = 100;
#else
	system("clear");
	struct winsize console;
	ioctl(STDOUT_FILENO, TIOCGWINSZ, &console);

	maxX = console.ws_col;
	maxY = console.ws_row;
#endif

	columns = _columns;

	semMaxY = maxY*CONSOLE_SEMAPHORE_PERCENT/100;

	statusCurrY	= 0;
	statusMaxY = maxY*CONSOLE_STATUS_PERCENT/100;

	programMaxY = maxY - semMaxY - statusMaxY;
}

int vprintf_coordinates(int x, int y, const char* format, _G_va_list args){
	//Los \n que rompen t0do :D
#ifdef ECLIPSE_MOCK
	printf("(%d, %d): ", x, y);	//Si no estas en consola, mostra eso
#else
	printf("\033[%d;%dH", y, x);	//Moverte (SOLO EN CONSOLA)
#endif
	return vprintf(format, args);			//Pasar la bola a printf
}

int printf_coordinates(int x, int y, const char* format, ...){
	int ret = -1;
	if(	x<=maxX && x>=0 &&
		y<=maxY && y>=0 ){
		va_list args;
		va_start(args, format);			//Los ...
		ret = vprintf_coordinates(x, y, format, args);			//Pasar la bola a printf
		va_end(args);					//Limpiar
		fflush(stdout);					//Flush para mostrar
	} else
		error("Imprimiendo fuera de pantalla");
	return ret;
}

int printf_status(const char* format, ...){
	va_list args;
	int ret = -1;
	va_start(args, format);			//Los ...


	printf_coordinates(0, programMaxY + semMaxY + statusCurrY, "  "); //Limpia el anterior
		statusCurrY = (statusCurrY+1) % statusMaxY;	// \n rotativo
	printf_coordinates(0, programMaxY + semMaxY + statusCurrY, "->"); //Flechita en cual estas

	ret = vprintf_coordinates(2, programMaxY + semMaxY + statusCurrY, format, args);
	va_end(args);					//Limpiar

	return ret;
}

int split_console(int totalPieces, int currentPiece, int totalSize, int margin){
	return margin + currentPiece * (totalSize - margin*2) / totalPieces;
}
