/*-----------------------------------------------------*
 * Protocole de communication avec l'uart               *
 *-----------------------------------------------------*/
 
#ifndef _MESSAGE_H
#define _MESSAGE_H

/* Les différents code ci dessous permettent de différencier les commandes
 envoyé/recu au pSoC.
 Au depart ces codes était sur 3 bits, ils sont maintenent sur 4 pour avoir
 plus de commandes.
 Sauf CODE_DELTA qui est restait sur 3, car il n'y a pas la place dans la trame
 donc le code de delta est en fait soit 0b0100 soit 0b0101 (0x4 ou 0x5)*/

#define CODE_DELTA 0b010
#define CODE_ALPHA 0b1010
#define CODE_ALPHA_ABS 0b1011
#define CODE_STOP  0b0000
#define CODE_GET_XY 0b1100
#define CODE_GET_ALPHA 0b1101
#define CODE_SET_XY 0b0011
#define CODE_SET_ALPHA 0b0010
#define CODE_PRES_GROS 0b0110
#define CODE_PRES_PRECIS 0b0111
#define CODE_FREE_WHEEL 0b1001

/* code particulier qui sert à modifier les coeff PID, certanement obsolète*/
#define CODE_CONFIG 0b1110

#define DONE 0x10 //correspond au CODE 0b0001
#define DERAPAGE 0x80 //correspond au CODE 0b1000


#define CODE_P 0b00
#define CODE_I 0b01
#define CODE_D 0b10

#define CODE_CONFIG_DELTA 0
#define CODE_CONFIG_ALPHA 1


struct TrDelta
{
	unsigned short 
		delta:12, //poids faible
		signe:1, // 1 négatif, 0 positif
		codet:3; //poids fort
};
typedef struct TrDelta TrDelta;

struct TrAlpha
{
	unsigned short
		alpha:9, //poids faible
		signe:1,
		stub:2,
		codet:4; //poids fort
};
typedef struct TrAlpha TrAlpha;

/* trame genérique 16bits, utilile pour switché sur le code*/
struct TrGenShort
{
	unsigned short
		stub:12,
		codet:4; //poids fort
};
typedef struct TrGenShort TrGenShort;

struct TrShort
{
	unsigned short
		oX:8, //poids faible
		Xo:8; //poids fort
};
typedef struct TrShort TrShort;

union TrameAsser
{
	TrDelta trDelta;
	TrAlpha trAlpha;
	TrShort trShort;
	TrGenShort trGenShort;
	unsigned short us;
};
typedef union TrameAsser TrameAsser;

struct Tr4Char
{
	unsigned long  
		oooX:8, //poids faible
		ooXo:8,
		oXoo:8,
		Xooo:8; //poids fort
};
typedef struct Tr4Char Tr4Char;

struct TrConf
{
	unsigned long  
		dec:8, //poids faible
		ent:16,
		stub:1,
		ad2:1,
		ad1:1,
		pid:2,
		config:3; //poids fort
};
typedef struct TrConf TrConf;



struct TrPos
{
	unsigned long
		y:12,
		x:12,
		stub:4,
		codet:4;
};
typedef struct TrPos TrPos;

union TrameConfig
{
	Tr4Char tr4Char;
	TrConf conf;
	TrPos tpos;
	unsigned long l;
};
typedef union TrameConfig TrameConfig;
typedef union TrameConfig Trame32;

///////////////////////////////////////////////////////























void message_processing(unsigned char inMessage);	// Appelée à l'interruption de l'UART

#endif
