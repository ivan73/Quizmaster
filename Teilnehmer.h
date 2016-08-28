// Teilnehmer.h

#ifndef _TEILNEHMER_h
#define _TEILNEHMER_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

#include "Fragen.h"

#define maxAnzahlTeilnehmer 12

#define Taster_UP 69

const byte Taster_Belegung[maxAnzahlTeilnehmer][3] = {
	{22,23,24},	//1
	{25,26,27},	//2
	{28,29,30},	//3
	{31,32,33},	//4
	{34,35,36},	//5
	{37,38,39},	//6
	{40,41,42},	//7
	{43,44,45},	//8
	{46,47,48},	//9
	{62,63,64},	//10
	{65,66,67},	//11
	{14,15,16}	//12
};



static byte ListeTeilnehmer[maxAnzahlTeilnehmer];
static byte anzahl_Teilnehmer = 0;
static byte ListeTasten[maxAnzahlTeilnehmer];
static int summePunkte = 0;
//static unsigned long startZeit = 0;
static unsigned long clickmillis = 0;
static bool clickstatus  = false;
static byte clicks = 0;
static unsigned long clickPressedMillis = 0;

struct Taste_ID
{
	byte Teilnehmer_Nr = -1;
	byte Taste_Nr = -1;
};

struct Punkte
{
	//byte  Frage_Nr = -1;
	//byte Zeit = -1;
	byte gedrueckteTaste = -1;
	int Punkte = 0;
};

class TeilnehmerClass
{
 protected:


 public:
	void init();
	
	//static Taste_ID ermittleTeilnehmer_aus_IO_Pin(byte IO_Pin);
	
	byte Teilnehmer_Nr = 0;
	bool aktive = false;
	Punkte punkte_Liste[maxAnzahlFragen];
	int punkte = 0;
	
	void ermittleListeTeilnehmer(char *str);
	void ermittleTasteTeilnehmer(char *str);
	bool leseTaster(void);
	String ermittleTasten();
	byte alleTeilnehmerGedrueckt(void);
	void loescheTasten(void);
	void loescheTeilnehmer(void);
	void auswertungPunkte(FragenClass fk);
	void loeschePunkte(void);
	int ermittleAnzahlTeilnehmer();
	void setzteStatus(byte TeilnehmerNr);
	String ermittleListeTeilnehmerGedrueckt(void);
	byte leseFunktionstaste(void);
	byte leseFunktionsTaste(void);
	byte ermittleClicks(void);
	int SummePunkte(void);
};

extern TeilnehmerClass Teilnehmer;

#endif

