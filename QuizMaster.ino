// Do not remove the include below
#include "Teilnehmer.h"
#include "Frage.h"
#include "Fragen.h"
#include <SD.h>
#include <EthernetUdp.h>
#include <EthernetServer.h>
#include <EthernetClient.h>
#include <Dns.h>
#include <Dhcp.h>
#include <SPI.h>
#include <Ethernet.h>
#include "TimerOne.h"  
//#include "SdFat.h"
//#include "SdFatUtil.h"

#define Version "1.06"

#define alle_x_ms 100	// wie oft sollen die Tasten gelesen werden IO-Eingänge

const int buttonPin1 = 22;

// define the controller's MAC address
byte mac[] = {
	0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };
// SDCARD STUFF
Sd2Card card;
SdVolume volume;
SdFile root;
SdFile qm;
//SdFile jquery;
SdFile quizFile;

char fileList[10][13];
char fileName[13] = "Test.txt";
byte fileNrSelected = 0;

byte auswertungSelected = 1;

// set the server at port 80 for HTTP
int port = 80;
// char ipAddress[20];
EthernetServer server(port);


// Ethernet controller SPI CS pin
#define W5200_CS  10
// SD card SPI CS pin
#define SDCARD_CS 4
FragenClass fk = FragenClass();
TeilnehmerClass teilnehmer[maxAnzahlTeilnehmer] = { TeilnehmerClass() };

unsigned long previousMillis = 0;        // will store last time
unsigned long interval = 10000;           // interval at which (milliseconds)
long restzeit = 0;

#define Dateiwahl 0
#define Teilnehmersuche 1
#define Quiz 2
#define Antwort 3
#define Auswertung 4
#define Ende 10

byte status_html = Dateiwahl;

int frage_nr = -1;

int buttonState = 0;         // variable for reading the pushbutton status

bool alleTeilnehmerGedrueckt = false;
bool ZeitUm = false;
byte fehlendeTastenTeilnehmer = maxAnzahlTeilnehmer;

char html_str[4 * maxZeichenJeZeile];

void setup() 
{
	Serial.begin(38400);
	byte PinNr = 0;

	for (byte t = 1; t <= maxAnzahlTeilnehmer; t++)
	{
		for (byte k = 1; k <= 3; k++)
		{
			PinNr = Taster_Belegung[t - 1][k - 1];
			pinMode(PinNr, INPUT);	// set pin to input
			digitalWrite(PinNr, LOW);	// turn off pullup resistors
		}
	}

	pinMode(Taster_UP, INPUT);	// set pin to input
	digitalWrite(Taster_UP, LOW);	// turn off pullup resistors

	Timer1.initialize(alle_x_ms * 1000);
	Timer1.attachInterrupt(lese_taster);

	//Serial.println(FreeRam());

	// initialize the SD card at SPI_HALF_SPEED to avoid bus errors with
	// breadboards.  use SPI_FULL_SPEED for better performance.
	
	pinMode(SDCARD_CS, OUTPUT);
	digitalWrite(SDCARD_CS, LOW);	// SPI SD Karte einschalten

	pinMode(W5200_CS, OUTPUT);
	digitalWrite(W5200_CS, HIGH);	// W5100 chip ausschalten

	if (!card.init(SPI_HALF_SPEED, SDCARD_CS))
		Serial.println("card.init failed!");

	// initialize a FAT volume
	if (!volume.init(&card)) 
		Serial.println("vol.init failed!");

	Serial.println("Volume is FAT");
	Serial.println(volume.fatType(), DEC);
	Serial.println();
	
	if (!root.openRoot(&volume)) 
		Serial.println("openRoot failed");

	if (qm.open(&root, "qm", O_READ))
		Serial.println("Opened qm");
		
	dir_t d;
	if (!qm.dirEntry(&d)) {
			Serial.println("file.dirEntry failed");
		}

	// list file in root with date and size
	Serial.println("Files found in qm:");
	qm.ls(LS_DATE | LS_SIZE);
	Serial.println();

	Serial.println();
	Serial.println("Done");

	
	//if (jquery.open(&root, "jquery.js", O_READ))
	//{
	//	Serial.println("Opened jquery.js");
	//}
	//else
	//	Serial.println("File open error jquery.js");

	int anzahl_zeilen = 0;

	IPAddress ip_set = IPAddress(192, 168, 0, 211);
	IPAddress dns = IPAddress(192, 168, 0, 1);
	IPAddress gw = IPAddress(192, 168, 0, 1);
	IPAddress sn = IPAddress(255, 255, 255, 0);
	Ethernet.begin(mac, ip_set);

	//Ethernet.begin()
	// Prüfen ob Verbindung erfolgreich??

	//while (Ethernet.begin(mac) == 0) {
	//Serial.println("DHCP configuration failed. Trying again...");
	//}
	// now that we have a good ethernet connection we can start the server

	server.begin();
	Serial.print("The server can be accessed at: ");
	IPAddress ip = Ethernet.localIP();
	
	Serial.println(Ethernet.localIP());
}

// Timer1.attachInterrupt
void lese_taster()
{
	teilnehmer[0].leseTaster();
}

void loop() {
	// listen for incoming clients
	EthernetClient client = server.available();
	boolean currentLineIsBlank = true;
	
	if (client) {
		Serial.println("new client");
		
		// an http request ends with a blank line
		boolean currentLineIsBlank = true;
		while (client.connected()) {
			if (client.available()) {
				char c = client.read();
				Serial.write(c);
				//Serial.println("");

				if (status_html == Dateiwahl)
				{
					// if you've gotten to the end of the line (received a newline
					// character) and the line is blank, the http request has ended,
					// so you can send a reply
					if (c == '\n' && currentLineIsBlank)
					{
						// send a standard http response header
						Serial.println("");
						Serial.println("schreibe Dateiwahl");
						schreibeDateiListe(client, 0);

						break;
					}
					if (c == '\n') {
						// you're starting a new line
						currentLineIsBlank = true;
					}
					else if (c != '\r') {
						// you've gotten a character on the current line
						currentLineIsBlank = false;
					}
					byte taste = teilnehmer[0].leseFunktionstaste();
					if (taste == 1)
					{
						fileNrSelected++;
					}
					else if (taste > 1)
					{
						
						if (quizFile.isOpen())
						{
							Serial.println(""); 
							Serial.print("Close File");
							quizFile.close();
						}
						if (quizFile.open(&qm, fileName, O_READ))
						{
							Serial.println("");
							Serial.print("Opened: ");
							Serial.println(fileName);
							quizFile.rewind();
							alleTeilnehmerGedrueckt = false;
							ZeitUm = false;

							teilnehmer[0].loescheTasten();
							teilnehmer[0].loescheTeilnehmer();
							teilnehmer[0].loeschePunkte();
							fk.anzahl_fragen = 0;

							status_html++;
							//result = true;
						}
						else
						{
							Serial.print("File open error: ");
							Serial.println(fileName);
						}
						teilnehmer[0].loescheTeilnehmer();
					}
				}
					
				if (status_html == Teilnehmersuche)
				{
					// if you've gotten to the end of the line (received a newline
					// character) and the line is blank, the http request has ended,
					// so you can send a reply
					if (c == '\n' && currentLineIsBlank)
					{
						// send a standard http response header
						Serial.println("");
						Serial.println("schreibeAnzahlTeilnehmer");
						schreibeAnzahlTeilnehmer(client);

						break;
					}
					if (c == '\n') {
						// you're starting a new line
						currentLineIsBlank = true;
					}
					else if (c != '\r') {
						// you've gotten a character on the current line
						currentLineIsBlank = false;
					}
					byte taste = teilnehmer[0].leseFunktionstaste();

					if (taste >= 1)
					{
						frage_nr = -1;
						previousMillis = millis();
						teilnehmer[0].loescheTasten();
						teilnehmer[0].loeschePunkte();
						for (byte t = 1; t <= maxAnzahlTeilnehmer; t++)
						{
							teilnehmer[t - 1].Teilnehmer_Nr = t;
							teilnehmer[t - 1].setzteStatus(t);
						}
						status_html++;
					}
				}
				if (status_html == Quiz)
				{
					if (frage_nr != -1)
					{
						unsigned long currentMillis = millis();
						//Serial.print("zeit:"); Serial.println(fk.naechsteFrage.zeit);
						interval = (unsigned long)fk.naechsteFrage.zeit * 1000;
						restzeit = (interval - (currentMillis - previousMillis))/1000;
						//Serial.print("interval"); Serial.println(interval);
						//Serial.print("previousMillis"); Serial.println(previousMillis);
						//Serial.print("currentMillis"); Serial.println(currentMillis);
						//Serial.print("restzeit"); Serial.println(restzeit);
						if (restzeit < 0)
							restzeit = 0;
						fehlendeTastenTeilnehmer = teilnehmer[0].alleTeilnehmerGedrueckt();
						if (fehlendeTastenTeilnehmer == 0)
							alleTeilnehmerGedrueckt = true;

						if ((currentMillis - previousMillis) > interval)
						{
							if (!ZeitUm)
							{
								Serial.println("");
								Serial.println("Zeit um: ");
								Serial.println(interval / 1000);
								ZeitUm = true;
							}
						}
						// if you've gotten to the end of the line (received a newline
						// character) and the line is blank, the http request has ended,
						// so you can send a reply
						if (c == '\n' && currentLineIsBlank) 
						{
							// send a standard http response header
							Serial.println("");
							Serial.println("schreibe Frage");
							if (ZeitUm || alleTeilnehmerGedrueckt)
							{
								teilnehmer[0].loescheFNTaste();
								status_html = Antwort;
							}
							else
							{
								schreibeFrage(client);
							}
							break;
						}
						if (c == '\n') {
							// you're starting a new line
							currentLineIsBlank = true;
						}
						else if (c != '\r') {
							// you've gotten a character on the current line
							currentLineIsBlank = false;
						}
					}
					else
					{
						Serial.print("Lese_Frage");
						int tmp = -1;
						while (tmp == -1)
						{
							html_str[0] = 0;
							tmp = LeseZeile(html_str);
							if (tmp > 0)
								tmp = fk.Lese_Frage(html_str);
							else
								break;

						}

						Serial.println(tmp);
						if (tmp != -1)
						{
							frage_nr = tmp;
							Serial.println("");
							Serial.print("Lese_Frage erfolgreich:");
							Serial.print(frage_nr);
						}
						if (tmp == -1 || fk.anzahl_fragen >= maxAnzahlFragen)
						{
							status_html = Auswertung;
							if (quizFile.isOpen())
								quizFile.close();
						}
					}
				}
				if (status_html == Antwort)
				{
					// if you've gotten to the end of the line (received a newline
					// character) and the line is blank, the http request has ended,
					// so you can send a reply
					if (c == '\n' && currentLineIsBlank)
					{
						// send a standard http response header
						Serial.println("");
						Serial.println("schreibeFrageMitAntwort");
						schreibeFrageMitAntwort(client);

						break;
					}
					if (c == '\n') {
						// you're starting a new line
						currentLineIsBlank = true;
					}
					else if (c != '\r') {
						// you've gotten a character on the current line
						currentLineIsBlank = false;
					}
					byte taste = teilnehmer[0].leseFunktionstaste();

					if (taste >= 1)//> 1)
					{
						for (byte t = 1; t <= maxAnzahlTeilnehmer; t++)
						{
							teilnehmer[t - 1].auswertungPunkte(fk);
							Serial.print("Tn:"); Serial.println(t);
							Serial.print("Punkte:"); Serial.println(teilnehmer[t - 1].punkte);
						}
						if (taste > 2)
						{
							//for (byte t = 1; t <= maxAnzahlTeilnehmer; t++)
							//{
							//	//punkte[t - 1] = teilnehmer[t - 1].punkte;
							//	teilnehmer[t - 1].punkte = random(300);
							//	teilnehmer[t - 1].aktive = true;
							//}
							auswertungSelected = 1;
							qsort(teilnehmer, maxAnzahlTeilnehmer, sizeof(TeilnehmerClass), myCompareFunction);
							status_html = Auswertung;
						}
						else
							status_html = Quiz;
						previousMillis = millis();
						ZeitUm = false;
						alleTeilnehmerGedrueckt = false;
						teilnehmer[0].loescheTasten();
						frage_nr = -1;
					}

				}
				if (status_html == Auswertung)
				{
					// if you've gotten to the end of the line (received a newline
					// character) and the line is blank, the http request has ended,
					// so you can send a reply
					if (c == '\n' && currentLineIsBlank)
					{
						// send a standard http response header
						Serial.println("");
						Serial.println("schreibeAuswertung");
						schreibeAuswertung(client);

						break;
					}
					if (c == '\n') {
						// you're starting a new line
						currentLineIsBlank = true;
					}
					else if (c != '\r') {
						// you've gotten a character on the current line
						currentLineIsBlank = false;
					}
					
					byte taste = teilnehmer[0].leseFunktionstaste();
					if (taste > 1)
					{
						status_html = Ende;
					}
					else if (taste == 1)
					{
						auswertungSelected++;
					}
					
				}
				if (status_html == Ende)
				{

					teilnehmer[0].loescheTasten();
					teilnehmer[0].loescheTeilnehmer();
					teilnehmer[0].loeschePunkte();

					for (byte t = 1; t <= maxAnzahlTeilnehmer; t++)
					{
						//teilnehmer[t - 1].auswertungPunkte(fk);
						teilnehmer[t - 1].punkte = 0;
					}
				

					status_html = Dateiwahl;
					frage_nr = -1;
				}
			}
		}
		// give the web browser time to receive the data
		delay(1);
		// close the connection:
		client.stop();
		Serial.println("client disonnected");
	}
}

bool schreibeAnzahlTeilnehmer(EthernetClient client)
{
	schreibeHeader(client);
	client.print("<h3>Ermittle Anzahl der Teilnehmer:</h3>");
	client.print("<h1>Bitte alle die Taste Nr: 1 druecken</h1>");

	html_str[0] = 0;
	strcat(html_str, "<h2>");	// String tn = "<h2>";
	teilnehmer[0].ermittleListeTeilnehmer(html_str);
	strcat(html_str, "</h2>");	//tn += "</h2>";
	client.print(html_str);
	Serial.println(html_str);

	html_str[0] = 0;
	strcat(html_str, "<h2>");	// String tn = "<h2>";
	teilnehmer[0].ermittleTasteTeilnehmer(html_str);
	strcat(html_str, "</h2>");	//tn += "</h2>";
	client.print(html_str);
	Serial.println(html_str);

	//client.print("<br />");
	anzahl_Teilnehmer = teilnehmer[0].ermittleAnzahlTeilnehmer();
	client.print("<h2>");
	client.print("Anzahl Teilnehmer: ");  client.println(anzahl_Teilnehmer);
	client.print("</h2>");
	client.print("<br />Weiter mit Taste!");
}

void schreibeFrage(EthernetClient client)
{

	schreibeHeader(client);
	client.print("QuizDatei: ");  client.println(fileName);
	client.print("<br />");
	
	html_str[0] = 0;
	fk.naechsteFrage.get_html_frage(html_str);

	Serial.print(html_str);
	client.print(html_str);

	html_str[0] = 0;
	fk.naechsteFrage.get_html_antworten(0, html_str);
	
	client.print(html_str);

	client.print("<br />");
	client.print("<br />");
	client.print("<h2>");
	client.print("Zeit: ");  client.println(restzeit);
	client.print("<br />");
	client.print("Teilnehmer ohne Antwort: ");  client.println(fehlendeTastenTeilnehmer);
	client.print("<br />");
	client.print("</h2>");
	html_str[0] = 0;
	teilnehmer[0].ermittleListeTeilnehmerGedrueckt(html_str);
	client.print("Teilnehmer hat geantwortet: ");  client.println(html_str);
}

int LeseZeile(char *line)
{
	memset(line, 0, maxZeichenJeZeile);
	char c = 0;
	int cnt = 0;
	
	//quizFile.rewind();

	do
	{
		c = quizFile.read();
		if (c != EOF)
		{
			Serial.print(c);
			line[cnt] = c;
			cnt++;
			if (cnt >= maxZeichenJeZeile)
			{
				Serial.println("Zeile zu lange:");
				do
				{
					c = quizFile.read();
					Serial.print(c);
				} while (c != EOF && c != '\n');
			}
		}
	} while (c != EOF && c != '\n');
	Serial.print("Zeile:");
	Serial.println(line);
	if (cnt == 0)
		return -1;
	else
		return cnt;

}

void schreibeFrageMitAntwort(EthernetClient client)
{
	schreibeHeader(client);
	client.print("QuizDatei: ");  client.println(fileName);
	client.print("<br />");
	int antw = fk.naechsteFrage.richtige_antwort;

	memset(html_str, 0, 4 * maxZeichenJeZeile);
	fk.naechsteFrage.get_html_frage(html_str);	// client.print(fk.naechsteFrage.get_html_frage());
	client.print(html_str);

	fk.naechsteFrage.get_html_antworten(antw, html_str);	// fk.naechsteFrage.get_html_antworten(antw);

	client.print(html_str);
	Serial.println(html_str);

	
	client.print("<br />Weiter mit Taste! ");
	client.print("<br />Abbruch Quiz, zur Auswertung - Taste min. 5Sekunden druecken ");
}

void schreibeAuswertung(EthernetClient client)
{
	schreibeHeader(client);
	client.print("QuizDatei: ");  client.println(fileName);
	client.print("<br />");

	client.print("<h3>Auswertung:</h3>");
	String tn = "<h2>";
	byte tmp[10];

	//int punkte[maxAnzahlTeilnehmer] = {};
	
	

	byte tNr = 1;
	byte t = 0;

	if (auswertungSelected > maxAnzahlTeilnehmer)
		auswertungSelected = maxAnzahlTeilnehmer;

	//Serial.println("");
	//Serial.print("auswertungSelected:");
	//Serial.print(auswertungSelected);
	//Serial.print(" t:");
	//Serial.print(t);

	while (tNr < auswertungSelected)
	{
		if (teilnehmer[t].aktive)
		{
			tn += "Teilnehmer:";
			tn += teilnehmer[t].Teilnehmer_Nr; //t;
			tn += " Punkte:";
			tn += teilnehmer[t].punkte;
			tn += "<br />";
			tNr++;
		}
		if (t < maxAnzahlTeilnehmer)
			t++;
		else
			break;

	}

	//for (byte t = 1; t <= maxAnzahlTeilnehmer; t++)
	//{
	//	if (teilnehmer[t - 1].aktive)
	//	{
	//		tn += "Teilnehmer:";
	//		tn += teilnehmer[t - 1].Teilnehmer_Nr; //t;
	//		tn += " Punkte:";
	//		tn += teilnehmer[t - 1].punkte;
	//		tn += "<br />";
	//	}
	//}
	tn += "<br />";
	tn += "max: ";
	tn += " Punkte:";
	tn += teilnehmer[0].SummePunkte();
	tn += "<br />";
	tn += "</h2>";
	client.print(tn);

	client.print("<br />Teilnehmer auflisten mit Taste!");
	client.print("<br />Neustart mit Taste! (min. 2Sekunden druecken) ");
}




void schreibeHeader(EthernetClient client)
{
	client.println("HTTP/1.1 200 OK");
	client.println("Content-Type: text/html");
	client.println("Connnection: close");
	client.println("Refresh: 1");  // refresh the page automatically every 5 sec
	client.println();
	client.println("<!DOCTYPE HTML>");
	client.print("<html>");
	client.print("<head>");
	client.print("<meta charset=\"UTF - 8\">");
	//client.print("<meta charset=\"ISO - 8859 - 1\">");
	client.print("<title>Quizmaster</title>");
	client.print("</head>");
	client.print("<body>");
	client.print("<h3>Quizmaster Webpage Version:");
	client.print(Version);
	client.print("</h3>");

}

bool schreibeDateiListe(EthernetClient client, uint8_t flags)
{
	int file_nr = 0;
	bool result = false;

	dir_t p;

	qm.rewind();


	while (qm.readDir(p) > 0)
	{
		// done if past last used entry
		if (p.name[0] == DIR_NAME_FREE) break;

		// skip deleted entry and entries for . and  ..
		if (p.name[0] == DIR_NAME_DELETED || p.name[0] == '.') continue;

		// only list subdirectories and files
		if (!DIR_IS_FILE_OR_SUBDIR(&p)) continue;

		char FileName[13];

		int y = 0;
		for (uint8_t i = 0; i < 11; i++) {
			if (p.name[i] == ' ')
				continue;
			if (i == 8)
			{
				FileName[y] = '.';
				y++;
			}
			//Serial.print(p.name[i]);
			FileName[y] = p.name[i];
			y++;
		}
		FileName[y] = '\0';
		//Serial.println(FileName);
		strcpy(fileList[file_nr], FileName); //fileList[file_nr] = FileName; 
		Serial.println(fileList[file_nr]);
		file_nr++;
	}

	schreibeHeader(client);

	client.print("<h3>Auswahl der Quiz-Datei:</h3>");
	client.print("<h1>Bitte eine Datei aus der Liste waehlen:</h1>");

	// the string "pin=" is not present in the browser/client HTTP request
	// so display the buttons webpage.

	// send a standard http response header

	// Create the HTML button code e.g. <button id="xx" class="led">Toggle Pin xx</button> where xx is the pin number
	// In the <button id="xx" class="led"> tags, the ID attribute is the value sent to the arduino via the "pin" GET parameter
	if (fileNrSelected >= file_nr)
		fileNrSelected = 0;

	for (int i = 0; i < file_nr; i++)
	{
		html_str[0] = 0;
		strcat(html_str, "<h2>");	// String file = "<h2>";
		
		if (i == fileNrSelected)
			strcat(html_str, "<mark>");	//	file += "<mark>";
		strcat(html_str, fileList[i]);	//file += fileList[i];
		if (i == fileNrSelected)
			strcat(html_str, "</mark>");	//	file += "</mark>";
		strcat(html_str, "</h2>");	//file += "</h2>";
		client.println(html_str);
	}
	Serial.print("fileNrSelected");
	Serial.println(fileNrSelected);
	strcpy(fileName, fileList[fileNrSelected]);

	client.print("<br />Weiterscrollen: Kurzer Tastendruck");
	client.print("<br />Auswaehlen: Langer Tastendruck  (min 2 Sekunden)");
}

// callback function for doing comparisons
int myCompareFunction(const void * arg1, const void * arg2)
{
	int a = ((TeilnehmerClass*)arg1)->punkte;
	int b = ((TeilnehmerClass*)arg2)->punkte;

	//Serial.println(a);
	//Serial.println(b);

	// a less than b? 
	if (a < b)
		return -1;

	// a greater than b?
	if (a > b)
		return 1;

	// must be equal
	return 0;
}  // end of myCompareFunction