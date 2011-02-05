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

#include "ReefAngel_Wifi.h"
#include <ReefAngel_Globals.h>


#ifdef wifi

#include <DS1307RTC.h>
#include <ReefAngel.h>

void WebResponse (const prog_char *response, long strsize)
{
//  P(WebHeaderMsg) = SERVER_HEADER_HTML;
//  printP(WebHeaderMsg);
    P(WebBodyMsg) = SERVER_HEADER_HTML;
    printP(WebBodyMsg);
    Serial.print(strsize,DEC);
    P(WebBodyMsg1) = SERVER_HEADER3;
    printP(WebBodyMsg1);
    printP(response);
}

void printP(const prog_char *str)
{
    char a;
    do
    {
        a=pgm_read_byte(str++);
        if (a!=0) Serial.print(a);
    }
    while (a!=0);
}

void pushbuffer(byte inStr)
{

	m_pushback[m_pushbackindex]=inStr;
	m_pushback[m_pushbackindex+1]=0;
	//memcpy(&m_pushback[0], &m_pushback[1], 31);
	//m_pushback[30]=inStr;
	//m_pushback[31]=0;
	//if (~reqtype)
	//{
	if (reqtype>0 && reqtype<128)
	{
		if (authStr[m_pushbackindex]==inStr) m_pushbackindex++; else m_pushbackindex=0;
		if (authStr[m_pushbackindex]==0) auth=true;
		//if (m_pushbackindex>0) Serial.println(m_pushbackindex,DEC);
		//if (m_pushbackindex>0) Serial.println(test,DEC);
	}
	else
	{
		m_pushbackindex++;
		if (m_pushbackindex==32) m_pushbackindex=0;
		if (reqtype>128)
		{
		    if (inStr==' ')
		    {
		        reqtype=256-reqtype;
		        if ( (reqtype == REQ_M_BYTE) || (reqtype == REQ_M_INT) )
		        {
		        	// if a memory request, verify that the second value was given
		        	if ( m_pushback[m_pushbackindex-2] != ',' )
		        	{
		        		// if the previous value was a comma, then the second value was most likely not given
		        		bHasSecondValue = true;
		        	}
		        }
		    }
		    else if (inStr == ',')
		    {
		    	// when we hit a comma, copy the first value (weboption) to memory location
		    	// then skip over the comma and put the value to be written into weboption
		    	webmemoryloc = weboption;
		    	// reset weboption to 0
		    	weboption = 0;
		    }
		    else
		    {
		    	// consider limiting to only numeric chars (0-9) instead of alphanumeric
		    	//if ( (inStr >= '0') && (inStr <= '9') )
		    	//{
					weboption*=10;
					weboption+=inStr-'0';
		    	//}
		    	/*
		    	else
		    	{
		    		// alpha char, so set weboption to 9 so the relay mask won't process it
		    		// if the value is 0 it will try to set the -1 bit
		    		// if the value is 1 - 8, it will turn off the port
		    		// 9 is not a valid port, so it will just be ignored
					weboption = 9;
		    	}
		    	*/
		    }
		}
		else
		{
            if (strncmp("GET / ", m_pushback, 6)==0) reqtype = REQ_ROOT;
            if (strncmp("GET /wifi", m_pushback, 9)==0) reqtype = REQ_WIFI;
            if (strncmp("GET /r", m_pushback, 6)==0) reqtype = -REQ_RELAY;
            if (strncmp("GET /mb", m_pushback, 7)==0) { reqtype = -REQ_M_BYTE; webmemoryloc = -1; bHasSecondValue = false; }
            if (strncmp("GET /mi", m_pushback, 7)==0) { reqtype = -REQ_M_INT; webmemoryloc = -1; bHasSecondValue = false; }
		}
	}
}

void processHTTP()
{
    bIncoming=true;
    timeout=millis();
    while (bIncoming)
    {
		if (millis()-timeout>100)
		{
			bIncoming=false;
			//for (int a=0;a<32;a++) pushbuffer(0);
		}
		if (Serial.available()>0)
		{
			pushbuffer(Serial.read());
			timeout=millis();
		}
    }
	if (authStr[0]==0) auth=true;
    if (auth)
    {
		//Serial.println(reqtype,DEC);
		auth=false;
		switch ( reqtype )
		{
			case REQ_ROOT:
			{
				P(WebBodyMsg) = SERVER_DEFAULT;
				WebResponse(WebBodyMsg, sizeof(WebBodyMsg) - 1);
				break;
			}  // REQ_ROOT
			case REQ_WIFI:
			{
				P(WebBodyMsg) = SERVER_HEADER_HTML;
				printP(WebBodyMsg);
				Serial.print(sizeof(SERVER_RA) - 1,DEC);
				P(WebBodyMsg1) = SERVER_HEADER3;
				printP(WebBodyMsg1);
				PROGMEMprint(SERVER_RA);
				break;
			}  // REQ_WIFI
			case REQ_RELAY:
			{
				byte o_relay=weboption/10;
				byte o_type=weboption%10;
				if (o_type==0)  // Turn port off
				{
					if ( o_relay < 9 )
					{
						bitClear(ReefAngel.Relay.RelayMaskOn,o_relay-1);
						bitClear(ReefAngel.Relay.RelayMaskOff,o_relay-1);
					}
#ifdef RelayExp
					if ( (o_relay > 10) && (o_relay < 89) )
					{
						byte EID = byte(o_relay/10);
						bitClear(ReefAngel.Relay.RelayMaskOnE[EID-1],(o_relay%10)-1);
						bitClear(ReefAngel.Relay.RelayMaskOffE[EID-1],(o_relay%10)-1);
					}
#endif  // RelayExp
				}
				else if (o_type==1)  // Turn port on
				{
					if ( o_relay < 9 )
					{
						bitSet(ReefAngel.Relay.RelayMaskOn,o_relay-1);
						bitSet(ReefAngel.Relay.RelayMaskOff,o_relay-1);
					}
#ifdef RelayExp
					if ( (o_relay > 10) && (o_relay < 89) )
					{
						byte EID = byte(o_relay/10);
						bitSet(ReefAngel.Relay.RelayMaskOnE[EID-1],(o_relay%10)-1);
						bitSet(ReefAngel.Relay.RelayMaskOffE[EID-1],(o_relay%10)-1);
					}
#endif  // RelayExp
				}
				else if (o_type==2)  // Set port back to Auto
				{
					if ( o_relay < 9 )
					{
						bitClear(ReefAngel.Relay.RelayMaskOn,o_relay-1);
						bitSet(ReefAngel.Relay.RelayMaskOff,o_relay-1);
					}
#ifdef RelayExp
					if ( (o_relay > 10) && (o_relay < 89) )
					{
						byte EID = byte(o_relay/10);
						bitClear(ReefAngel.Relay.RelayMaskOnE[EID-1],(o_relay%10)-1);
						bitSet(ReefAngel.Relay.RelayMaskOffE[EID-1],(o_relay%10)-1);
					}
#endif  // RelayExp
				}
				ReefAngel.Relay.Write();
				char temp[6];
				int s=408;

				s += intlength(ReefAngel.Params.Temp1);
				s += intlength(ReefAngel.Params.Temp2);
				s += intlength(ReefAngel.Params.Temp3);
				s += intlength(ReefAngel.Params.PH);
				s += intlength(ReefAngel.Relay.RelayData);
				s += intlength(ReefAngel.Relay.RelayMaskOn);
				s += intlength(ReefAngel.Relay.RelayMaskOff);
#ifdef RelayExp
				for ( int EID = 0; EID < MAX_RELAY_EXPANSION_MODULES; EID++ )
				{
					s += intlength(ReefAngel.Relay.RelayDataE[EID]);
					s += intlength(ReefAngel.Relay.RelayMaskOnE[EID]);
					s += intlength(ReefAngel.Relay.RelayMaskOffE[EID]);
				}
#endif  // RelayExp
				s += intlength(ReefAngel.LowATO.IsActive());
				s += intlength(ReefAngel.HighATO.IsActive());
				P(WebBodyMsg) = SERVER_HEADER_XML;
				printP(WebBodyMsg);
				Serial.print(s);
				P(WebBodyMsg1) = SERVER_HEADER3;
				printP(WebBodyMsg1);
				ReefAngel.PCLogging();  // print the XML data
				break;
			}  // REQ_RELAY
			case REQ_M_BYTE:
			case REQ_M_INT:
			{
				// webmemoryloc is location
				// weboption is value
				if ( bHasSecondValue && (webmemoryloc >= 0) )
				{
					if ( reqtype == REQ_M_BYTE )
						InternalMemory.write(webmemoryloc, weboption);
					else
						InternalMemory.write_int(webmemoryloc, weboption);
					Serial.print("OK");
				}
				else Serial.print("ERR");
				break;
			}  // REQ_M_BYTE || REQ_M_INT
			default:
				break;
		}  // switch reqtype
    }
    else
    {
		if (reqtype>0)
		{
		  P(WebBodyMsg) = SERVER_DENY;
		  printP(WebBodyMsg);
		}
    }
	Serial.flush();
	m_pushbackindex=0;
    reqtype=0;
    weboption=0;
}

char GetC(int c)
{
	return pgm_read_byte(c+EncodingChars);
}

void ConvertC(char* strIn, char* strOut, byte len)
{
	strOut[0] = GetC(strIn[0]>>2);
	strOut[1] = GetC(((strIn[0]&0x03)<<4)|((strIn[1]&0xf0)>>4));
	strOut[2] = len>1?GetC(((strIn[1]&0x0f)<<2)|((strIn[2]&0xc0)>>6)):'=';
	strOut[3] = len>2?GetC(strIn[2]&0x3f ):'=';
}

void WifiAuthentication(char* userpass)
{
	char* authPtr;
	int len = strlen(userpass);
	int authPtrSize = ((len + 2) / 3) << 2;
	authPtr = (char*)malloc(authPtrSize + 1);
	char* authPtr1 = authPtr;
	while (len > 0)
	{
		ConvertC(userpass, authPtr1, min(len,3));
		authPtr1 += 4;
		userpass += 3;
		len -= 3;
	}
	*(authPtr + authPtrSize) = 0;
	strcpy(authStr,authPtr);
	free(authPtr);
	Serial.println(authStr);
}

#endif  // wifi

void pingSerial()
{
#ifdef wifi
    if ( Serial.available() > 0 ) processHTTP();
#endif  // wifi
}

void PROGMEMprint(const prog_char str[])
{
    char c;
    if(!str) return;
    while((c = pgm_read_byte(str++)))
        Serial.print(c,BYTE);
}

