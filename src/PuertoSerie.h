//============================================================================
// Author      : Juan Antonio Silva Luján
//             : Alberto Mangut Bustamante
// GRUPO       : L6 - Miércoles de 10.00 a 11.30
// Version     : Sesion 1
// Description : Sesion 1 (FRC)
//============================================================================

//FUNDAMENTOS DE REDES Y COMUNICACIONES - CURSO 2019/20
//LIBRERÍA QUE CONTROLA EL PUERTO SERIE

#ifndef __PUERTOSERIE_H_
#define __PUERTOSERIE_H_
#include <windows.h>
using namespace std;


//Manejar el puerto
HANDLE AbrirPuerto(LPCSTR NombrePuerto, DWORD Velocidad, BYTE NumBitsXByte, BYTE Paridad, BYTE BitsParada);
void CerrarPuerto(HANDLE &PuertoCOM);

//Enviar y recibir
BOOL EnviarCaracter(HANDLE &PuertoCOM, char CarAEnviar);
BOOL EnviarCadena(HANDLE &PuertoCOM, const void *CadenaAEnviar, int longitud);
char RecibirCaracter(HANDLE &PuertoCOM);
BOOL RecibirCadena(HANDLE &PuertoCOM, char *CadenaRecibida, int NumCaracteres);

//Circuitos de control
void SetDTR(HANDLE &PuertoCOM, int Estado);
void SetRTS(HANDLE &PuertoCOM, int Estado);
BOOL GetCTS(HANDLE &PuertoCOM);
BOOL GetDCD(HANDLE &PuertoCOM);
BOOL GetDSR(HANDLE &PuertoCOM);
BOOL GetRI(HANDLE &PuertoCOM);

//Buffers
void ComprobarTamanoBuffers(HANDLE &PuertoCOM);
int CambiarTamanoBuffers(HANDLE &PuertoCOM, int TamBufferEntrada, int TamBufferSalida);
int VaciarBufferEntrada(HANDLE &PuertoCOM);
int VaciarBufferSalida(HANDLE &PuertoCOM);
int VaciarBuffers(HANDLE &PuertoCOM);

#endif
