//============================================================================
// Author      : Juan Antonio Silva Luján
//             : Alberto Mangut Bustamante
// GRUPO       : L6 - Miércoles de 10.00 a 11.30
// Version     : Sesion 2
// Description : Sesion 2 (FRC)
//============================================================================

#include "TramaDatos.h"
#include "Recibir.h"


/**
 * FUNCION: TeclaFuncionF2_enviarTramaDatos
 * PARAMETROS: HANDLE PuertoCOM, char cadena[], int indice,TramaControl &trCtrl,TramaDatos &trDatos,int &campo,bool &fichero,int &esCabecera,bool &tamano,string &nomFichero, bool &logF5, ofstream &flujoLOG, int &color
 * DESCRIPCION: Una vez que se ha pulsado la tecla F2, se envia una trama de datos, procedimiento:
 *
 * Se comprueba la longitud de la cadena :
 *		Si es MENOR de 254:
 *			Se le asignan los valores indicados
 *			Se envía la TRAMA
 *
 *		Si es MAYOR de 254:
 *			Declaramos dos vectores char del tamaño de la trama
 */
void TeclaFuncionF1_enviarTramaDatos(HANDLE PuertoCOM, char cadena[], int indice,TramaControl &trCtrl,TramaDatos &trDatos,int &campo,bool &fichero,int &esCabecera,bool &tamano,string &nomFichero, bool &logF5, ofstream &flujoLOG, int &color) {


	TramaDatos trmDatos;

	int NumCar = strlen(cadena), pos = 0, tamMensaje = 254;

		while (NumCar > 0) {

			if (NumCar >= tamMensaje) { //Si es mayor que 254 todavía queda cadena por trocear entonces entra.
				NumCar = NumCar - tamMensaje;
				int i=0;
				for (; i < tamMensaje; i++) {
					trmDatos.datos[i] = cadena[pos];
					pos++;
				}
				trmDatos.datos[i] = '\0';
				TeclaFuncionF2_envioPorDefecto(PuertoCOM, trmDatos, flujoLOG, logF5);

			} else { //Si es menor que 254 significa que estamos en la ultima cadena que se va a enviar.
				NumCar = NumCar - (strlen(cadena) - pos);//Esta comprobacion se usa para salir del bucle ya que siempre dará 0

				int i = 0;
				for (; pos < strlen(cadena); pos++) {
					trmDatos.datos[i] = cadena[pos];
					i++;
				}
				trmDatos.datos[i]= '\n';
				trmDatos.datos[i++] = '\0';
				TeclaFuncionF2_envioPorDefecto(PuertoCOM, trmDatos, flujoLOG, logF5);

				cadena[0] = '\0';
			}
			recibir(PuertoCOM,campo,trCtrl,trDatos,fichero,esCabecera,tamano,nomFichero, logF5,color);
		}



}

/**
 * FUNCION: calcularBCE_Envio
 * PARAMETROS: TramaDatos TrDato, char  campoDatos[]
 * DESCRIPCION: El método retorna la aplicacion de la puerta XOR en todas las posiciones del vector.
 */
void calcularBCE_Envio(TramaDatos &trmDatos, char cadena[]) {

		unsigned char resulBCE = 0;
		resulBCE=cadena[0];
		int contador = 1;

		while (contador < trmDatos.Longitud) {
			resulBCE = resulBCE ^ cadena[contador];
			contador++;
		}

		if (resulBCE == 0 || resulBCE == 255)	resulBCE = 1;

		trmDatos.BCE = resulBCE;

}

/**
 * FUNCION: TeclaFuncionF2_envioPorDefecto
 * PARAMETROS: HANDLE PuertoCOM, TramaDatos trmDatos, ofstream &flujoLOG, bool &logF5
 * DESCRIPCION: Tramita el envio con los datos por defectos: Sincronismo con valor 22, Direccion con valor T, Control con valor 02, NumTrama con valor 0
 * 	a continuacion, realiza el calculo del BCE para preparar el envio y por ultimo, invoca el metodo F2_TramaDatos_TramitarEnviosDefecto
 * 	para realizar el envio definitivo.
 */
void TeclaFuncionF2_envioPorDefecto(HANDLE PuertoCOM, TramaDatos trmDatos, ofstream &flujoLOG, bool &logF5){
	trmDatos.Sincronismo = 22;
	trmDatos.Direccion = 'T';
	trmDatos.Control = 02;
	trmDatos.NumTrama = '0';
	trmDatos.Longitud = (unsigned char) strlen(trmDatos.datos);
	calcularBCE_Envio(trmDatos, trmDatos.datos);
	F2_TramaDatos_TramitarEnviosDefecto(PuertoCOM, trmDatos);
}

/**
 * FUNCION: F2_TramaDatos_TramitarEnviosDefecto
 * PARAMETROS: HANDLE PuertoCOM, TramaDatos trmDatos
 * DESCRIPCION:  Una vez que se han incorporado los datos del registro del metodo TeclaFuncionF2_envioPorDefecto, se realiza el envio mediante EnviarCaracter y
 * EnviarCadena pertenecientes a PuertoSerie.
 */
void F2_TramaDatos_TramitarEnviosDefecto(HANDLE PuertoCOM, TramaDatos trmDatos) {

	EnviarCaracter(PuertoCOM, (char) trmDatos.Sincronismo);
	EnviarCaracter(PuertoCOM, (char) trmDatos.Direccion);
	EnviarCaracter(PuertoCOM, (char) trmDatos.Control);
	EnviarCaracter(PuertoCOM, (char) trmDatos.NumTrama);
	EnviarCaracter(PuertoCOM, (char) trmDatos.Longitud);
	EnviarCadena(PuertoCOM, trmDatos.datos, (int) strlen(trmDatos.datos));
	EnviarCaracter(PuertoCOM, (char) trmDatos.BCE);

}


