//============================================================================
// Name        : Fichero.h
// Author      : Juan Antonio Silva Luján
//             : Alberto Mangut Bustamante
// GRUPO       : L6 - Miércoles de 10.00 a 11.30
// Version     : Sesion 4
// Description : Sesion 4 (FRC)
//============================================================================
#ifndef FICHERO_H_
#define FICHERO_H_

#include <iostream>
#include <fstream>
#include "PuertoSerie.h"
#include "TeclasFuncion.h"
#include "PuertoSerie.h"
#include "TipoTrama.h"


void F3_enviarFichero(HANDLE PuertoCOM, int indice, ofstream &flujoLOG, bool &logF5, int &color, bool &salir);


#endif /* FICHERO_H_ */


