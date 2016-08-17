// Fragen.h
#include <SPI.h>
#include <SD.h>
#include <Ethernet.h>
#include "Frage.h"





#ifndef _FRAGEN_h
#define _FRAGEN_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

#define maxAnzahlFragen 30

// Ethernet controller SPI CS pin
#define W5200_CS  10
// SD card SPI CS pin
#define SDCARD_CS 4

class FragenClass
{
 
private:
	//String * line_buffer;
	//Frage fr[];
	// Ethernet controller SPI CS pin

 protected:


 public:
	IPAddress ip;
	//FrageClass Fragen_Liste[maxAnzahlFragen] = { FrageClass() };
	FrageClass naechsteFrage;
	File fkFile;
	
	
	void init();
	bool Oeffne_Datei(String Name);
	void Schliesse_Datei();
	int Lese_Frage();
	bool Set_IPAddress(String str_ip);
	int anzahl_fragen = 0;
	
};


extern FragenClass Fragen;


#endif

