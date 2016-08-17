// 
// 
// 

#include "Frage.h"

void FrageClass::init()
{


}

String FrageClass::get_html_frage()
{
	String res = "";
	res = "<h1>";
	res += "Frage Nr.: ";
	res += frage_nr;
	res += " ";
	res += frage;
	res += "</h1>";
	return res;

}

// gibt einen String mit den Antworten im html-Format zurück
// richtige_antwort: wenn der Parameter ungleich 0 ist, wird die richtige Antwort nicht hervorgehoben
String FrageClass::get_html_antworten(byte richtige_antwort)
{
	String res = "";
	res = "<h1>";
	for (int i = 0; i < 3; i++)
	{
		//if (antworten[i].length() > 0)
		//{
			if (richtige_antwort == (i + 1))
				res += "<mark>";// markiere die richtige Antwort

			res += i+1;
			res += ". ";
			res += antworten[i];
			if (richtige_antwort == (i + 1))
				res += "</mark>";// markiere die richtige Antwort
			res += "<br>";
			res += "<br>";
		//}
	}
	res += "</h1>";
	return res;
}


FrageClass Frage;

