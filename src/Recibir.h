//============================================================================
// Author      : Juan Antonio Silva Luján
//             : Alberto Mangut Bustamante
// GRUPO       : L6 - Miércoles de 10.00 a 11.30
// Version     : Sesion 2
// Description : Sesion 2 (FRC)
//============================================================================

#ifndef RECIBIR_H_
#define RECIBIR_H_
#include <stdio.h>
#include <iostream>


//================== I Funciones de RECIBIR =========================================
void recibir(HANDLE PuertoCOM, int &campo, TramaControl &TrCtrl, TramaDatos &TrDato,bool &fichero,int &esCabecera,bool &tamano,string &nomFichero, bool &logF5,int &color);
bool calcularBCE_recepcion (TramaDatos TrDato, char  campoDatos[]);

#endif /* RECIBIR_H_ */
