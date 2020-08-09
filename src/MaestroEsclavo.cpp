//============================================================================
// Author      : Juan Antonio Silva Luján
//             : Alberto Mangut Bustamante
// GRUPO       : L6 - Miércoles de 10.00 a 11.30
// Version     : Sesion 5
// Description : Sesion 5 (Protocolo Maestro-Esclavo)
//============================================================================

/**
 * F4_eleccionRolMaestroEsclavo
 *
 * Descripción: Se presenta al usuario en la terminal diversas opciones para
 * seleccionar que rool desea que asuma la terminal.
 * En primer lugar se solicita al usuario si desea ser MAESTRO o ESCLAVO
 * en caso de asumir el rol de MAESTRO, el usuario deberá decidir el tipo de operación
 * a realizar en el proceso de ejecución (selección o sondeo).
 */
#include "TramaControl.h"
#include "MaestroEsclavo.h"
#include "PuertoSerie.h"
#include "FicheroMaestro.h"

#include <conio.h>
#include <stdio.h>
#include <iostream>
#include <string.h>
#include <windows.h>
#include <fstream>
#include <wchar.h>
#include <locale.h>


/**
 * F6_eleccionRolMaestroEsclavo
 * Descripción:
 * Función gestora para decisión de que terminal es el Maestro y cual Esclavo
 *
 */
void F6_eleccionRolMaestroEsclavo(HANDLE PuertoCOM, bool &salir) {

	HANDLE Pantalla= GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute (Pantalla,  11 + 0*16);

	int eleccionRol;
	printf("==================================================================================\n");
	printf("==================================================================================\n");
	printf("                            PROTOCOLO MAESTRO-ESCLAVO                             \n");
	printf("==================================================================================\n\n");
	printf("==================================================================================\n\n");
	SetConsoleTextAttribute (Pantalla,  12 + 0*16);
	printf(" Seleccione el rol que desea asumir en este terminal:\n");
	printf(" 1. Maestro\n");
	printf(" 2. Esclavo\n");
	scanf("%d", &eleccionRol);

	SetConsoleTextAttribute (Pantalla, 2 + 0*16);
	switch (eleccionRol) {

	case 1:

		int eleccionOpe;

		printf(" _______________________________\n");
		printf("|  Esta terminal es el MAESTRO |\n");
		printf("|______________________________|\n");
		printf("Maestro - Seleccione la operación a realizar:\n");
		printf(" 1. Seleccion  (El Maestro envía el fichero)\n");
		printf(" 2. Sondeo     (El esclavo envía el fichero)\n");
		scanf("%d", &eleccionOpe);

		switch (eleccionOpe) {
		case 1:
			printf("Ejecutando MAESTRO - SELECCION\n\n");
			F6_ejecucionMaestro_Seleccion(PuertoCOM,salir);
			break;

		case 2:
			printf("Ejecutando MAESTRO - SONDEO\n");
			F6_ejecucionMaestro_Sondeo(PuertoCOM);
			break;

		default:
			printf("Opción Incorrecta. Introducir 1 o 2\n");
			break;
		}
		break;

	case 2:
		printf(" ______________________________\n");
		printf("|  Este terminal es el ESCLAVO |\n");
		printf("|______________________________|\n\n");
		F6_ejecucionEsclavo(PuertoCOM);
		break;

	default:
		printf("Opción Incorrecta. Introducir 1. Maestro 2.Esclavo\n");
		break;

	}
}

void F6_ejecucionMaestro_Seleccion(HANDLE PuertoCOM,bool &salir) {

	//Primera parte:  ESTABLECIMIENTO
	char indice = '0';
	establecerComunicacionMaestroSeleccion(PuertoCOM);
	bool correcto = recibirConfirmacionMaestroSeleccion(PuertoCOM, indice);

	//Segunda parte: TRANSFERENCIA

	if (correcto)	F3_enviarFicheroMaestro(PuertoCOM, true,salir);

	//Tercera parte: CIERRE - Petición
	liberarComunicacionMaestroSeleccion(PuertoCOM);
	if (!recibirConfirmacionMaestroSeleccion(PuertoCOM, '0'))
		printf("Error al finalizar la comunicacion con el esclavo.");

}


void F6_ejecucionMaestro_Sondeo(HANDLE PuertoCOM) {

	TramaControl TrCtrl;
	bool salir = false;
	int indice = 0;
	char numtrama = '0';

	//Primera parte:  ESTABLECIMIENTO
	establecerComunicacionMaestroSondeo(PuertoCOM);
	bool correcto = recibirConfirmacionMaestroSondeo(PuertoCOM, '0');

	//Segunda parte: TRANSFERENCIA
	if (correcto)	recibirMaestroEsclavo(PuertoCOM, true);


	//Tercera parte: CIERRE - Petición
	while (salir == false) {

		numtrama = (indice % 2 == 0) ? '0' : '1';

		TrCtrl = recibirTramaControl(PuertoCOM);
		if (comprobarTrama(TrCtrl, numtrama)) {
			int decisionLiberar;
			printf("MAESTRO:  ¿ desea cerrar la comunicación ?\n");
			printf("1. SI     2. NO\n");
			scanf("%d", &decisionLiberar);

			if (decisionLiberar == 1) {
				salir = true;
				liberarComunicacionMaestroSondeo(PuertoCOM, numtrama);
			}

			else if (decisionLiberar == 2) {

				denegarLiberacionMaestroSondeo(PuertoCOM, numtrama);
				indice++;
			} else
				(cout << "opcion incorrecta");
		}
	}
}

void F6_ejecucionEsclavo(HANDLE PuertoCOM) {

	TramaControl comunicacion = recibirTramaControl(PuertoCOM);
	//llamada al método que determinará el tipo de comunicacion. mostrarTramaControl(comunicacion, 1);
	int tipoComunicacion = establecimientoComunicacionEsclavo(PuertoCOM, comunicacion);

	if (tipoComunicacion == 1)	esclavoSeleccion(PuertoCOM);
	else{

		if(tipoComunicacion == 2)
			esclavoSondeo(PuertoCOM);
		else
			cout << "Error en el establecimiento de la comunicacion";
	}


}




void esclavoSondeo(HANDLE PuertoCOM) {
	bool salir = false,escape=false;
	int indice = 0;
	char numtrama = '0';
	enviarConfirmacionEsclavoSondeo(PuertoCOM, '0');

	F3_enviarFicheroMaestro(PuertoCOM, false,escape);

	while (salir == false) {

		(indice % 2 == 0) ? numtrama = '0' : numtrama = '1';

		solicitarLiberacion(PuertoCOM, numtrama);
		indice++;

		salir = comprobarSalir(recibirTramaControl(PuertoCOM), numtrama);
	}

}

void esclavoSeleccion(HANDLE PuertoCOM) {

	enviarConfirmacionEsclavoSeleccion(PuertoCOM, '0');
	recibirMaestroEsclavo(PuertoCOM, false);
	bool correcto = liberarComunicacionEsclavo(PuertoCOM);

	(correcto) ? enviarConfirmacionEsclavoSeleccion(PuertoCOM, '0') : cout << "Error al finalizar la comunicacion";

}


bool liberarComunicacionEsclavo(HANDLE PuertoCOM) {
	TramaControl TrCtrl = recibirTramaControl(PuertoCOM);
	if (TrCtrl.Control != 02) { //Comprobamos que se trata de una trama de control
		char tipoTrama = TrCtrl.Control;
		switch (tipoTrama) {
		case 4: //Se trata de una trama EOT

			if (TrCtrl.NumTrama == '0') { //comprobamos que el numero de trama es correcto.
				mostrarTramaControl(TrCtrl, 1);
				return true;

			} else return false;

			break;

		default:
			printf("No se reconoce el tipo de trama");
			return false;
			break;
		}

	}

	return false;
}

int establecimientoComunicacionEsclavo(HANDLE PuertoCOM, TramaControl TrCtrl) {
	int tipoComunicacion = 0;
	char tipoTrama = ' ';
	if (TrCtrl.Control != 02) {

		tipoTrama = TrCtrl.Control;

		switch (tipoTrama) {

		case 5:
			if (TrCtrl.NumTrama == '0') {//comprobamos que el numero de trama es correcto.

				if (TrCtrl.Direccion == 'R')//comprobacion del tipo de comunicacion que se desea establecer.
					tipoComunicacion = 1;	//se trata de Seleccion
				 else if (TrCtrl.Direccion == 'T')
					tipoComunicacion = 2;	//se trata de Sondeo
			}
			break;

		default:
			printf("No se reconoce el tipo de trama");

			break;
		}

	}
	return tipoComunicacion;
}

/**
 * enviarTramaControl_MaestroEsclavo
 *
 * Realiza un envío de la Trama de Control a través de sus parámetros.
 */
void enviarTramaControl_MaestroEsclavo(HANDLE PuertoCOM, TramaControl TrCtrl) {

	EnviarCaracter(PuertoCOM, TrCtrl.Sincronismo);  //SINCRONISMO =  SYN = '22'
	EnviarCaracter(PuertoCOM, TrCtrl.Direccion); 	//DIRECCIÓN   =  (En principio fijo a 'T')
	EnviarCaracter(PuertoCOM, TrCtrl.Control); 		//CONTROL     =  05 ENQ | 04 EOT | 06 ACK | 21 NACK
	EnviarCaracter(PuertoCOM, TrCtrl.NumTrama); 	//Nº TRAMA    =  (En principio fijo a '0')

	mostrarTramaControl(TrCtrl, 0);
}

void establecerComunicacionMaestroSeleccion(HANDLE PuertoCOM) {

	TramaControl TrCtrl;

	TrCtrl.Sincronismo = 22;
	TrCtrl.Direccion = 'R';
	TrCtrl.Control = 5;
	TrCtrl.NumTrama = '0';
	enviarTramaControl_MaestroEsclavo(PuertoCOM, TrCtrl);

}

void establecerComunicacionMaestroSondeo(HANDLE PuertoCOM) {

	TramaControl TrCtrl;

	TrCtrl.Sincronismo = 22;
	TrCtrl.Direccion = 'T';
	TrCtrl.Control = 5;
	TrCtrl.NumTrama = '0';
	enviarTramaControl_MaestroEsclavo(PuertoCOM, TrCtrl);
}

void liberarComunicacionMaestroSeleccion(HANDLE PuertoCOM) {
	TramaControl TrCtrl;
	TrCtrl.Sincronismo = 22;
	TrCtrl.Direccion = 'R';
	TrCtrl.Control = 4;
	TrCtrl.NumTrama = '0';
	enviarTramaControl_MaestroEsclavo(PuertoCOM, TrCtrl);

}

void liberarComunicacionMaestroSondeo(HANDLE PuertoCOM, char indice) {
	TramaControl TrCtrl;
	TrCtrl.Sincronismo = 22;
	TrCtrl.Direccion = 'T';
	TrCtrl.Control = 6;
	TrCtrl.NumTrama = indice;
	enviarTramaControl_MaestroEsclavo(PuertoCOM, TrCtrl);

}

void denegarLiberacionMaestroSondeo(HANDLE PuertoCOM, char indice) {
	TramaControl TrCtrl;
	TrCtrl.Sincronismo = 22;
	TrCtrl.Direccion = 'T';
	TrCtrl.Control = 21;
	TrCtrl.NumTrama = indice;
	enviarTramaControl_MaestroEsclavo(PuertoCOM, TrCtrl);

}

void solicitarLiberacion(HANDLE PuertoCOM, char indice) {
	TramaControl TrCtrl;
	TrCtrl.Sincronismo = 22;
	TrCtrl.Direccion = 'T';
	TrCtrl.Control = 4;
	TrCtrl.NumTrama = indice;
	enviarTramaControl_MaestroEsclavo(PuertoCOM, TrCtrl);

}

void errorTramaSeleccion(HANDLE PuertoCOM, char indice) {
	TramaControl TrCtrl;
	TrCtrl.Sincronismo = 22;
	TrCtrl.Direccion = 'R';
	TrCtrl.Control = 21;
	TrCtrl.NumTrama = indice;
	enviarTramaControl_MaestroEsclavo(PuertoCOM, TrCtrl);

}

void errorTramaSondeo(HANDLE PuertoCOM, char indice) {
	TramaControl TrCtrl;
	TrCtrl.Sincronismo = 22;
	TrCtrl.Direccion = 'T';
	TrCtrl.Control = 21;
	TrCtrl.NumTrama = indice;
	enviarTramaControl_MaestroEsclavo(PuertoCOM, TrCtrl);

}

void enviarConfirmacionEsclavoSeleccion(HANDLE PuertoCOM, char indice) {

	TramaControl TrCtrl;
	//Enviamos trama de control para confirmar la comunicacion.
	TrCtrl.Sincronismo = 22;
	TrCtrl.Direccion = 'R';
	TrCtrl.Control = 6;
	TrCtrl.NumTrama = indice;
	enviarTramaControl_MaestroEsclavo(PuertoCOM, TrCtrl);

}

void enviarConfirmacionEsclavoSondeo(HANDLE PuertoCOM, char indice) {

	TramaControl TrCtrl;
	//Enviamos trama de control para confirmar la comunicacion.
	TrCtrl.Sincronismo = 22;
	TrCtrl.Direccion = 'T';
	TrCtrl.Control = 6;
	TrCtrl.NumTrama = indice;
	enviarTramaControl_MaestroEsclavo(PuertoCOM, TrCtrl);

}

TramaControl recibirTramaControl(HANDLE PuertoCOM) {

	TramaControl TrCtrl;
	char caracterRecibido = 0;

	int campo = 1;
	bool salir = false;
	while ((caracterRecibido == 0 && salir == false) || campo != 5) {

		char caracterRecibido = RecibirCaracter(PuertoCOM);

		//Cuando campo=1 en caso de que el caracter recibido sea igual a 22 significara que estamos
		//recibiendo una trama. En caso contrario se imprimirá por pantalla el caracter recibido.
		//Si se trata de una trama almacenaremos los caracteres recibidos en la trama que es recibida por referencia.
		//Cuando campo=4 significa que hemos recibido toda la trama. Seguidamente estudiamos si se trata de
		//una trama de control o de datos. En caso de tratarse del primer tipo mostramos por pantalla el tipo de trama recibida.

		//_________________________________________________Caso para SINCRONISMO

		if (caracterRecibido != 0) {

			switch (campo) {
			case 1:	//_________________________________________________Caso para SINCRONISMO

				if (caracterRecibido == 22) {
					TrCtrl.Sincronismo = caracterRecibido;
					campo++;
				}
				break;

			case 2:	//_________________________________________________Caso para DIRECCIÓN

				TrCtrl.Direccion = (unsigned char) caracterRecibido;
				campo++;
				break;

			case 3:	//_________________________________________________Caso para CONTROL

				TrCtrl.Control = (unsigned char) caracterRecibido;
				campo++;
				break;

			case 4:	//_________________________________________________Caso para NÚMERO DE TRAMA

				if (TrCtrl.Control != 02) {	//Si es distinto de 02 se trata de una trama de control y no de datos.
					TrCtrl.NumTrama = (unsigned char) caracterRecibido;
					campo++;
					salir = true;

				}

			}
		}
	}

	return TrCtrl;

}

bool calcularBCE_recepcionMaestro(TramaDatos TrDato, char campoDatos[], unsigned char &resulBCE) {

	bool resultado = false;
	resulBCE = campoDatos[0];
	int contador = 1;

	while (contador < TrDato.Longitud) {
		resulBCE = resulBCE ^ campoDatos[contador];
		contador++;
	}

	if (resulBCE == 0 || resulBCE == 255)	resulBCE = 1;

	if (resulBCE == TrDato.BCE)	resultado = true;

	return resultado;
}

void mostrarTramaControl(TramaControl TrCtrl, bool esReceptor) {

	char tipoTrama = TrCtrl.Control;
	char emisorReceptor = ' ';
	string tipo;
	switch (tipoTrama) {

	case 4:	tipo = "EOT";	break;

	case 5:	tipo = "ENQ";	break;

	case 6:	tipo = "ACK";	break;

	case 21:tipo = "NACK";	break;

	}

	emisorReceptor = (esReceptor) ?  'R' :  'E';
	printf("%c %c ", emisorReceptor, TrCtrl.Direccion);
	printf("%s", tipo.c_str());
	printf(" %C \n", TrCtrl.NumTrama);
	printf("------------------------------------\n\n");

}

void mostrarTramaDatoEmisor(TramaDatos TrCtrl) {

	char tipoTrama = TrCtrl.Control;
	string tipo;
	switch (tipoTrama) {

	case '2':	tipo = "STX";	break;

	}

	printf("E %c ", TrCtrl.Direccion);
	cout << tipo;
	printf(" %c %u \n", TrCtrl.NumTrama, TrCtrl.BCE);
	printf("------------------------------------\n\n");

}

void mostrarTramaDatoReceptor(TramaDatos TrCtrl, unsigned char resulBCE) {

	char tipoTrama = TrCtrl.Control;
	string tipo;
	switch (tipoTrama) {

		case '2':	tipo = "STX";	break;

	}

	printf("R %c ", TrCtrl.Direccion);
	cout << tipo;
	printf(" %c %u %u \n", TrCtrl.NumTrama, TrCtrl.BCE, resulBCE);
	printf("------------------------------------\n\n");

}

bool recibirConfirmacionMaestroSeleccion(HANDLE PuertoCOM, int indice) {

	TramaControl TrCtrl = recibirTramaControl(PuertoCOM);

	if (TrCtrl.Control != 02) { //Comprobamos que se trata de una trama de control
		char tipoTrama = TrCtrl.Control;

		switch (tipoTrama) {

		case 6: //Se trata de una trama ACK

			if (TrCtrl.NumTrama == indice) { //comprobamos que el numero de trama es correcto.

				if (TrCtrl.Direccion == 'R') { //comprobacion del tipo de comunicacion que se desea establecer.
					mostrarTramaControl(TrCtrl, 1);

					return true; //Trama correcta.

				} else {
					return false;
					cout << "Error en la trama de confirmacion";
				}
			}
			break;
		case 21: //se ha detectado un error en la trama
			if (TrCtrl.NumTrama == indice) { //comprobamos que el numero de trama es correcto.

				if (TrCtrl.Direccion == 'R') { //comprobacion del tipo de comunicacion que se desea establecer.
					mostrarTramaControl(TrCtrl, 1);
					return false; //Trama correcta.

				} else {
					return false;
					cout << "Error en la trama de confirmacion";
				}
			}
			break;
			break;
		default:
			printf("No se reconoce el tipo de trama");
			return false;
			break;
		}

	}

	return false;
}

bool recibirConfirmacionMaestroSondeo(HANDLE PuertoCOM, int indice) {

	TramaControl TrCtrl = recibirTramaControl(PuertoCOM);

	if (TrCtrl.Control != 02) { //Comprobamos que se trata de una trama de control
		char tipoTrama = TrCtrl.Control;

		switch (tipoTrama) {

		case 6: //Se trata de una trama ACK

			if (TrCtrl.NumTrama == indice) { //comprobamos que el numero de trama es correcto.

				if (TrCtrl.Direccion == 'T') { //comprobacion del tipo de comunicacion que se desea establecer.
					mostrarTramaControl(TrCtrl, 1);

					return true; //Trama correcta.

				} else {
					return false;
					cout << "Error en la trama de confirmacion";
				}
			}
			break;
		case 21: //se ha detectado un error en la trama
			if (TrCtrl.NumTrama == indice) { //comprobamos que el numero de trama es correcto.

				if (TrCtrl.Direccion == 'T') { //comprobacion del tipo de comunicacion que se desea establecer.
					mostrarTramaControl(TrCtrl, 1);
					return false; //Trama correcta.

				} else {
					return false;
					cout << "Error en la trama de confirmacion";
				}
			}
			break;

		default:
			printf("No se reconoce el tipo de trama");
			return false;
			break;
		}

	}

	return false;
}



bool comprobarTrama(TramaControl TrCtrl, char numtrama) {

	char tipoTrama = ' ';
	if (TrCtrl.Control != 02) {

		tipoTrama = TrCtrl.Control;

		switch (tipoTrama) {

		case 4:
			if (TrCtrl.NumTrama == numtrama) { //comprobamos que el numero de trama es correcto.

				if (TrCtrl.Direccion == 'T') { //comprobacion del tipo de comunicacion que se desea establecer.
					mostrarTramaControl(TrCtrl, true);
					return true;

				}
			}
			break;

		default:
			printf("No se reconoce el tipo de trama");
			return false;
			break;
		}

	}
	return false;

}





void recibirMaestroEsclavo(HANDLE PuertoCOM, bool maestro) {

	// Lectura y escritura simult‡nea de caracteres:
	//Las teclas de funcion se representan con \0 y un valor, se debe leer dos veces

	string nomFichero;
	int campo = 1, contador = 0, esCabecera = 0;
	TramaDatos TrDato;
	bool fichero = false, tamano = false;
	char campoDatos[255], indice = '0', caracterRecibido;
	ofstream flujoSalida;

	while (tamano == false || fichero == false) {

		caracterRecibido = RecibirCaracter(PuertoCOM);

		if (caracterRecibido != 0) {

			//Cuando campo=1 en caso de que el caracter recibido sea igual a 22 significara que estamos
			//recibiendo una trama. En caso contrario se imprimirá por pantalla el caracter recibido.
			//Si se trata de una trama almacenaremos los caracteres recibidos en la trama que es recibida por referencia.
			//Cuando campo=4 significa que hemos recibido toda la trama. Seguidamente estudiamos si se trata de
			//una trama de control o de datos. En caso de tratarse del primer tipo mostramos por pantalla el tipo de trama recibida.

			switch (campo) {
			case 1:	//_________________________________________________Caso para SINCRONISMO

				if (caracterRecibido == 22) {

					//Inicializar la TRAMA
					TrDato.Sincronismo = caracterRecibido;
					campo++;

				} else {

					if (caracterRecibido == '#') //Si '#' se está enviando un FICHERO (en proceso)
						fichero = true;


					if (caracterRecibido == '@') {//Si '@' YA se ha recibido el FICHERO (terminado)

						fichero = false;
						tamano = true;
						flujoSalida.flush();
						flujoSalida.close();
						esCabecera = 0;

					}
				}

				break;

			case 2://_________________________________________________Caso para DIRECCIÓN

				TrDato.Direccion = (unsigned char) caracterRecibido;
				campo++;
				break;

			case 3://_________________________________________________Caso para CONTROL

				TrDato.Control = (unsigned char) caracterRecibido;
				campo++;
				break;

			case 4://_________________________________________________Caso para NÚMERO DE TRAMA

				TrDato.NumTrama = (unsigned char) caracterRecibido;
				campo++;
				break;

			case 5://_________________________________________________Caso para LONGITUD

				TrDato.Longitud = (unsigned char) caracterRecibido;
				campo++;

				/* no break */

			case 6://_________________________________________________Caso para DATOS

				campoDatos[0] = '\0';
				TrDato.datos[0] = '\0';
				RecibirCadena(PuertoCOM, campoDatos, TrDato.Longitud);

				//Se deben recibir los datos completos de una sola vez
				campoDatos[TrDato.Longitud + 1] = '\0';
				strcpy(TrDato.datos, campoDatos);
				campo++;
				break;

			case 7://_________________________________________________Caso para BLOQUE CONTROL ERRORES (BCE)

				(contador % 2 == 0) ? indice = '0' : indice = '1';

				TrDato.BCE = (unsigned char) caracterRecibido;
				campo = 1;
				unsigned char resultBCE;
				if (calcularBCE_recepcionMaestro(TrDato, TrDato.datos,resultBCE)) {

					if (fichero && esCabecera >= 2) {

						flujoSalida.write(campoDatos, TrDato.Longitud);
						mostrarTramaDatoReceptor(TrDato, resultBCE);
						(maestro) ? enviarConfirmacionEsclavoSondeo(PuertoCOM,indice) : enviarConfirmacionEsclavoSeleccion(PuertoCOM,indice);

						contador++;
					} else {
						if (esCabecera == 1 && fichero) {
							esCabecera++;
							mostrarTramaDatoReceptor(TrDato, resultBCE);
							(maestro) ? enviarConfirmacionEsclavoSondeo(PuertoCOM,indice) : enviarConfirmacionEsclavoSeleccion(PuertoCOM,indice);

							contador++;
						} else if (fichero) {

							flujoSalida.open(campoDatos, ios::trunc);
							nomFichero = campoDatos;
							esCabecera++;
							mostrarTramaDatoReceptor(TrDato, resultBCE);

							(maestro) ? enviarConfirmacionEsclavoSondeo(PuertoCOM,indice) : enviarConfirmacionEsclavoSeleccion(PuertoCOM,indice);

							contador++;

						} else if (tamano == true) {
							mostrarTramaDatoReceptor(TrDato, resultBCE);
							printf("-----------------------------------------------------------------------------------TramaDatos\n");
							printf("\n");

							cout<< "El fichero recibido tiene un tamaño en bytes de ->"<< TrDato.datos << endl;
							printf("\n");
							fichero = true;

							(maestro) ? enviarConfirmacionEsclavoSondeo(PuertoCOM,indice) : enviarConfirmacionEsclavoSeleccion(PuertoCOM,indice);


						}

					}

				} else {
					mostrarTramaDatoReceptor(TrDato, resultBCE);

					(!maestro)? errorTramaSeleccion(PuertoCOM, indice) : errorTramaSondeo(PuertoCOM, indice);

				}

				break;
			}
		}
	}
}



bool comprobarSalir(TramaControl TrCtrl, char numtrama) {
	char tipoTrama = ' ';
	if (TrCtrl.Control != 02) {

		tipoTrama = TrCtrl.Control;

		switch (tipoTrama) {

		case 6:
			//comprobamos que el numero de trama es correcto y el tipo de comunicacion que se desea establecer.
			if (TrCtrl.NumTrama == numtrama && TrCtrl.Direccion == 'T' ) {
				mostrarTramaControl(TrCtrl, true);
				return true;
			}

			break;
		case 21:
			mostrarTramaControl(TrCtrl, true);
			return false;

		default:
			printf("No se reconoce el tipo de trama");
			return false;
			break;
		}

	}
	return false;
}

