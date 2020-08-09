//============================================================================
// Author      : Juan Antonio Silva Luján
//             : Alberto Mangut Bustamante
// GRUPO       : L6 - Miércoles de 10.00 a 11.30
// Version     : Sesion 1
// Description : Sesion 1 (FRC)
//============================================================================

//============================================================================
// ----------- PRÁCTICAS DE FUNDAMENTOS DE REDES DE COMUNICACIONES -----------
// ---------------------------- CURSO 2019/20 --------------------------------
// ----------------------------- SESION1.CPP ---------------------------------
//============================================================================

#include <stdio.h>
#include <conio.h>
#include <iostream>
#include <string.h>
#include <windows.h>
#include <fstream>
#include <wchar.h>
#include <locale.h>

using namespace std;

/*Clases propias del proyecto*/
#include "PuertoSerie.h"
#include "TeclasFuncion.h"
#include "TipoTrama.h"
#include "Recibir.h"

HANDLE Pantalla;
HANDLE puertoCOM;
const int MAX = 800;

/**
 * FUNCIÓN: seleccionPuertoVelocidad
 *
 * DESCRIPCIÓN: Selecciona la apertura de un puerto así como su velocidad. Esta selección será llevada a cabo manualmente
 * por el usuario mediante teclado.
 */
void seleccionPuertoVelocidad() {

	bool salirPuerto = false, salirVelocidad = false;
	int eleccionPuerto, eleccionVelocidad;
	const char* puerto;
	while (!salirPuerto) {

		printf("SELECCIONE EL PUERTO A UTILIZAR: \n");
		printf("1.    COM1 \n");
		printf("2.    COM2 \n");
		printf("3.    COM3 \n");
		printf("4.    COM4 \n");
		scanf("%d", &eleccionPuerto);

		switch (eleccionPuerto) {
		case 1:
			puerto = "COM1";
			salirPuerto = true;
			break;

		case 2:
			puerto = "COM2";
			salirPuerto = true;

			break;

		case 3:
			puerto = "COM3";
			salirPuerto = true;

			break;

		case 4:
			puerto = "COM4";
			salirPuerto = true;
			break;

		default:

			printf("ERROR. Eleccion del puerto incorrecta (1-4): \n");
			salirPuerto = false; //Debemos acabar el programa o pedir un nuevo valor
			break;
		}

	}

	while (!salirVelocidad) {

		printf("\n");
		printf("SELECCIONE LA VELOCIDAD DE TRANSMION: \n");
		printf("1.    1200 \n");
		printf("2.    2400 \n");
		printf("3.    4800 \n");
		printf("4.    9600 \n");
		printf("5.    19200 \n");
		scanf("%d", &eleccionVelocidad);

		switch (eleccionVelocidad) {
		case 1:
			eleccionVelocidad = 500;
			puertoCOM = AbrirPuerto(puerto, eleccionVelocidad, 8, 0, 0);
			salirVelocidad = true;
			break;
		case 2:
			eleccionVelocidad = 2400;
			puertoCOM = AbrirPuerto(puerto, eleccionVelocidad, 8, 0, 0);
			salirVelocidad = true;
			break;
		case 3:
			eleccionVelocidad = 4800;
			puertoCOM = AbrirPuerto(puerto, eleccionVelocidad, 8, 0, 0);
			salirVelocidad = true;
			break;
		case 4:
			eleccionVelocidad = 9600;
			puertoCOM = AbrirPuerto(puerto, eleccionVelocidad, 8, 0, 0);
			salirVelocidad = true;
			break;
		case 5:
			eleccionVelocidad = 19200;
			puertoCOM = AbrirPuerto(puerto, eleccionVelocidad, 8, 0, 0);
			salirVelocidad = true;
			break;

		default:
			printf("ERROR. Eleccion de velocidad incorrecta (1-5): \n");
		}
	}
	if(salirPuerto && salirVelocidad){
		printf("\n");
		printf("Abriendo puerto COM%d a %d de velocidad\n", eleccionPuerto, eleccionVelocidad);
	}
}

/**
 * FUNCIÓN: accion
 *
 * DESCRIPCIÓN: Gestiona la acción que lleva a cabo una de las terminales abiertas.
 */
void accion() {

	string nomFichero;
	TramaDatos tramDatos;
	TramaControl tramCtrl;
	char car = 0, cadena[802];
	//Lectura y escritura simult‡nea de caracteres:
	//Las teclas de funcion se representan con \0 y un valor, se debe leer dos veces
	int indice = 0, campo = 1, esCabecera=0,color=-1;
	bool fichero=false,tamano=false, logF5=false, salir = false;

	HANDLE Pantalla= GetStdHandle(STD_OUTPUT_HANDLE);


	while (car != 27 && !salir) { // 27 = ESC   Mientras que no pulsemos ESC la ejecucion continua.

		recibir(puertoCOM,campo,tramCtrl,tramDatos,fichero,esCabecera,tamano,nomFichero, logF5,color);
		SetConsoleTextAttribute (Pantalla,  14 + 0*16);
		//En caso de que se realice una pulsacion de una tecla, se estudiará de que tecla se trata.
		if (kbhit()) {

			car = getch();

			if (car != 27) {

				switch (car) {

					case ('\0'):   //Tecla de Función F_--------------------------------

						car = getch();
						//Clase TeclasFuncion  -  eleccionTeclaFuncionF_();
						//Comprueba con un switch cual ha sido la tecla de Función F_ pulsada
						eleccionTeclaFuncionF_(indice, car, cadena, puertoCOM, salir, tramCtrl,tramDatos,campo,fichero,esCabecera,tamano,nomFichero, logF5,color);
						break;

					case (8): //Cuando se pulsa la tecla [DELETE]-------------------------
						if (indice != 0) {
							printf("%c", car);
							printf(" ");
							printf("%c", car);
							cadena[indice] = '\0';
							indice--;
						}
						break;

					case (13):  //Cuando se pulsa el [Retorno de carro]-------------------
						if (indice < MAX) {
							printf("\n");
							cadena[indice] = '\n';
							indice++;
						}
						break;

					default:    //Cualquier otra tecla pulsada-----------------------------
						if (indice < MAX) {
							printf("%c", car);
							cadena[indice] = car;
							indice++;
						}
						break;
					}
			} else {
				printf("[!] Detectada tecla [ESC], cancelando accion...\n");
			}
		}
	}
	SetConsoleTextAttribute (Pantalla,  15 + 0*16);
}





int main() {

	//Encabezado
	printf("============================================================================\n");
	printf("----------- PRACTICAS DE FUNDAMENTOS DE REDES DE COMUNICACIONES ------------\n");
	printf("---------------------------- CURSO 2019/20 ---------------------------------\n");
	printf("----------------------------- SESION1.CPP ----------------------------------\n");
	printf("============================================================================\n\n");

	//Abrimos el puerto. Para ello necesitamos indicar los siguientes parámetros:
	// - Nombre del puerto a abrir: ("COM1", "COM2", "COM3", ...).
	// - Velocidad: (1200, 2400, 4800, 9600, 19200, 38400, 57600, 115200).
	// - Número de bits en cada byte enviado o recibido: (4, 5, 6, 7, 8).
	// - Paridad: (0=sin paridad, 1=impar, 2=par, 3=marca, 4=espacio).
	// - Bits de stop: (0=1 bit, 1=1.5 bits, 2=2 bits).

	//Abrimos el puerto COM1 (en la sala siempre abrimos el COM1)

	seleccionPuertoVelocidad();
	//Necesario borrar el contenido del archivo de la ejecución anterior, de lo contrario
	//posicionaría el puntero en la última posición de la anterior escritura.
	remove("log.txt");

	if (puertoCOM == NULL) {
		printf("Error al abrir el puerto \n");
		return (1);

	} else {
		accion();
	}

	// Cerramos el puerto:
	CerrarPuerto(puertoCOM);

	return (0);

}
