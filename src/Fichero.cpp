//============================================================================
// Name        : Fichero.cpp
// Author      : Juan Antonio Silva Luján
//             : Alberto Mangut Bustamante
// GRUPO       : L6 - Miércoles de 10.00 a 11.30
// Version     : Sesion 4
// Description : Sesion 4 (FRC)
//============================================================================
#include "Fichero.h"


/**
 * FUNCION: F3_enviarFichero
 * PARAMETROS: HANDLE PuertoCOM, int indice, ofstream &flujoLOG, bool &logF5,int &color, bool &salir
 * DESCRIPCION: Realiza un envio de un fichero llamado "fichero-e.txt" situado en la raiz del proyecto.
 */
void F3_enviarFichero(HANDLE PuertoCOM, int indice, ofstream &flujoLOG, bool &logF5,int &color, bool &salir) {

	// Lectura y escritura simult‡nea de caracteres:
	//Las teclas de funcion se representan con \0 y un valor, se debe leer dos veces

	string nomFichero;
	TramaDatos tramDatos;
	TramaControl tramCtrl;

	bool fichero=false,tamano=false;
	char lineasCabecerasF[50],car = '0', cadenaCuerpoFichero[255];
	int esCabecera=0, campo=1, contadorLineas = 0, contCaracteres = 0;;

	//Declaración y apertura del FICHERO Fenvio.txt
	ifstream flujo("fichero-e.txt");


	if (flujo.is_open()) {

		printf("Comenzando con el envio del Fichero....   ");
		if(logF5)	flujoLOG<<"Comenzando con el envio del Fichero....\n";

		EnviarCaracter(PuertoCOM, '{');     //Enviamos el caracter {

		while (!flujo.eof() && !salir) {//Ejecutamos hasta el final del fichero o hasta que se pulse la tecla de ESC.

			if (kbhit()) {
				car = getch();

				if (car == 27) {
					salir = true;
					printf("[!] Detectada tecla [ESC], cancelando accion...\n");
				}
			}


			//Envío de trama de las 3 primeras líneas de cabecera (Nombre y apellidos de los autores)
			//Color cabecera(2º Línea)
			//Nombre y extension del fichero  (3º Línea)
			if (contadorLineas < 3) {
				flujo.getline(lineasCabecerasF, 50,'\n');

				//La 2º línea es el autor, debe enviarse esta impresión por pantalla en el envío del emisor
				if (contadorLineas == 0) {
					cout << "Enviando FICHERO por AUTOR [" << lineasCabecerasF<< "]" << endl;
					if(logF5)	flujoLOG<<"Enviando FICHERO por AUTOR [" << lineasCabecerasF<< "]\n";
				}

				TramaDatos TrmDatos;
				strcpy(TrmDatos.datos,lineasCabecerasF);
				TeclaFuncionF2_envioPorDefecto(PuertoCOM,TrmDatos, flujoLOG, logF5);

			} else if (contadorLineas >= 3) {

				cadenaCuerpoFichero[0] = '\0';
				flujo.read(cadenaCuerpoFichero, 254); //Lectura cada 254 caracteres
				cadenaCuerpoFichero[flujo.gcount()] = '\0';
				contCaracteres = contCaracteres + strlen(cadenaCuerpoFichero);
				cadenaCuerpoFichero[strlen(cadenaCuerpoFichero)] = '\0'; //Aplico \0 al final

				//Envio de la TRAMA

				TeclaFuncionF1_enviarTramaDatos(PuertoCOM, cadenaCuerpoFichero, strlen(cadenaCuerpoFichero),tramCtrl,tramDatos,campo,fichero,esCabecera,tamano,nomFichero, logF5, flujoLOG,color);

			} else {
				printf("ERROR. Cuerpo del FICHERO vacío.\n");

			}
			contadorLineas++;
		}
	} else {
		printf("ERROR al abrir fichero-e.txt.");
	}

	//Fichero enviado, enviar caracter }
	EnviarCaracter(PuertoCOM, '}');
	//Envio  de numero de caracteres
	cadenaCuerpoFichero[0]='\0';
	itoa(contCaracteres,cadenaCuerpoFichero,10);
	cadenaCuerpoFichero[strlen(cadenaCuerpoFichero)]='\0';
	TeclaFuncionF1_enviarTramaDatos(PuertoCOM, cadenaCuerpoFichero,strlen(cadenaCuerpoFichero),tramCtrl,tramDatos,campo,fichero,esCabecera,tamano,nomFichero, logF5, flujoLOG,color);
	HANDLE Pantalla= GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute (Pantalla,  14 + 0*16);
	printf("Envio de fichero finalizado.");
	if(logF5)	flujoLOG<<"Envio de fichero finalizado.\n";

	//Cierre del flujo
	flujo.close();


}
