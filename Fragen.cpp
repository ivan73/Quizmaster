// 
// 
// 

#include "Fragen.h"
#include "Frage.h"

void FragenClass::init()
{
	
}


//FragenClass Fragen;


void FragenClass::Schliesse_Datei()
{
	digitalWrite(SDCARD_CS, LOW);	// SPI SD Karte einschalten
	digitalWrite(W5200_CS, HIGH);

	if (fkFile)
	{
		fkFile.close();
	}
	else
	{
		String s = "Fehler beim Schließen der Datei: ";
		s += fkFile.name();
		Serial.println(s);
	}
	digitalWrite(SDCARD_CS, HIGH);	// SPI SD Karte ausschalten (Ethernet ein)
	digitalWrite(W5200_CS, LOW);
}

bool FragenClass::Oeffne_Datei(String Name)
{
	bool result = false;
	
	Serial.print("öffnen und lesen der Datei: ");
	Serial.println(Name);
	digitalWrite(SDCARD_CS, LOW);	// SPI SD Karte einschalten
	digitalWrite(W5200_CS, HIGH);

	if (!SD.begin(SDCARD_CS))	// Initializes the SD library and card
	{
		Serial.println("SD Initialisierung fehlgeschlagen!");
	}
	Serial.println("SD Initialisierung erfolgreich!");

	fkFile = SD.open(Name);
	if (fkFile)
	{
		Serial.println(Name);
		result = true;
	}
	else
	{
		// if the file didn't open, print an error:
		String s = "Fehler beim Öffnen der Datei: ";
		s += Name;
		Serial.println(s);
	}
	digitalWrite(SDCARD_CS, HIGH);	// SPI SD Karte ausschalten (Ethernet ein)
	digitalWrite(W5200_CS, LOW);
	return result;
}

int FragenClass::Lese_Frage()
{
	int anzahl_zeilen = 0;
	bool ersteZeile = true;
	FrageClass *fr;

	String str_tmp = "";

	int ci = -1;
	int si = -1;

	int result = -1;

	digitalWrite(SDCARD_CS, LOW);	// SPI SD Karte einschalten
	digitalWrite(W5200_CS, HIGH);

	char c_line[160];
	

	memset(c_line, 32, 159);
	c_line[159] = '\0';

	String line = String(c_line);

	while (result == -1)
	{
		line = fkFile.readStringUntil('\n');

		if (line.length() == 0)
		{
			Serial.println("keine Zeichen in der Zeile gelesen");
			digitalWrite(SDCARD_CS, LOW);	// SPI SD Karte einschalten
			digitalWrite(W5200_CS, HIGH);
			line = fkFile.readStringUntil('\n');
			Serial.println(line);

			return result;
			break;
		}

		if (line.length() > 159)
		{
			Serial.println("Zeile zu lange");
			Serial.println(line);
			line.remove(159);
		}

						//fkFile.readBytes()
		//char line[160] = fkFile.readStringUntil('\n');
		Serial.println(line);
		if (ersteZeile == true && line.indexOf("IP") != -1)	// in erster Zeile steht die IP-Adresse
		{
			int index = line.indexOf(":");
			if (index != -1)
			{
				String ip_str = line.substring(index + 1);
				Set_IPAddress(ip_str);	// ermittle IP-Adresse aus dem String

				Serial.println("IP-Adresse aus Datei:");
				Serial.println(ip);
			}
		}
		else
			ersteZeile = false;
		/*
		1.Frage eins ci=1 si=10 ci<si | ci!=-1 & si==-1
		1.Antwort ci=5, si=3 si<ci
		2.Antwort ci=5, si=3 si<ci
		3.Antwort ci=5, si=3 si<ci
		Zeit = 25; FrageNr 3; Punkte 2; Falsch -1; ohne Antwort 0  c=-1, s=3
		*/

		ci = line.indexOf('.');
		si = line.indexOf(' ');
		// wenn Zeile mit Space beginnt, dann ermittle wieviele Space
		if (si == 0)	// Zeile beginnt mit Space?
		{
			int next_si = 0;
			while (si == next_si) // noch ein Space in Folge gefunden
			{
				next_si = line.indexOf(' ', si + 1);	//
				if (si + 1 == next_si)
					si = next_si;
			}
		}

		//Serial.print("ci:"); Serial.println(ci);
		//Serial.print("si:"); Serial.println(si);
		//Serial.println(anzahl_zeilen);

		// 1.Frage ci=1 si=10 ci<si
		if (ci != -1 && (ci < si || si == -1))
		{
			//Serial.print("Frage"); Serial.println(si);

			//Fragen_Liste[anzahl_fragen] = FrageClass();
			//fr = &Fragen_Liste[anzahl_fragen];	// neue Frage anlegen
			//fr = &naechsteFrage;	// neue Frage anlegen

			//Serial.print("anzahl_fragen"); Serial.println(anzahl_fragen);

			String str_tmp = line.substring(0, ci); // Numerierung der Frage steht vor dem Komma
			naechsteFrage.frage_nr = str_tmp.toInt();	// Wandle in Integer
			
														//naechsteFrage.frage = (line.substring(ci + 1));	// Text der Frage steht nach dem Komma
			String str_fr = line.substring(ci + 1);
			str_fr.toCharArray(naechsteFrage.frage, 150);	// Text der Antwort steht nach dem Komma
			
			Serial.print("Frage"); Serial.print(naechsteFrage.frage_nr); Serial.println(naechsteFrage.frage);
			//Serial.println(str_fr);
			//Serial.println(naechsteFrage.frage_nr);
			//Serial.println(str_fr.length());
			anzahl_fragen++;
		}

		// 1.Antwort ci=5, si=3 si<ci
		else if (ci != -1 && si < ci)
		{
			int antwort_nr = 0;
			String str_tmp = line.substring(0, ci); // Numerierung der Antwort steht vor dem Komma
			antwort_nr = str_tmp.toInt();	// Wandle in 
			if (antwort_nr > 3 || antwort_nr == 0)
			{
				Serial.println("Ungültige Nummerierung der Antwort");
				Serial.println(line);
			}
			else
			{
				//Serial.print("ci:"); Serial.print(ci);
				//Serial.print("si:"); Serial.print(si);
				String str_aw = line.substring(ci + 1);
				//naechsteFrage.antworten[antwort_nr - 1] = line.substring(ci + 1);	// Text der Antwort steht nach dem Komma
				str_aw.toCharArray(naechsteFrage.antworten[antwort_nr - 1], 150);	// Text der Antwort steht nach dem Komma
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

			String str = String("Zeit");
			int index = line.indexOf(str, si) + str.length();
			int sp = line.indexOf(';', index);
			str_tmp = line.substring(index, sp); // Numerierung steht vor dem Strichpunkt
			int value = str_tmp.toInt();	// Wandle in Zahl um
			Serial.print(str); Serial.println(value);
			naechsteFrage.zeit = value;

			str = String("Nr"); //AntwortNr
								//Serial.println(str);
			index = line.indexOf(str, si) + str.length();
			sp = line.indexOf(';', index);

			//Serial.print("index:"); Serial.print(index);
			//Serial.print("sp:"); Serial.println(sp);

			//Serial.print("line:"); Serial.println(line);
			str_tmp = line.substring(index, sp); // Numerierung steht vor dem Strichpunkt


												 //Serial.print("str_tmp:"); Serial.println(str_tmp);

			value = str_tmp.toInt();	// Wandle in Zahl um

			Serial.print(str); Serial.print(value);
			naechsteFrage.richtige_antwort = value;

			str = String("Punkte");
			index = line.indexOf(str, si) + str.length();
			sp = line.indexOf(';', index);
			str_tmp = line.substring(index, sp); // Numerierung steht vor dem Strichpunkt
			value = str_tmp.toInt();	// Wandle in Zahl um
			Serial.print(str); Serial.print(value);
			naechsteFrage.punkte_richtig = value;

			str = String("Falsch");
			index = line.indexOf(str, si) + str.length();
			sp = line.indexOf(';', index);
			str_tmp = line.substring(index, sp); // Numerierung steht vor dem Strichpunkt
			value = str_tmp.toInt();	// Wandle in Zahl um
			Serial.print(str); Serial.print(value);
			naechsteFrage.punkte_falsch = value;

			str = String("ohne Antwort");
			index = line.indexOf(str, si) + str.length();
			sp = line.indexOf(';', index);
			str_tmp = line.substring(index, sp); // Numerierung steht vor dem Strichpunkt
			value = str_tmp.toInt();	// Wandle in Zahl um
			Serial.print(str); Serial.println(value);
			naechsteFrage.punkte_ohne_antwort = value;

			result = naechsteFrage.frage_nr;

		}

		//Serial.println(line);
		anzahl_zeilen++;
	}

	return result;
	digitalWrite(SDCARD_CS, HIGH);	// SPI SD Karte ausschalten (Ethernet ein)
	digitalWrite(W5200_CS, LOW);
}

bool FragenClass::Set_IPAddress(String str_ip)
{
	bool result = true;

	int commaIndex = 0;
	uint8_t ip_byte[4];
	for (int i = 0; i < 4; i++)
	{
		int nextCommaIndex = str_ip.indexOf('.', commaIndex);
		int value = 0;
		String str_tmp = "";
		if (nextCommaIndex != -1)
		{
			str_tmp = str_ip.substring(commaIndex, nextCommaIndex);
			commaIndex = nextCommaIndex + 1;
		}
		else
		{
			if (i < 2)
			{
				Serial.println("Fehler in IP Adresse - Anzahl Dezimalpunkte");
				Serial.println(commaIndex);
				Serial.println(nextCommaIndex);
				result = false;
			}
			else
				str_tmp = str_ip.substring(commaIndex);
		}
		value = str_tmp.toInt();
		ip_byte[i] = (byte)value;
		//Serial.println(ip_byte[i]);
	}
	if (result != false)
	{
		ip = IPAddress(ip_byte);
		Serial.println(ip);
	}

	return result;

}

FragenClass Fragen;