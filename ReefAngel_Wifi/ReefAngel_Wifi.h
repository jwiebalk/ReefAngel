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
#define P(name)   static const prog_uchar name[] PROGMEM
const prog_uchar SERVER_RA[] PROGMEM = "<script language='javascript' src='http://www.reefangel.com/wifi/ra1.js'></script>";
const prog_char EncodingChars[] PROGMEM = {"ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/"};

static char m_pushback[32];
static byte m_pushbackindex=0;
static byte reqtype=0;
static unsigned long timeout;
static boolean bIncoming=false;
static boolean auth=false;
static char authStr[32];
static int weboption=0;


void WebResponse (const prog_uchar *response, long strsize);
void printP(const prog_uchar *str);
void PROGMEMprint(const prog_uchar str[]);
void pushbuffer(byte inStr);
char GetC(int c);
void ConvertC(char* strIn, char* strOut, byte len);
void WifiAuthentication(char* userpass);
void processHTTP();
#endif  // wifi

void pingSerial();

#endif  // __REEFANGEL_WIFI_H__
