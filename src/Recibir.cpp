#include <stdio.h>
#include <conio.h>
#include <iostream>
#include <string.h>
#include <windows.h>
#include <fstream>
#include <wchar.h>
#include <locale.h>
#include "TramaDatos.h"
#include "TramaControl.h"
#include "Recibir.h"

using namespace std;
char autores[30]="";


/**
 * FUNCION: recibir
 * PARAMETROS: HANDLE PuertoCOM, int &campo, TramaControl &TrCtrl, TramaDatos &TrDato,bool &fichero,int &esCabecera,bool &tamano,string &nomFichero, bool &logF5, int &color
 * DESCRIPCION: El metodo realiza todas las funciones correspondientes a la recepcion de informacion (terminal receptora)
 */
void recibir(HANDLE PuertoCOM, int &campo, TramaControl &TrCtrl, TramaDatos &TrDato,bool &fichero,int &esCabecera,bool &tamano,string &nomFichero, bool &logF5, int &color) {

	char tipoTrama = ' ';
	char caracterRecibido = RecibirCaracter(PuertoCOM);
	char campoDatos [255];

	//Flujo de escritura para la transferencia de informacion del fichero
	ofstream flujoSalida;

	//Flujo de escritura para el registro log.txt, solo se lleva a cabo cuando la bandera logF5 = TRUE
	ofstream flujoLOG("log.txt", ios::app);

	if (caracterRecibido != 0) {

		//Cuando campo=1 en caso de que el caracter recibido sea igual a 22 significara que estamos
		//recibiendo una trama. En caso contrario se imprimirá por pantalla el caracter recibido.
		//Si se trata de una trama almacenaremos los caracteres recibidos en la trama que es recibida por referencia.
		//Cuando campo=4 significa que hemos recibido toda la trama. Seguidamente estudiamos si se trata de
		//una trama de control o de datos. En caso de tratarse del primer tipo mostramos por pantalla el tipo de trama recibida.
		switch (campo) {
		case 1://_________________________________________________Caso para SINCRONISMO

			if (caracterRecibido == 22) {
				//Inicializar la TRAMA
				TrCtrl.Sincronismo = caracterRecibido;
				TrDato.Sincronismo = caracterRecibido;
				campo++;
			}else {

				if(caracterRecibido == '{'){//Si '#' se está enviando un FICHERO (en proceso)
					fichero = true;
				}

				if(caracterRecibido == '}'){//Si '@' YA se recibido el FICHERO (terminado)
					HANDLE Pantalla = GetStdHandle(STD_OUTPUT_HANDLE);
					SetConsoleTextAttribute(Pantalla, color);
					fichero = false;
					tamano=true;
					flujoSalida.flush();
					flujoSalida.close();
					nomFichero="";
					esCabecera=0;
					printf("\n");
					printf("RECEPTOR: Fichero RECIBIDO CORRECTAMENTE\n");
					if(logF5)	flujoLOG<<"RECEPTOR: Fichero RECIBIDO CORRECTAMENTE"<<endl;
				}

			}
			break;

		case 2://_________________________________________________Caso para DIRECCIÓN

			TrCtrl.Direccion = (unsigned char)caracterRecibido;
			TrDato.Direccion = (unsigned char)caracterRecibido;
			campo++;
			break;

		case 3://_________________________________________________Caso para CONTROL

			TrCtrl.Control = (unsigned char)caracterRecibido;
			TrDato.Control = (unsigned char)caracterRecibido;
			campo++;
			break;

		case 4:	//_________________________________________________Caso para NÚMERO DE TRAMA

			TrCtrl.NumTrama = (unsigned char) caracterRecibido;
			TrDato.NumTrama = (unsigned char) caracterRecibido;

			//Si caracter de control != 2 NO es trama de dtos entonces se trata de una trama de control
			if (TrCtrl.Control != 02) {
				campo = 1;
				tipoTrama = TrCtrl.Control;
				HANDLE Pantalla = GetStdHandle(STD_OUTPUT_HANDLE);
				//Imprimo recepción TrCtrl - Imprimo en negro con fondo verde
				SetConsoleTextAttribute(Pantalla, 0 + 2 * 16);

				switch (tipoTrama) {

				case 4:
					printf("\n Se ha recibido una trama EOT \n");
					if(logF5)	flujoLOG<<"Se ha recibido una trama EOT"<<endl;
					break;

				case 5:
					printf("\n Se ha recibido una trama ENQ \n");
					if(logF5)	flujoLOG<<"Se ha recibido una trama ENQ"<<endl;
					break;

				case 6:
					printf("\n Se ha recibido una trama ACK \n");
					if(logF5)	flujoLOG<<"Se ha recibido una trama ACK"<<endl;
					break;

				case 21:
					printf("\n Se ha recibido una trama NACK \n");
					if(logF5)	flujoLOG<<"Se ha recibido una trama NACK"<<endl;
					break;

				default:
					printf("\n No se reconoce el tipo de trama");
					break;
				}
				SetConsoleTextAttribute(Pantalla, 15 + 0 * 16); //Dejo de recibir - Volver color default

			} else { //else -> se trata de una TRAMA DE DATOS
				campo++;
			}
			break;

		case 5: //_________________________________________________Caso para LONGITUD
			TrDato.Longitud = (unsigned char) caracterRecibido;
			campo++;
			/* no break */

		case 6: //_________________________________________________Caso para DATOS
			campoDatos[0] = '\0';
			TrDato.datos[0] = '\0';
			RecibirCadena(PuertoCOM, campoDatos, TrDato.Longitud);

			//Se deben recibir los datos completos de una sola vez
			campoDatos[TrDato.Longitud] = '\0';
			strcpy(TrDato.datos, campoDatos);
			campo++;

			break;

		case 7: //_________________________________________________Caso para BLOQUE CONTROL ERRORES (BCE)

			TrDato.BCE = (unsigned char) caracterRecibido;
			campo = 1;
			if (calcularBCE_recepcion(TrDato, TrDato.datos)) {


				if (fichero && esCabecera >= 3) {//nombre fichero
					flujoSalida.open(nomFichero, ios::app);

					flujoSalida.write(TrDato.datos, TrDato.Longitud);
				} else {
					if (esCabecera == 0 && fichero) {//Autores
						strcpy(autores,TrDato.datos);
						esCabecera++;
					} else if (esCabecera == 1 &&fichero) {
						int i;
						sscanf(TrDato.datos, "%d", &i);
						color= i;
						HANDLE Pantalla = GetStdHandle(STD_OUTPUT_HANDLE);
						SetConsoleTextAttribute(Pantalla, color);
						cout << "Recibiendo Fichero por Autor: " << autores<< endl;
						if(logF5)	flujoLOG<<"Recibiendo Fichero por Autor: "<<autores<<endl;
						esCabecera++;
					}else if(esCabecera == 2 &&fichero){
						flujoSalida.open(TrDato.datos, ios::trunc);
						nomFichero = TrDato.datos;
						esCabecera++;
					} else if (tamano == true) {
						HANDLE Pantalla = GetStdHandle(STD_OUTPUT_HANDLE);
						SetConsoleTextAttribute(Pantalla, color);

						printf("\n");

						cout<< "El fichero recibido tiene un tamano en bytes de ->"<< TrDato.datos << endl;
						if(logF5)	flujoLOG<< "El fichero recibido tiene un tamano en bytes de ->"<< TrDato.datos << endl;
						tamano = false;

					} else {
						HANDLE Pantalla = GetStdHandle(STD_OUTPUT_HANDLE);
						SetConsoleTextAttribute (Pantalla,  14 + 0*16);
						SetConsoleTextAttribute(Pantalla, 0 + 14 * 16);
						cout << TrDato.datos;
						if(logF5)	flujoLOG<<TrDato.datos;
					}
				}
			} else {
				printf("[!] ERROR AL CALCULAR BCE");
			}

			break;

		case 27:
			//_________________________________________________Caso para tecla ESCAPE [ESC] - CANCELAR

			printf("[!] Detectada tecla [ESC], cancelando acción...\n");
			break;

		default:
			//_________________________________________________Cualquier otro caso

			printf("[!] Error en la trama recibida.");
			campo = 1;
			break;

		}
	}

	flujoLOG.close();
}

/**
 * FUNCION: calcularBCE_recepcion
 * PARAMETROS: TramaDatos TrDato, char  campoDatos[]
 * DESCRIPCION: El método retorna la aplicacion de la puerta XOR en todas las posiciones del vector campoDatos.
 */
bool calcularBCE_recepcion (TramaDatos TrDato, char  campoDatos[] ){
	bool resultado=false;
	unsigned char resulBCE =0;
	resulBCE= campoDatos[0];
	int contador = 1;

	while(contador < TrDato.Longitud){
		resulBCE = resulBCE ^ campoDatos[contador];
		contador++;
	}

	if(resulBCE==0 || resulBCE==255)	resulBCE=1;

	if(resulBCE == TrDato.BCE)	resultado=true;

	return resultado;
}

