/*
 * Copyright 2010 Reef Angel / Roberto Imai
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

 /*
  * Updated by:  Curt Binder
  * Updates Released under Apache License, Version 2.0
  */

#ifndef __REEFANGEL_WIFI_H__
#define __REEFANGEL_WIFI_H__

#include <ReefAngel_Globals.h>
#include <avr/pgmspace.h>

const prog_char XML_T1[] PROGMEM = "<RA><T1>";
const prog_char XML_T2[] PROGMEM = "</T1><T2>";
const prog_char XML_T3[] PROGMEM = "</T2><T3>";
const prog_char XML_PH[] PROGMEM = "</T3><PH>";
const prog_char XML_R[] PROGMEM = "</PH><R>";
const prog_char XML_RON[] PROGMEM = "</R><RON>";
const prog_char XML_ROFF[] PROGMEM = "</RON><ROFF>";
const prog_char XML_RE_OPEN[] PROGMEM = "<R";
const prog_char XML_RE_CLOSE[] PROGMEM = "</R";
const prog_char XML_RE_ON[] PROGMEM = "ON";
const prog_char XML_RE_OFF[] PROGMEM = "OFF";
const prog_char XML_ATOLOW[] PROGMEM = "<ATOLOW>";
const prog_char XML_ATOHIGH[] PROGMEM = "</ATOLOW><ATOHIGH>";
const prog_char XML_PWMA[] PROGMEM = "</ATOHIGH><PWMA>";
const prog_char XML_PWMD[] PROGMEM = "</PWMA><PWMD>";
const prog_char XML_END[] PROGMEM = "</PWMD></RA>";
const prog_char XML_CLOSE_TAG[] PROGMEM = ">";


#ifdef wifi
const prog_char XML_M_OPEN[] PROGMEM = "<M";
const prog_char XML_M_CLOSE[] PROGMEM = "</M";
const prog_char XML_MEM_OPEN[] PROGMEM = "<MEM>";
const prog_char XML_MEM_CLOSE[] PROGMEM = "</MEM>";
const prog_char XML_OK[] PROGMEM = "OK";
const prog_char XML_ERR[] PROGMEM = "ERR";

#define SERVER_DENY "HTTP/1.1 401 Access Denied" CRLF "WWW-Authenticate: Basic realm=Reef Angel Controller" SERVER_HEADER2 "0" SERVER_HEADER3
#define SERVER_HEADER1 "HTTP/1.1 200 OK" CRLF "Server: ReefAngel" CRLF "Cache-Control: no-store, no-cache, must-revalidate" CRLF "Pragma: no-cache" CRLF "Connection: close" CRLF "Content-Type: text/"
#define SERVER_HEADER2 CRLF "Content-Length: "
#define SERVER_HEADER3 CRLF CRLF
#define SERVER_HEADER_HTML SERVER_HEADER1 "html" SERVER_HEADER2
#define SERVER_HEADER_XML SERVER_HEADER1 "xml" SERVER_HEADER2
#define SERVER_DEFAULT "<h1>Reef Angel Controller Web Server</h1>"
#define SERVER_UKNOWN "<h1>Unknown Request</h1>"
#define SERVER_RELAY ""
#define CRLF "\r\n"
#define CR "\r"
#define LF "\n"
#define BAD "bad=1"

// REQUEST TYPES
#define REQ_ROOT		1		// Default page
#define REQ_WIFI		2		// Wifi page
#define REQ_RELAY		3		// Relay status information
#define REQ_M_BYTE		4  		// Memory read/write byte
#define REQ_M_INT		5  		// Memory read/write int
#define REQ_M_ALL		6  		// All memory values
#define REQ_VERSION		7  		// Library version
#define REQ_DATE		8  		// Date and time
#define REQ_UNKNOWN		128	 	// Unknown request

#define P(name)   static const prog_char name[] PROGMEM
const prog_char SERVER_RA[] PROGMEM = "<script language='javascript' src='http://www.reefangel.com/wifi/ra1.js'></script>";
const prog_char EncodingChars[] PROGMEM = {"ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/"};
const prog_char BannerGET[] PROGMEM = "GET /status/submit.asp?t1=";
const prog_char BannerT2[] PROGMEM = "&t2=";
const prog_char BannerT3[] PROGMEM = "&t3=";
const prog_char BannerPH[] PROGMEM = "&ph=";
const prog_char BannerRelayData[] PROGMEM = "&relaydata";

const prog_char id_tag[] PROGMEM = "id";
const prog_char probe1_tag[] PROGMEM = "t1n";
const prog_char probe2_tag[] PROGMEM = "t2n";
const prog_char probe3_tag[] PROGMEM = "t3n";
const prog_char relay1_tag[] PROGMEM = "r1n";
const prog_char relay2_tag[] PROGMEM = "r2n";
const prog_char relay3_tag[] PROGMEM = "r3n";
const prog_char relay4_tag[] PROGMEM = "r4n";
const prog_char relay5_tag[] PROGMEM = "r5n";
const prog_char relay6_tag[] PROGMEM = "r6n";
const prog_char relay7_tag[] PROGMEM = "r7n";
const prog_char relay8_tag[] PROGMEM = "r8n";
#ifdef RelayExp
#if InstalledRelayExpansionModules >= 1
const prog_char relay11_tag[] PROGMEM = "r11n";
const prog_char relay12_tag[] PROGMEM = "r12n";
const prog_char relay13_tag[] PROGMEM = "r13n";
const prog_char relay14_tag[] PROGMEM = "r14n";
const prog_char relay15_tag[] PROGMEM = "r15n";
const prog_char relay16_tag[] PROGMEM = "r16n";
const prog_char relay17_tag[] PROGMEM = "r17n";
const prog_char relay18_tag[] PROGMEM = "r18n";
#endif  // InstalledRelayExpansionModules >= 1
#if InstalledRelayExpansionModules >= 2
const prog_char relay21_tag[] PROGMEM = "r21n";
const prog_char relay22_tag[] PROGMEM = "r22n";
const prog_char relay23_tag[] PROGMEM = "r23n";
const prog_char relay24_tag[] PROGMEM = "r24n";
const prog_char relay25_tag[] PROGMEM = "r25n";
const prog_char relay26_tag[] PROGMEM = "r26n";
const prog_char relay27_tag[] PROGMEM = "r27n";
const prog_char relay28_tag[] PROGMEM = "r28n";
#endif  // InstalledRelayExpansionModules >= 2
#if InstalledRelayExpansionModules >= 3
const prog_char relay31_tag[] PROGMEM = "r31n";
const prog_char relay32_tag[] PROGMEM = "r32n";
const prog_char relay33_tag[] PROGMEM = "r33n";
const prog_char relay34_tag[] PROGMEM = "r34n";
const prog_char relay35_tag[] PROGMEM = "r35n";
const prog_char relay36_tag[] PROGMEM = "r36n";
const prog_char relay37_tag[] PROGMEM = "r37n";
const prog_char relay38_tag[] PROGMEM = "r38n";
#endif  // InstalledRelayExpansionModules >= 3
#if InstalledRelayExpansionModules >= 4
const prog_char relay41_tag[] PROGMEM = "r41n";
const prog_char relay42_tag[] PROGMEM = "r42n";
const prog_char relay43_tag[] PROGMEM = "r43n";
const prog_char relay44_tag[] PROGMEM = "r44n";
const prog_char relay45_tag[] PROGMEM = "r45n";
const prog_char relay46_tag[] PROGMEM = "r46n";
const prog_char relay47_tag[] PROGMEM = "r47n";
const prog_char relay48_tag[] PROGMEM = "r48n";
#endif  // InstalledRelayExpansionModules >= 4
#if InstalledRelayExpansionModules >= 5
const prog_char relay51_tag[] PROGMEM = "r51n";
const prog_char relay52_tag[] PROGMEM = "r52n";
const prog_char relay53_tag[] PROGMEM = "r53n";
const prog_char relay54_tag[] PROGMEM = "r54n";
const prog_char relay55_tag[] PROGMEM = "r55n";
const prog_char relay56_tag[] PROGMEM = "r56n";
const prog_char relay57_tag[] PROGMEM = "r57n";
const prog_char relay58_tag[] PROGMEM = "r58n";
#endif  // InstalledRelayExpansionModules >= 5
#if InstalledRelayExpansionModules >= 6
const prog_char relay61_tag[] PROGMEM = "r61n";
const prog_char relay62_tag[] PROGMEM = "r62n";
const prog_char relay63_tag[] PROGMEM = "r63n";
const prog_char relay64_tag[] PROGMEM = "r64n";
const prog_char relay65_tag[] PROGMEM = "r65n";
const prog_char relay66_tag[] PROGMEM = "r66n";
const prog_char relay67_tag[] PROGMEM = "r67n";
const prog_char relay68_tag[] PROGMEM = "r68n";
#endif  // InstalledRelayExpansionModules >= 6
#if InstalledRelayExpansionModules >= 7
const prog_char relay71_tag[] PROGMEM = "r71n";
const prog_char relay72_tag[] PROGMEM = "r72n";
const prog_char relay73_tag[] PROGMEM = "r73n";
const prog_char relay74_tag[] PROGMEM = "r74n";
const prog_char relay75_tag[] PROGMEM = "r75n";
const prog_char relay76_tag[] PROGMEM = "r76n";
const prog_char relay77_tag[] PROGMEM = "r77n";
const prog_char relay78_tag[] PROGMEM = "r78n";
#endif  // InstalledRelayExpansionModules >= 7
#if InstalledRelayExpansionModules >= 8
const prog_char relay81_tag[] PROGMEM = "r81n";
const prog_char relay82_tag[] PROGMEM = "r82n";
const prog_char relay83_tag[] PROGMEM = "r83n";
const prog_char relay84_tag[] PROGMEM = "r84n";
const prog_char relay85_tag[] PROGMEM = "r85n";
const prog_char relay86_tag[] PROGMEM = "r86n";
const prog_char relay87_tag[] PROGMEM = "r87n";
const prog_char relay88_tag[] PROGMEM = "r88n";
#endif  // InstalledRelayExpansionModules >= 8
#endif  // RelayExp
static PROGMEM const char *webbanner_tags[] = {
	id_tag, probe1_tag, probe2_tag, probe3_tag,
	relay1_tag, relay2_tag, relay3_tag, relay4_tag, relay5_tag, relay6_tag, relay7_tag, relay8_tag,
#ifdef RelayExp
#if InstalledRelayExpansionModules >= 1
	relay11_tag, relay12_tag, relay13_tag, relay14_tag, relay15_tag, relay16_tag, relay17_tag, relay18_tag,
#endif  // InstalledRelayExpansionModules >= 1
#if InstalledRelayExpansionModules >= 2
	relay21_tag, relay22_tag, relay23_tag, relay24_tag, relay25_tag, relay26_tag, relay27_tag, relay28_tag,
#endif  // InstalledRelayExpansionModules >= 2
#if InstalledRelayExpansionModules >= 3
	relay31_tag, relay32_tag, relay33_tag, relay34_tag, relay35_tag, relay36_tag, relay37_tag, relay38_tag,
#endif  // InstalledRelayExpansionModules >= 3
#if InstalledRelayExpansionModules >= 4
	relay41_tag, relay42_tag, relay43_tag, relay44_tag, relay45_tag, relay46_tag, relay47_tag, relay48_tag,
#endif  // InstalledRelayExpansionModules >= 4
#if InstalledRelayExpansionModules >= 5
	relay51_tag, relay52_tag, relay53_tag, relay54_tag, relay55_tag, relay56_tag, relay57_tag, relay58_tag,
#endif  // InstalledRelayExpansionModules >= 5
#if InstalledRelayExpansionModules >= 6
	relay61_tag, relay62_tag, relay63_tag, relay64_tag, relay65_tag, relay66_tag, relay67_tag, relay68_tag,
#endif  // InstalledRelayExpansionModules >= 6
#if InstalledRelayExpansionModules >= 7
	relay71_tag, relay72_tag, relay73_tag, relay74_tag, relay75_tag, relay76_tag, relay77_tag, relay78_tag,
#endif  // InstalledRelayExpansionModules >= 7
#if InstalledRelayExpansionModules >= 8
	relay81_tag, relay82_tag, relay83_tag, relay84_tag, relay85_tag, relay86_tag, relay87_tag, relay88_tag,
#endif  // InstalledRelayExpansionModules >= 8
#endif  // RelayExp
};

static char m_pushback[32];
static byte m_pushbackindex=0;
static byte reqtype=0;
static unsigned long timeout;
static boolean bIncoming=false;
static boolean auth=false;
static char authStr[32];
static int weboption=0;
static int webmemoryloc=-1;
static byte bHasSecondValue = false;
static byte bHasComma = false;


void WebResponse (const prog_char *response, long strsize);
void printP(const prog_char *str);
void pushbuffer(byte inStr);
char GetC(int c);
void ConvertC(char* strIn, char* strOut, byte len);
void WifiAuthentication(char* userpass);
void processHTTP();
#endif  // wifi

void pingSerial();
void PROGMEMprint(const prog_char str[]);

#endif  // __REEFANGEL_WIFI_H__
