//============================================================================
// Author      : Juan Antonio Silva Luján
//             : Alberto Mangut Bustamante
// GRUPO       : L6 - Miércoles de 10.00 a 11.30
// Version     : Sesion 2
// Description : Sesion 2 (FRC)
//============================================================================

#include "TeclasFuncion.h"
#include <conio.h>
#include "Fichero.h"

/**
 * La clase TeclasFuncion gestiona que tipo de TECLA de FUNCION se ha pulsado
 * para actuar en consecuencia invocando a los métros de las clases pertinentes.
 */

/**
 * FUNCION: eleccionTeclaFuncionF_
 * PARAMETROS: int &indice, char car, char cadena[], HANDLE PuertoCOM, bool &salir,TramaControl &trCtrl,TramaDatos &trDatos,int &campo,bool &fichero,int &esCabecera,bool &tamano,string &nomFichero, bool &logF5,int &color
 * DESCRIPCION: Selecciona la tecla de función pulsada e invoca a su método correspondiente, opciones:
 *
 *              case 59 ---> F1 (enviar un mensaje)
 *              case 60 ---> F2 (enviar una trama de control)
 *              case 61 ---> f3 (enviar fichero)
 *              case 63 ---> f5 (Registro de actividad en log.txt)
 *              case 64 ---> f6 (Protocolo Maestro-Esclavo)
 */
void eleccionTeclaFuncionF_(int &indice, char car, char cadena[],
		HANDLE PuertoCOM, bool &salir,TramaControl &trCtrl,TramaDatos &trDatos,int &campo,bool &fichero,int &esCabecera,bool &tamano,string &nomFichero, bool &logF5,int &color) {

	ofstream flujoLOG("log.txt", ios::app);

	switch (car) {

	case 59: //-------------------------------------------------------Pulsado F1   -   ENVIO DE TRAMA DE DATOS

		cadena[indice]='\n';
		indice++;
		cadena[indice] = '\0';//Almacenamos \0 en la posicion donde se encuentra el puntero.
		indice++;
		if(logF5)	flujoLOG<<cadena;
		TeclaFuncionF1_enviarTramaDatos(PuertoCOM, cadena, indice,trCtrl,trDatos,campo,fichero,esCabecera,tamano,nomFichero, logF5, flujoLOG,color);
		printf("\n");
		indice = 0;
		break;

	case 60: //--------------------------------------------------------Pulsado F2   -   ENVIO DE TRAMA DE CONTROL
		TeclaFuncionF2_enviarTramaControl(PuertoCOM, salir, flujoLOG, logF5);
		break;

	case 61: //--------------------------------------------------------Pulsado F3   -   ENVIO DE FICHERO
			F3_enviarFichero(PuertoCOM, indice,flujoLOG, logF5,color,salir);
			printf("\n");
			break;

	case 63: //--------------------------------------------------------Pulsado F5 - Bandera para permiso de escritura en log.txt
			printf("\n\n");
			printf("¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯\n");
			printf("                    PULSADO F5 - Inicio registro en log.txt                                \n");
			printf("_______________________________________________________________________________________\n\n\n");
			logF5=true;

		break;

	case 64:
			logF5=false;		//Cancelación de fichero log.txt
			flujoLOG.close();
			F6_eleccionRolMaestroEsclavo(PuertoCOM,salir);
		break;
	default:
		printf("ERROR. Seleccione una FUNCIÓN correcta\n");
		break;
	}

	flujoLOG.close();	//Cierre del flujo para el archivo log.txt
}



