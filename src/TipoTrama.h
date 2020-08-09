//============================================================================
// Author      : Juan Antonio Silva Luj�n
//             : Alberto Mangut Bustamante
// GRUPO       : L6 - Mi�rcoles de 10.00 a 11.30
// Version     : Sesion 2
// Description : Sesion 2 (FRC)
//============================================================================

#ifndef ARRAYTC_H_
#define ARRAYTC_H_

struct TramaControl{
	unsigned char Sincronismo;   //SINCRONISMO =  SYN = 22
	unsigned char Direccion;     //DIRECCI�N   =  (En principio fijo a 'T')
	unsigned char Control;       //CONTROL     =  05 ENQ | 04 EOT | 06 ACK | 21 NACK
	unsigned char NumTrama;      //N� TRAMA    =  (En principio fijo a '0')
};


struct TramaDatos{
	unsigned char Sincronismo;  //SINCRONISMO =  SYN = 22
	unsigned char Direccion;    //DIRECCI�N   =  (En principio fijo a 'T')
	unsigned char Control;		//CONTROL     =  STX = 02
	unsigned char NumTrama;		//N� TRAMA    =  (En principio fijo a '0')
	unsigned char Longitud;		//LONGITUD    =  Longitud del campo de datos
	unsigned char BCE;          //Bloque Control Errores entre (1 y 254)
    char datos[255];
};

#endif /* FUNCIONES_H_ */

