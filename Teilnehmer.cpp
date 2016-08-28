// 
// 
// 

#include "Teilnehmer.h"

void TeilnehmerClass::init()
{
	memset(ListeTeilnehmer, 0, maxAnzahlTeilnehmer);

}

// prüft welcher Teilnehmer eine Taste gedrückt hat
// -> schreibe Taste des Teilnehmers in ListeTeilnehmer
// -> schreibt in das char array welcher Teilnehmernummer 
// welche bereits eine Taste gedrückt haben
void TeilnehmerClass::ermittleListeTeilnehmer(char *str)
{
	strcat(str, "Teilnehmer: ");	//char teiln[50] = "Teilnehmer: ";
	
	bool state = false;
	byte PinNr = 0;
	

	for (int t = 1; t <= maxAnzahlTeilnehmer; t++)
	{
		if (ListeTasten[t - 1] > 0)
		{
			if (ListeTeilnehmer[t - 1] == 0)
			{
				anzahl_Teilnehmer++;
				Serial.print("Anzahl Teilnehmer: "); Serial.print(anzahl_Teilnehmer);
			}
			ListeTeilnehmer[t - 1] = ListeTasten[t - 1];
			Serial.print("Teilnehmer: "); Serial.print(t);
			Serial.print("Taste: "); Serial.println(ListeTeilnehmer[t - 1]);
			ListeTasten[t - 1] = 0;
		}

		if (ListeTeilnehmer[t-1] == 0)
		{
			strcat(str, ".. ");	// teiln += ".. ";
		}
		else
		{
			
			if (t < 9)
				strcat(str, "0"); //teiln += "0";
			char num[2] = { " " };
			sprintf(num, "%d ", t);
			strcat(str, num);	// teiln += t;
			strcat(str, " ");	//teiln += " ";

		}
	}
}

// prüft welcher Teilnehmer eine Taste gedrückt hat
// -> schreibe Taste des Teilnehmers in ListeTeilnehmer
// -> schreibt in das char array welche Taste die Teilnehmernummer 
// gedrückt haben
void TeilnehmerClass::ermittleTasteTeilnehmer(char *str)
{
	strcat(str, "<br />Taste &nbsp &nbsp &nbsp &nbsp &nbsp : ");	//char teiln[50] = "Teilnehmer: ";

	bool state = false;
	byte PinNr = 0;


	for (int t = 1; t <= maxAnzahlTeilnehmer; t++)
	{
		if (ListeTasten[t - 1] > 0)
		{
			if (ListeTeilnehmer[t - 1] == 0)
			{
				anzahl_Teilnehmer++;
				Serial.print("Anzahl Teilnehmer: "); Serial.print(anzahl_Teilnehmer);
			}
			ListeTeilnehmer[t - 1] = ListeTasten[t - 1];
			Serial.print("Teilnehmer: "); Serial.print(t);
			Serial.print("Taste: "); Serial.println(ListeTeilnehmer[t - 1]);
			ListeTasten[t - 1] = 0;
		}

		if (ListeTeilnehmer[t - 1] == 0)
		{
			strcat(str, ".. ");	// keys += ".. ";
		}
		else
		{
			strcat(str, "0");	// keys += "0";
			char num[2] = { " " };
			sprintf(num, "%d ", ListeTeilnehmer[t - 1]);
			strcat(str, num);	// keys += ListeTeilnehmer[t-1];
			strcat(str, " ");	// keys += " ";
		}
	}
}


int TeilnehmerClass::ermittleAnzahlTeilnehmer()
{
	return anzahl_Teilnehmer;
}

byte TeilnehmerClass::ermittleClicks()
{
	byte result = clicks;
	clicks = 0;
	return result;
}

// -> lieste die Arduino-IO und schreibe die Taste des Teilnehmers in ListeTasten
bool TeilnehmerClass::leseTaster()
{
	bool state = false;
	byte PinNr = 0;

	for (byte t = 1; t <= maxAnzahlTeilnehmer; t++)
	{
		for (byte k = 1; k <= 3; k++)
		{
			PinNr = Taster_Belegung[t - 1][k - 1];
			state = digitalRead(PinNr);
			if (state)
			{
				if (ListeTasten[t - 1] == 0)
					ListeTasten[t - 1] = k;
			}
		}
	}
	state = digitalRead(Taster_UP);
	// Taste gedrückt
	if (state && clickmillis == 0)
	{
		Serial.println("Taste gedrückt");
		clickmillis = millis();
	}
	// Taste losgelassen
	if (!state && clickmillis != 0)
	{
		Serial.println("Taste losgelassen"); 
		clicks++;
		clickPressedMillis = millis() - clickmillis;
		clickmillis = 0;
	}
}

byte TeilnehmerClass::leseFunktionstaste()
{
	byte result = clickPressedMillis / 1000;
	if (clickPressedMillis > 0)
	{
		if (clickPressedMillis < 1000)
			result = 1;
		clickPressedMillis = 0;
		Serial.print("Taste fuer ");
		Serial.print(result);
		Serial.println(" sec gedrueckt: ");
	}
	

	
	return result;
}

//byte TeilnehmerClass::leseFunktionstaste()
//{
//	//Serial.println("leseFunktionstaste");
//	byte result = 0;
//
//	bool state = false;
//	byte PinNr = 0;
//
//	PinNr = Taster_Belegung[0][0];
//	state = digitalRead(PinNr);
//
//	// Taste gedrückt
//	if (ListeTasten[0] != 0 && clickmillis == 0)
//	{
//		Serial.println("Taste gedrückt");
//		clickmillis = millis();
//	}
//	// Taste losgelassen
//	if (!state && clickmillis != 0)
//	{
//		Serial.println("Taste losgelassen");
//		unsigned long pressedTime = millis() - clickmillis;
//
//		Serial.println(pressedTime);
//		Serial.println(clickmillis);
//		Serial.println(millis());
//
//		clickmillis = 0;
//		ListeTasten[0] = 0;
//
//		result = pressedTime / 1000;
//		if (result == 0)
//		{
//			result = 1;
//			Serial.println("Taste unter 1 sec lang gedrückt");
//		}
//		Serial.println(result);
//	}
//	return result;
//}

// prüft welcher Teilnehmer eine Taste gedrückt hat
// -> gibt die Anzahl der Teilnehmer zurück, welche keine Taste gedrückt haben.
byte TeilnehmerClass::alleTeilnehmerGedrueckt()
{
	byte result = anzahl_Teilnehmer;
	

	for (byte t = 1; t <= maxAnzahlTeilnehmer; t++)
	{

		if (ListeTeilnehmer[t - 1] > 0 && ListeTasten[t - 1] > 0)
		{
			result--;
		}
	}
	return result;
}
void TeilnehmerClass::loescheTasten(void)
{
	memset(ListeTasten, 0, maxAnzahlTeilnehmer);
}

void TeilnehmerClass::loescheTeilnehmer(void)
{
	memset(ListeTeilnehmer, 0, maxAnzahlTeilnehmer);
	anzahl_Teilnehmer = 0;
}

void TeilnehmerClass::setzteStatus(byte TeilnehmerNr)
{
	Teilnehmer_Nr = TeilnehmerNr;
	if (ListeTeilnehmer[TeilnehmerNr - 1] != 0)
		aktive = true;
	//memset(ListeTasten, 0, maxAnzahlTeilnehmer);
}

void TeilnehmerClass::loeschePunkte(void)
{
	//memset(ListeTasten, 0, maxAnzahlTeilnehmer);
	punkte = 0;
	for (byte f = 0; f < maxAnzahlFragen; f++)
	{
		Punkte p = punkte_Liste[f];
		p.gedrueckteTaste = 0;
		p.Punkte = 0;
	}
	summePunkte = 0;

}

int TeilnehmerClass::SummePunkte(void)
{
	return summePunkte;

}



void TeilnehmerClass::auswertungPunkte(FragenClass fk)
{
	int fr_nr = fk.naechsteFrage.frage_nr;

	Punkte p = punkte_Liste[fr_nr];

	// Teilnehmer aktiviert?
	if (ListeTeilnehmer[Teilnehmer_Nr-1])
	{
		// hat Teilnehmer Taste gedrückt
		if (ListeTasten[Teilnehmer_Nr-1] > 0)
		{
			// richtige Taste gedrückt?
			if (ListeTasten[Teilnehmer_Nr-1] == fk.naechsteFrage.richtige_antwort)
			{
				punkte_Liste[fr_nr].Punkte = fk.naechsteFrage.punkte_richtig;
			}
			else
			{
				punkte_Liste[fr_nr].Punkte = fk.naechsteFrage.punkte_falsch;
			}
		}
		// Taste nicht gedrückt
		else
		{
			punkte_Liste[fr_nr].Punkte = fk.naechsteFrage.punkte_ohne_antwort;
		}
	}
	punkte += punkte_Liste[fr_nr].Punkte;
	if (Teilnehmer_Nr == 1)
		summePunkte += fk.naechsteFrage.punkte_richtig;
}


String TeilnehmerClass::ermittleTasten(void)
{
	String res = "";
	for (byte t = 1; t <= maxAnzahlTeilnehmer; t++)
	{
		if (ListeTasten[t - 1] > 0)
		{
			res += String(ListeTasten[t - 1]);
			res += " ";
		}
		else
			res += ".. ";
	}
	return res;
}

String TeilnehmerClass::ermittleListeTeilnehmerGedrueckt(void)
{
	String res = "";
	for (byte t = 1; t <= maxAnzahlTeilnehmer; t++)
	{
		if (ListeTasten[t - 1] > 0)
		{
			res += t;
			res += " ";
		}
		else
			res += ".. ";
	}
	return res;
}



TeilnehmerClass Teilnehmer;

