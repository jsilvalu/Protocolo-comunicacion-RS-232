//============================================================================
// Author      : Juan Antonio Silva Luján
//             : Alberto Mangut Bustamante
// GRUPO       : L6 - Miércoles de 10.00 a 11.30
// Version     : Sesion 2
// Description : Sesion 2 (FRC)
//============================================================================

#ifndef TRAMADATOS_H_
#define TRAMADATOS_H_
#include "PuertoSerie.h"
#include "TipoTrama.h"
#include <stdio.h>
#include <iostream>
#include <Windows.h>
#include <fstream>



//================== I Funciones de TRAMAS de DATOS =========================================
void TeclaFuncionF1_enviarTramaDatos(HANDLE PuertoCOM, char cadena[], int indice,TramaControl &trCtrl,TramaDatos &trDatos,int &campo,bool &fichero,int &esCabecera,bool &tamano,string &nomFichero, bool &logF5, ofstream &flujoLOG, int &color);
void F2_TramaDatos_TramitarEnviosDefecto(HANDLE PuertoCOM, TramaDatos trmDatos);
void TeclaFuncionF2_envioPorDefecto(HANDLE PuertoCOM, TramaDatos trmDatos, ofstream &flujoLOG, bool &logF5);

#endif /* TRAMADATOS_H_ */
