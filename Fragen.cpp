// 
// 
// 

#include "Fragen.h"
#include "Frage.h"

void FragenClass::init()
{
	
}

int FragenClass::Lese_Frage(char *line)
{
	bool ersteZeile = true;
	FrageClass *fr;

	//String str_tmp = "";

	int ci = -1;
	int si = -1;

	int result = -1;

	
	Serial.println(line);
		
	/*
	1.Frage eins ci=1 si=10 ci<si | ci!=-1 & si==-1
	1.Antwort ci=5, si=3 si<ci
	2.Antwort ci=5, si=3 si<ci
	3.Antwort ci=5, si=3 si<ci
	Zeit = 25; FrageNr 3; Punkte 2; Falsch -1; ohne Antwort 0  c=-1, s=3
	*/

	//ci = line.indexOf('.');
	//si = line.indexOf(' ');

	ci = indexOf(line, ".", 0);
	si = indexOf(line, " ", 0);

	// wenn Zeile mit Space beginnt, dann ermittle wieviele Space
	if (si == 0)	// Zeile beginnt mit Space?
	{
		int next_si = 0;
		while (si == next_si) // noch ein Space in Folge gefunden
		{
			//next_si = line.indexOf(' ', si + 1);	//
			next_si = indexOf(line , " ", si + 1);	//
			if (si + 1 == next_si)
				si = next_si;
		}
	}

	Serial.print("ci:"); Serial.println(ci);
	Serial.print("si:"); Serial.println(si);
	//Serial.println(anzahl_zeilen);

	// 1.Frage ci=1 si=10 ci<si
	if (ci != -1 && (ci < si || si == -1))
	{

		//String str_tmp = line.substring(0, ci); // Numerierung der Frage steht vor dem Komma
		char *str_tmp = substring(line, 0, ci); // Numerierung der Frage steht vor dem Komma

		//naechsteFrage.frage_nr = str_tmp.toInt();	// Wandle in Integer
		naechsteFrage.frage_nr = atoi(str_tmp); // Wandle in Integer
			
													//naechsteFrage.frage = (line.substring(ci + 1));	// Text der Frage steht nach dem Komma
		//String str_fr = line.substring(ci + 1);
		//str_fr.toCharArray(naechsteFrage.frage, 150);	// Text der Antwort steht nach dem Komma

		char * str_fr = substring(line,ci + 1, 0);
		strcpy(naechsteFrage.frage, str_fr);	// Text der Antwort steht nach dem Komma
			
		Serial.print("Frage"); Serial.print(naechsteFrage.frage_nr); Serial.println(naechsteFrage.frage);
		anzahl_fragen++;
	}

	// 1.Antwort ci=5, si=3 si<ci
	else if (ci != -1 && si < ci)
	{
		int antwort_nr = 0;
		//String str_tmp = line.substring(0, ci); // Numerierung der Antwort steht vor dem Komma
		//antwort_nr = str_tmp.toInt();	// Wandle in 
		char *str_tmp = substring(line, 0, ci);	// Numerierung der Antwort steht vor dem Komma
		antwort_nr = atoi(str_tmp);	// Wandle in Integer
		if (antwort_nr > 3 || antwort_nr == 0)
		{
			Serial.println("Ungültige Nummerierung der Antwort");
			Serial.println(line);
		}
		else
		{
			//Serial.print("ci:"); Serial.print(ci);
			//Serial.print("si:"); Serial.print(si);

			//String str_aw = line.substring(ci + 1);
			char *str_aw = substring(line, ci+1, 0);
			//str_aw.toCharArray(naechsteFrage.antworten[antwort_nr - 1], 150);	// Text der Antwort steht nach dem Komma
			strcpy(naechsteFrage.antworten[antwort_nr - 1], str_aw);	// Text der Antwort steht nach dem Komma

			//Serial.println(line); 
			//Serial.println(line.length());
			Serial.print("Antwort"); Serial.print(antwort_nr); Serial.println(naechsteFrage.antworten[antwort_nr - 1]);
			//Serial.println(str_fr);
			//Serial.println(antwort_nr);
			//Serial.println(str_fr.length());

		}
	}
	// Parameter c=-1, s=3
	else if (ci == -1 && si > -1) // Zeit = 25 ; FrageNr 3; Punkte 2; Falsch -1; ohne Antwort 0  
	{
		naechsteFrage.zeit = LesePunkte(line + si, "Zeit"); 
		naechsteFrage.richtige_antwort = LesePunkte(line + si, "Nr");
		naechsteFrage.punkte_richtig = LesePunkte(line + si, "Punkte");
		naechsteFrage.punkte_falsch = LesePunkte(line + si, "Falsch");
		naechsteFrage.punkte_ohne_antwort = LesePunkte(line + si, "ohne Antwort");

		result = naechsteFrage.frage_nr;
		Serial.print("naechsteFrage.zeit"); Serial.println(naechsteFrage.zeit);
	}


	return result;
}

//bool FragenClass::Set_IPAddress(String str_ip)
//{
//	bool result = true;
//
//	int commaIndex = 0;
//	uint8_t ip_byte[4];
//	for (int i = 0; i < 4; i++)
//	{
//		int nextCommaIndex = str_ip.indexOf('.', commaIndex);
//		int value = 0;
//		String str_tmp = "";
//		if (nextCommaIndex != -1)
//		{
//			str_tmp = str_ip.substring(commaIndex, nextCommaIndex);
//			commaIndex = nextCommaIndex + 1;
//		}
//		else
//		{
//			if (i < 2)
//			{
//				Serial.println("Fehler in IP Adresse - Anzahl Dezimalpunkte");
//				Serial.println(commaIndex);
//				Serial.println(nextCommaIndex);
//				result = false;
//			}
//			else
//				str_tmp = str_ip.substring(commaIndex);
//		}
//		value = str_tmp.toInt();
//		ip_byte[i] = (byte)value;
//		//Serial.println(ip_byte[i]);
//	}
//	if (result != false)
//	{
//		ip = IPAddress(ip_byte);
//		Serial.println(ip);
//	}
//
//	return result;
//
//}

int FragenClass::LesePunkte(char *str, char *search)
{
	int res = 0;
	int index = indexOf(str, search, 0) + strlen(search);
	int sp = indexOf(str, ";", index);
	//str_tmp = line.substring(index, sp); // Numerierung steht vor dem Strichpunkt
	char *str_tmp = substring(str, index, sp); // Numerierung der Frage steht vor dem Komma

												//int value = str_tmp.toInt();	// Wandle in Zahl um
	int value = atoi(str_tmp);	// Wandle in Integer

	//Serial.print(search); Serial.println(value);
	return value;

}

int FragenClass::indexOf(char *str, char *search, unsigned int fromIndex)
{
	int len = strlen(str);
	if (fromIndex >= len) return -1;
	const char *found = strstr(str + fromIndex, search);
	if (found == NULL) return -1;
	return found - str;

}

char *FragenClass::substring(char *str, unsigned int start, unsigned int end)
{
	
	char tmp[160];
	memset(tmp, 0, 160);
	unsigned int cnt = 0;
	int len = strlen(str);
	if (end == 0 || end > len)
		end = len;
	for (unsigned int i = start; i < end; i++)
	{
		tmp[cnt] = str[i];
		cnt++;
	}
	Serial.print("substring"); Serial.print(str); Serial.print(start); Serial.print(end); Serial.println(tmp);
	return tmp;
	 
	
}

//FragenClass Fragen;