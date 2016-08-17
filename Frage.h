// Frage.h

#ifndef _FRAGE_h
#define _FRAGE_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

class FrageClass
{
 protected:


 public:
	void init();
	//String frage = "";
	char frage[150] = "";
	//String antworten[3] = { "","","" };
	char antworten[3][150] = { "","","" };
	int frage_nr = 0;
	int zeit = 25;
	int richtige_antwort = 0;
	int punkte_richtig = 2;
	int punkte_falsch = -1;
	int punkte_ohne_antwort = 0;
	String get_html_frage() ;
	String get_html_antworten(byte richtige_antwort) ;
};

extern FrageClass Frage;

#endif

