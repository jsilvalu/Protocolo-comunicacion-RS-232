//============================================================================
// Author      : Juan Antonio Silva Luján
//             : Alberto Mangut Bustamante
// GRUPO       : L6 - Miércoles de 10.00 a 11.30
// Version     : Sesion 5
// Description : Sesion 5 (Protocolo Maestro-Esclavo)
//============================================================================


#include "FicheroMaestro.h"

/**
 * F3_enviarFicheroMaestro
 * Descripción:
 * Realiza un envio de un fichero llamado "EProtoc.txt" situado en la raiz del proyecto.
 *
 */
void F3_enviarFicheroMaestro(HANDLE PuertoCOM, bool maestro, bool &salir) {

	//Declaración y apertura del FICHERO Fenvio.txt

	ifstream flujo("EProtoc.txt");
	bool NoError = true, finFichero = false;
	int contadorLineas = 0, contador = 0, contadorErrores = 0, contCaracteres =0;
	char lineasCabecerasF[20] = { "" }, cadenaCuerpoFichero[255] = { "" }, indice = '0', primerCaracter = '0', car = '0';


	if (flujo.is_open()) {

		EnviarCaracter(PuertoCOM, '{'); //Enviamos el caracter { para comenzar la retransmision.
		finFichero = flujo.eof();
		while (!finFichero && NoError == true && salir == false) {

			if (kbhit()) {

				car = getch();

				if (car == '\0') {
					car = getch();
					if (car == 63)	contadorErrores++;
				}

				if (car == 27) 	salir = true;

			}

			indice = (contador % 2 == 0) ? '0' :'1' ;

			if (contadorLineas < 2) { //Envío de trama de las 2 primeras líneas de cabecera (Nombre de fichero y Autor)
									  //Linea de cabecera para nombre del fichero (1º Línea)
									  //Linea de cabecera para nombre del autor   (2º Línea)
				flujo.getline(lineasCabecerasF, 20);
				finFichero = flujo.eof();
				TramaDatos TrmDatos;
				strcpy(TrmDatos.datos, lineasCabecerasF);
				TrmDatos.NumTrama = indice;
				TrmDatos.Longitud = (unsigned char) strlen(TrmDatos.datos);
				calcularBCE_Envio(TrmDatos, TrmDatos.datos);

				if (contadorErrores > 0) {
					primerCaracter = TrmDatos.datos[0];
					TrmDatos.datos[0] = 'ç';
					contadorErrores--;
				}

				if (maestro == true) {
					envioMaestroSeleccion(PuertoCOM, TrmDatos);
					NoError = recibirConfirmacionMaestroSeleccion(PuertoCOM,indice);
				} else {
					envioMaestroSondeo(PuertoCOM, TrmDatos);
					NoError = recibirConfirmacionMaestroSondeo(PuertoCOM,indice);
				}

				if (!NoError) {
					TrmDatos.datos[0] = primerCaracter;
					//calcularBCE_Envio(TrmDatos, TrmDatos.datos);
					if (maestro == true) {
						envioMaestroSeleccion(PuertoCOM, TrmDatos);
						NoError = recibirConfirmacionMaestroSeleccion(PuertoCOM,indice);
					} else {
						envioMaestroSondeo(PuertoCOM, TrmDatos);
						NoError = recibirConfirmacionMaestroSondeo(PuertoCOM,indice);
					}
				}
				contador++;

				//comprobamos que llega correctamente la confirmacion

			} else if (contadorLineas >= 2) {

				cadenaCuerpoFichero[0] = '\0';
				flujo.read(cadenaCuerpoFichero, 254); //Lectura cada 254 caracteres
				cadenaCuerpoFichero[flujo.gcount()] = '\0';
				finFichero = flujo.eof();

				if (strlen(cadenaCuerpoFichero) == 0)	break;

				contCaracteres = contCaracteres + strlen(cadenaCuerpoFichero);
				cadenaCuerpoFichero[strlen(cadenaCuerpoFichero)] = '\0';

				//Envio de la TRAMA
				contador++;

				if (contadorErrores > 0) {
					TeclaFuncionF1_enviarTramaDatos(PuertoCOM,cadenaCuerpoFichero, strlen(cadenaCuerpoFichero),true, maestro, indice, true, primerCaracter);
					contadorErrores--;
				} else
					TeclaFuncionF1_enviarTramaDatos(PuertoCOM,cadenaCuerpoFichero, strlen(cadenaCuerpoFichero),true, maestro, indice, false, primerCaracter);

				NoError = (maestro) ? recibirConfirmacionMaestroSeleccion(PuertoCOM,indice) : recibirConfirmacionMaestroSondeo(PuertoCOM,indice);

				if (!NoError) {
					TeclaFuncionF1_enviarTramaDatos(PuertoCOM,cadenaCuerpoFichero, strlen(cadenaCuerpoFichero),true, maestro, indice, false, primerCaracter);

					NoError = (maestro) ? recibirConfirmacionMaestroSeleccion(PuertoCOM,indice) : recibirConfirmacionMaestroSondeo(PuertoCOM,indice);

				}

			} else {
				printf("ERROR. Cuerpo del FICHERO vacío.\n");
			}

			contadorLineas++;

		}

	} else 	printf("ERROR al abrir Fenvio.txt.");

	//Fichero enviado, enviar caracter @
	EnviarCaracter(PuertoCOM, '@');

	indice = (contador % 2 == 0)  ? '0' :'1' ;

	cadenaCuerpoFichero[0] = '\0';
	itoa(contCaracteres, cadenaCuerpoFichero, 10);
	cadenaCuerpoFichero[strlen(cadenaCuerpoFichero)] = '\0';
	//TeclaFuncionF2_enviarTramaDatos(PuertoCOM, cadenaCuerpoFichero,strlen(cadenaCuerpoFichero), true, maestro, indice, false, indice); //indice no se usa para nada, es sólo para cumplir con los parámetros que se deben enviar.
	NoError = (maestro) ? recibirConfirmacionMaestroSeleccion(PuertoCOM,indice) : recibirConfirmacionMaestroSondeo(PuertoCOM,indice);

	//Cierre del flujo
	flujo.close();

}
