//============================================================================
// Author      : Juan Antonio Silva Luján
//             : Alberto Mangut Bustamante
// GRUPO       : L6 - Miércoles de 10.00 a 11.30
// Version     : Sesion 2
// Description : Sesion 2 (FRC)
//============================================================================

#ifndef TECLASFUNCION_H_
#define TECLASFUNCION_H_

#include <stdio.h>
#include <iostream>
#include "TramaControl.h"
#include "TramaDatos.h"
#include "MaestroEsclavo.h"

//================== I Funciones de selección =======================================
void eleccionTeclaFuncionF_(int &indice, char car, char cadena[],HANDLE PuertoCOM, bool &salir,TramaControl &trCtrl,TramaDatos &trDatos,int &campo,bool &fichero,int &esCabecera,bool &tamano,string &nomFichero, bool &logF5, int &color);


#endif /*TECLASFUNCION_H_*/
