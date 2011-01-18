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
const prog_char XML_END[] PROGMEM = "</ATOHIGH></RA>";

#ifdef wifi
#define SERVER_DENY "HTTP/1.1 401 Access Denied" CRLF "WWW-Authenticate: Basic realm=Reef Angel Controller" SERVER_HEADER2 "0" SERVER_HEADER3
#define SERVER_HEADER1 "HTTP/1.1 200 OK" CRLF "Server: ReefAngel" CRLF "Cache-Control: no-store, no-cache, must-revalidate" CRLF "Pragma: no-cache" CRLF "Connection: close" CRLF "Content-Type: text/"
#define SERVER_HEADER2 CRLF "Content-Length: "
#define SERVER_HEADER3 CRLF CRLF
#define SERVER_HEADER_HTML SERVER_HEADER1 "html" SERVER_HEADER2
#define SERVER_HEADER_XML SERVER_HEADER1 "xml" SERVER_HEADER2
#define SERVER_DEFAULT "<h1>Reef Angel Controller Web Server</h1>"
#define SERVER_RELAY ""
#define CRLF "\r\n"
#define CR "\r"
#define LF "\n"
#define BAD "bad=1"

#define P(name)   static const prog_char name[] PROGMEM
const prog_char SERVER_RA[] PROGMEM = "<script language='javascript' src='http://www.reefangel.com/wifi/ra1.js'></script>";
const prog_char EncodingChars[] PROGMEM = {"ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/"};
const prog_char BannerGET[] PROGMEM = "GET /status/submit.asp?t1=";
const prog_char BannerT2[] PROGMEM = "&t2=";
const prog_char BannerT3[] PROGMEM = "&t3=";
const prog_char BannerPH[] PROGMEM = "&ph=";
const prog_char BannerRelayData[] PROGMEM = "&relaydata=";

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
static PROGMEM const char *webbanner_tags[] = {
	id_tag, probe1_tag, probe2_tag, probe3_tag, relay1_tag,	relay2_tag,
	relay3_tag, relay4_tag, relay5_tag, relay6_tag, relay7_tag, relay8_tag};

static char m_pushback[32];
static byte m_pushbackindex=0;
static byte reqtype=0;
static unsigned long timeout;
static boolean bIncoming=false;
static boolean auth=false;
static char authStr[32];
static int weboption=0;


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
