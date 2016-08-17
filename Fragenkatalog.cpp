// 
// 
// 

#include "Fragenkatalog.h"
#include <SPI.h>
#include <SD.h>
#include <Ethernet.h>

#define SDCARD_CS 4


class Fragenkatalog
{
public:
	int Lese_Datei(String Name);
	int Lese_Fragen_aus_Datei();

private:

	String * line_buffer;
	//Frage fr[];
	IPAddress ip;
};

int Fragenkatalog::Lese_Fragen_aus_Datei()
{
	int anzahl_fragen = 0;
	return anzahl_fragen;

}

int Fragenkatalog::Lese_Datei(String Name)
{
	File fkFile;
	int anzahl_zeilen = 0;

	if (!SD.begin(SDCARD_CS))	// Initializes the SD library and card
	{
		Serial.println("SD Initialisierung fehlgeschlagen!");
		return anzahl_zeilen;
	}
	Serial.println("SD Initialisierung erfolgreich!");

	fkFile = SD.open(Name);
	if (fkFile)
	{
		Serial.println(Name);

		while (fkFile.available())
		{
			String line = fkFile.readStringUntil('\n');
			if (anzahl_zeilen == 0)
			{
				if (line.indexOf("IP") != -1)
				{
					int index = line.indexOf(":");
					if (index != -1)
					{
						String ip_str = line.substring(index + 1);
						Serial.println("ip_str:");
						Serial.println(ip_str);
						int commaIndex = -1;
						uint8_t ip_byte[4];
						for (int i = 0; i <= 4; i++)
						{
							int nextCommaIndex = ip_str.indexOf('.', commaIndex + 1);
							int value = 0;
							String str_tmp;
							if (nextCommaIndex != -1)
							{
								str_tmp = ip_str.substring(commaIndex + 1, nextCommaIndex);
								commaIndex = nextCommaIndex;
							}
							else
							{
								str_tmp = ip_str.substring(commaIndex + 1);
							}


							Serial.println(str_tmp);
							value = str_tmp.toInt();
							Serial.println(value);
							ip_byte[i] = (byte)value;
						}
						ip = IPAddress(ip_byte);
						Serial.println("ip:");
						Serial.println(ip);
					}
				}
			}
			/*
			1.Frage eins ci=1 si=7 ci<si | ci!=-1 & si==-1
			1.Antwort ci=3, si=0 si<ci
			2.Antwort ci=3, si=0 si<ci
			3.Antwort ci=3, si=0 si<ci
			Zeit = 25 ; FrageNr 3; Punkte 2; Falsch -1; ohne Antwort 0  c=-1, s=0
			*/

			String str_tmp;
			int ni = line.indexOf('.');
			int si = line.indexOf(' ');
			// wenn Zeile mit Space beginnt, dann ermittle Index vom letzen aufeinanderfolgenden Space
			if (si == 0)	// Zeile beginnt mit Space?
			{
				int next_si = 0;
				while (next_si != -1) // noch ein Space in Folge gefunden
				{
					next_si = line.indexOf(' ', si + 1);	//
					if (si + 1 == next_si)
					{
						si = next_si;
					}
				}
			}
			Serial.println(line);
			anzahl_zeilen++;
		}
		fkFile.close();	// close the file:

		String s = String(anzahl_zeilen);
		s += " Zeilen aus Datei gelesen ";
		s += Name;
		Serial.println(s);
	}
	else
	{
		// if the file didn't open, print an error:
		String s = "Fehler beim Öffnen der Datei: ";
		s += Name;
		Serial.println(s);
	}
	return anzahl_zeilen;
}



