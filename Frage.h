// Frage.h

#ifndef _FRAGE_h
#define _FRAGE_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

#include <stdio.h>
//#include <string.h>


#define maxZeichenJeZeile 150

class FrageClass
{
 protected:


 public:
	void init();
	//String frage = "";
	char frage[maxZeichenJeZeile] = "";
	//String antworten[3] = { "","","" };
	char antworten[3][maxZeichenJeZeile] = { "","","" };
	int frage_nr = 0;
	int zeit = 25;
	int richtige_antwort = 0;
	int punkte_richtig = 2;
	int punkte_falsch = -1;
	int punkte_ohne_antwort = 0;
	void get_html_frage(char * str) ;
	void get_html_antworten(char richtige_antwort, char * str) ;
};

extern FrageClass Frage;

#endif

