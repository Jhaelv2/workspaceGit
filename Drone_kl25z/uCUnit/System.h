/*
Este es una Libreria de Codigo fuente para la biblioteca de Drivers
del Dron-Carga beta 0
 
"Nombre Chido!"   R & D

@file System.h
@date 18 mar. 2021
@author Madera
@brief System

@version 2021
*/
#ifndef System_H_
#define System_H_

/* function prototypes */
void System_Init(void);
void System_Shutdown(void);
void System_exit(int val);
void System_Safestate(void);
void System_Recover(void);
void System_WriteString(char * msg);
void System_WriteInt(int n);

#endif /* SYSTEM_H_ */
