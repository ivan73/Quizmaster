// 
// 
// 

#include "Frage.h"

void FrageClass::init()
{


}

void FrageClass::get_html_frage(char *str)
{
	
	str[0] = 0;
	strcat(str, "<h1>Frage Nr.: ");	//res = "<h1>";
	char num[5] = { " " };
	sprintf(num, "%d ", frage_nr);
	strcat(str, num); //res += frage_nr; res += " ";
	
	
	strcat(str, frage);	// res += frage;
	strcat(str, "</h1>");		// res += "</h1>";
	////return res;

}

// gibt einen String mit den Antworten im html-Format zurück
// richtige_antwort: wenn der Parameter ungleich 0 ist, wird die richtige Antwort nicht hervorgehoben
void FrageClass::get_html_antworten(char richtige_antwort, char *str)
{
	
	//String res = "";
	memset(str, 0, maxZeichenJeZeile);
	strcat(str, "<h1>");
	//res = "<h1>";
	for (int i = 0; i < 3; i++)
	{
		//if (antworten[i].length() > 0)
		//{
			if (richtige_antwort == (i + 1))
				strcat(str, "<mark>"); //res += "<mark>";// markiere die richtige Antwort
			char num[5] = { " " };
			sprintf(num, "%d.", i + 1);
			strcat(str, num); //res += i+1; res += ". ";
			strcat(str, antworten[i]);	//res += antworten[i];
			if (richtige_antwort == (i + 1))
				strcat(str, "</mark>");  //res += "</mark>";// markiere die richtige Antwort
			strcat(str, "<br><br>");	// res += "<br>"; res += "<br>";
		//}
	}
	strcat(str, "</h1>");	// res += "</h1>";
	
	//return res;
}


//FrageClass Frage;

