//============================================================================
// Author      : Juan Antonio Silva Luján
//             : Alberto Mangut Bustamante
// GRUPO       : L6 - Miércoles de 10.00 a 11.30
// Version     : Sesion 5
// Description : Sesion 5 (Protocolo Maestro-Esclavo)
//============================================================================

#ifndef MAESTROESCLAVO_H_
#define MAESTROESCLAVO_H_

using namespace std;
#include <conio.h>
#include "PuertoSerie.h"
#include "TipoTrama.h"
#include <stdio.h>
#include <iostream>


/*Metodos organizativos en la clase (eleccion usuario)*/
void F6_eleccionRolMaestroEsclavo(HANDLE PuertoCOM,bool &salir);

void F6_construirTramaControlMaestroEsclavo(HANDLE PuertoCOM);

void F6_enviarTramaControl_MaestroEsclavo(HANDLE PuertoCOM,TramaControl TrCtrl);

void F6_ejecucionMaestro_Seleccion(HANDLE PuertoCOM,bool &salir);

void F6_ejecucionMaestro_Sondeo(HANDLE PuertoCOM);

void F6_ejecucionEsclavo(HANDLE PuertoCOM);

void enviarTramaControl_MaestroEsclavo(HANDLE PuertoCOM,TramaControl TrCtrl);

void esclavoSeleccion(HANDLE PuertoCOM);

void esclavoSondeo(HANDLE PuertoCOM);

void mostrarTramaControl(TramaControl TrCtrl, bool esReceptor);

void mostrarTramaDatoEmisor(TramaDatos TrCtrl);

bool recibirConfirmacionMaestroSeleccion(HANDLE PuertoCOM,int indice);

bool recibirConfirmacionMaestroSondeo(HANDLE PuertoCOM,int indice);

void recibirMaestroEsclavo(HANDLE PuertoCOM,bool maestro);

bool calcularBCE_recepcionMaestro (TramaDatos TrDato, char  campoDatos[], unsigned char &resulBCE );

bool liberarComunicacionEsclavo(HANDLE PuertoCOM);

bool comprobarSalir(TramaControl TrCtrl,char numtrama);

bool comprobarTrama(TramaControl TrCtrl,char numtrama);

void establecerComunicacionMaestroSeleccion(HANDLE PuertoCOM);

void establecerComunicacionMaestroSondeo(HANDLE PuertoCOM);

void liberarComunicacionMaestroSeleccion(HANDLE PuertoCOM);

void liberarComunicacionMaestroSondeo(HANDLE PuertoCOM, char indice);

TramaControl recibirTramaControl(HANDLE PuertoCOM);

void enviarConfirmacionEsclavoSondeo(HANDLE PuertoCOM, char indice);

void denegarLiberacionMaestroSondeo(HANDLE PuertoCOM, char indice);

void solicitarLiberacion(HANDLE PuertoCOM, char indice);

int establecimientoComunicacionEsclavo(HANDLE PuertoCOM, TramaControl TrCtrl) ;

void enviarConfirmacionEsclavoSeleccion(HANDLE PuertoCOM, char indice);

#endif /* MAESTROESCLAVO_H_ */
