#ifndef CONSOLE_H_
#define CONSOLE_H_

#include <sys/ioctl.h>
#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <stdarg.h>
#include <stdlib.h>

#define CONSOLE_SEMAPHORE_PERCENT 20
#define CONSOLE_STATUS_PERCENT 20

//Para no calcular la consola
//#define ECLIPSE_MOCK

int statusCurrY, maxX, maxY, programMaxY, columns, statusMaxY, semMaxY;	//Globales, porque no hay objetos :( Sad Joaco
										//status es el alto del pedazito de status
										//currX e Y son donde esta en la barra de estado

void console_init(int);

int printf_coordinates(int x, int y, const char* format, ...);
int printf_status(const char* format, ...);
int split_console(int, int, int, int);

#endif
