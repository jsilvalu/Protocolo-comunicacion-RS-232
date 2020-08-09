//============================================================================
// Author      : Juan Antonio Silva Luján
//             : Alberto Mangut Bustamante
// GRUPO       : L6 - Miércoles de 10.00 a 11.30
// Version     : Sesion 2
// Description : Sesion 2 (FRC)
//============================================================================

#ifndef TRAMACONTROL_H_
#define TRAMACONTROL_H_

using namespace std;
#include "PuertoSerie.h"
#include <conio.h>
#include <iostream>
#include "TipoTrama.h"
#include <stdio.h>
#include <conio.h>
#include <string.h>
#include <windows.h>
#include <fstream>



//================== I Funciones de TRAMAS de Control =========================================
void TeclaFuncionF2_enviarTramaControl(HANDLE PuertoCOM, bool &salir, ofstream &flujoLOG, bool &logF5);


#endif /* TRAMACONTROL_H_ */
