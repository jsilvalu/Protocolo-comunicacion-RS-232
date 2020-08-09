//============================================================================
// Author      : Juan Antonio Silva Luján
//             : Alberto Mangut Bustamante
// GRUPO       : L6 - Miércoles de 10.00 a 11.30
// Version     : Sesion 5
// Description : Sesion 5 (Protocolo Maestro-Esclavo)
//============================================================================

#ifndef FICHEROMAESTRO_H_
#define FICHEROMAESTRO_H_

#include <iostream>
#include <fstream>
#include "PuertoSerie.h"
#include "TeclasFuncion.h"
#include "PuertoSerie.h"
#include "TipoTrama.h"
#include "TramaDatos.h"
#include "MaestroEsclavo.h"



void F3_enviarFicheroMaestro(HANDLE PuertoCOM,bool maestro,bool &salir);


#endif /* FICHERO_H_ */


