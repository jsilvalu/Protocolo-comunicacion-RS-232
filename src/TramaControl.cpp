//============================================================================
// Author      : Juan Antonio Silva Luján
//             : Alberto Mangut Bustamante
// GRUPO       : L6 - Miércoles de 10.00 a 11.30
// Version     : Sesion 2
// Description : Sesion 2 (FRC)
//============================================================================


#include "TramaControl.h"

/**
 * FUNCION: TeclaFuncionF2_enviarTramaControl
 * PARAMETROS: HANDLE PuertoCOM, bool &salir, ofstream &flujoLOG, bool &logF5
 * DESCRIPCION: Una vez que se ha pulsado la tecla F2, se envia una trama, procedimiento:
 *
 *		1. Solicitud al usuario de que trama desea enviar
 *		2. Selección de la trama
 */
void TeclaFuncionF2_enviarTramaControl(HANDLE PuertoCOM, bool &salir, ofstream &flujoLOG, bool &logF5) {

	//Creacion de la Trama de Control que se enviará
	TramaControl TrCtrl;
	bool exit = false;
	char car = 0;

	//En principio, valores constantes
	TrCtrl.Sincronismo = 22;
	TrCtrl.Direccion = 'T';
	TrCtrl.NumTrama = '0';

	HANDLE Pantalla= GetStdHandle(STD_OUTPUT_HANDLE);

	SetConsoleTextAttribute (Pantalla,  10 + 0*16);

	//Se muestra el menu de opciones
	while (!exit && !salir) {
		printf("\n_______________________________________________\n");
		printf("SELECCIONE LA TRAMA A UTILIZAR: \n");
		printf("1.    TRAMA DE CONTROL  [ENQ]   \n");
		printf("2.    TRAMA DE CONTROL  [EOT]   \n");
		printf("3.    TRAMA DE CONTROL  [ACK]   \n");
		printf("4.    TRAMA DE CONTROL  [NACK]  \n");
		if(logF5){
			flujoLOG<<"\n_______________________________________________\n";
			flujoLOG<<"SELECCIONE LA TRAMA A UTILIZAR: \n";
			flujoLOG<<"1.    TRAMA DE CONTROL  [ENQ]   \n";
			flujoLOG<<"2.    TRAMA DE CONTROL  [EOT]   \n";
			flujoLOG<<"3.    TRAMA DE CONTROL  [ACK]   \n";
			flujoLOG<<"4.    TRAMA DE CONTROL  [NACK]  \n"<<endl;
		}

		car = getch();
		//Control pulsacion escape
		if (car == 27) {
			salir = true;
		} else {

			SetConsoleTextAttribute (Pantalla,  10 + 8*16);
			//Switch para enviar cada tipo de trama de control.
			switch (car) {

			case 49:  //Caso para la TRAMA de CONTROL [ENQ]--------------------------------------------- Trama de Control [ENQ]
				TrCtrl.Control = 5;
				EnviarCaracter(PuertoCOM, TrCtrl.Sincronismo);
				EnviarCaracter(PuertoCOM, TrCtrl.Direccion);
				EnviarCaracter(PuertoCOM, TrCtrl.Control);
				EnviarCaracter(PuertoCOM, TrCtrl.NumTrama);
				printf("Trama de Control [ENQ] enviada\n");
				SetConsoleTextAttribute (Pantalla,  10 + 0*16);
				printf("_______________________________________________\n\n");
				if(logF5)	flujoLOG<<"Trama de Control [ENQ] enviada\n"<<"_______________________________________________\n\n";

				exit = true;
				break;

			case 50:  //Caso para la TRAMA de CONTROL [EOT]--------------------------------------------- Trama de Control [EOT]
				TrCtrl.Control = 4;
				EnviarCaracter(PuertoCOM, TrCtrl.Sincronismo);
				EnviarCaracter(PuertoCOM, TrCtrl.Direccion);
				EnviarCaracter(PuertoCOM, TrCtrl.Control);
				EnviarCaracter(PuertoCOM, TrCtrl.NumTrama);
				printf("Trama de Control [EOT] enviada\n");
				SetConsoleTextAttribute (Pantalla,  10 + 0*16);
				printf("_______________________________________________\n\n");
				if(logF5)	flujoLOG<<"Trama de Control [EOT] enviada\n"<<"_______________________________________________\n\n";
				exit = true;
				break;

			case 51:  //Caso para la TRAMA de CONTROL [ACK]--------------------------------------------- Trama de Control [ACK]
				TrCtrl.Control = 6;
				EnviarCaracter(PuertoCOM, TrCtrl.Sincronismo);
				EnviarCaracter(PuertoCOM, TrCtrl.Direccion);
				EnviarCaracter(PuertoCOM, TrCtrl.Control);
				EnviarCaracter(PuertoCOM, TrCtrl.NumTrama);
				printf("Trama de Control [ACK] enviada\n");
				SetConsoleTextAttribute (Pantalla,  10 + 0*16);
				printf("_______________________________________________\n\n");
				if(logF5)	flujoLOG<<"Trama de Control [ACK] enviada\n"<<"_______________________________________________\n\n";
				exit = true;
				break;

			case 52:  //Caso para la TRAMA de CONTROL [NACK]--------------------------------------------- Trama de Control [NACK]

				TrCtrl.Control = 21;
				EnviarCaracter(PuertoCOM, TrCtrl.Sincronismo);
				EnviarCaracter(PuertoCOM, TrCtrl.Direccion);
				EnviarCaracter(PuertoCOM, TrCtrl.Control);
				EnviarCaracter(PuertoCOM, TrCtrl.NumTrama);
				printf("Trama de Control [NACK] enviada\n");
				SetConsoleTextAttribute (Pantalla,  10 + 0*16);
				printf("_______________________________________________\n\n");
				if(logF5)	flujoLOG<<"Trama de Control [NACK] enviada\n"<<"_______________________________________________\n\n";
				exit = true;
				break;

			default:
				//Mostramos mensaje de error en caso de que no se seleccione una opcion correcta----------------------------ERROR
				SetConsoleTextAttribute (Pantalla,  12 + 0*16);
				printf("ERROR. Seleccione una de las opciones [1-4]\n");
				printf("\n");
				printf("\n");
				SetConsoleTextAttribute (Pantalla,  10 + 0*16);
				break;
			}
		}
	}

	SetConsoleTextAttribute (Pantalla,  15 + 0*16);
}
