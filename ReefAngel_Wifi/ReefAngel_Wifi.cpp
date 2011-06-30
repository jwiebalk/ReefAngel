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
		        	// must have a comma to have second value
		        	// verify that the last char was a digit
		        	if ( isdigit(m_pushback[m_pushbackindex-2]) )
		        	{
		        		// check for the comma to determine how we proceed
		        		if ( bHasComma )
		        		{
		        			bHasSecondValue = true;
		        		}
		        		else
		        		{
		        			bHasSecondValue = false;
		        			webmemoryloc = weboption;
		        		}
		        	}
		        }
		    }
		    else if (inStr == ',')
		    {
		    	// when we hit a comma, copy the first value (weboption) to memory location
		    	// then skip over the comma and put the value to be written into weboption
		    	bHasComma = true;
		    	webmemoryloc = weboption;
		    	// reset weboption to 0
		    	weboption = 0;
		    }
		    else if(isdigit(inStr))
		    {
		    	// process digits here
				weboption*=10;
				weboption+=inStr-'0';
		    }
		    // 3/14/11 - curt
		    //else all other chars are discarded
		    // consider further sanity checks to ensure that we don't get any malformed strings or commands
		    // right now, we can embed non digits in the parameters and as long as the last char is a digit,
		    // it is ok and the non digits are just skipped over
		    // we may want to signal an error or break out of processing the string to indicate there is an error
		    // maybe set webmemoryloc and weboption to -1 or 0
		    // could also flush the buffer and set reqtype to a REQ_ERROR or something
		    // need to give this more thought
		    //
		    // NOTES about too large of value being stored
		    //   if you exceed the storage limit for the variable, the negative value gets stored
		    //   so we should limit the value being saved from the client side
		    //   otherwise we would have to do additional checks in here for the size and that would
		    //   require more code
		    //   Users shouldn't be manually changing values from the web without an interface that
		    //   limits them, so we "should" be safe (in theory), but this may need to be revisited
		    //   in the future.  - curt (3/14/11)
		}
		else
		{
            if (strncmp("GET / ", m_pushback, 6)==0) reqtype = REQ_ROOT;
            else if (strncmp("GET /wifi", m_pushback, 9)==0) reqtype = REQ_WIFI;
            else if (strncmp("GET /r", m_pushback, 6)==0) reqtype = -REQ_RELAY;
            else if (strncmp("GET /mb", m_pushback, 7)==0) { reqtype = -REQ_M_BYTE; webmemoryloc = -1; bHasSecondValue = false; bHasComma = false;}
            else if (strncmp("GET /mi", m_pushback, 7)==0) { reqtype = -REQ_M_INT; webmemoryloc = -1; bHasSecondValue = false; bHasComma = false;}
            else if (strncmp("GET /ma", m_pushback, 7)==0) reqtype = -REQ_M_ALL;
            else if (strncmp("GET /v", m_pushback, 6)==0) reqtype = -REQ_VERSION;
            else if (strncmp("GET /d", m_pushback, 6)==0) reqtype = -REQ_DATE;
            else reqtype = -REQ_UNKNOWN;
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
/*
				P(WebBodyMsg) = SERVER_HEADER_HTML;
				printP(WebBodyMsg);
				Serial.print(sizeof(SERVER_RA) - 1,DEC);
				P(WebBodyMsg1) = SERVER_HEADER3;
				printP(WebBodyMsg1);
				PROGMEMprint(SERVER_RA);
*/
				WebResponse(SERVER_RA, sizeof(SERVER_RA) - 1);
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
				int s=112;

				s += intlength(ReefAngel.Params.Temp1);
				s += intlength(ReefAngel.Params.Temp2);
				s += intlength(ReefAngel.Params.Temp3);
				s += intlength(ReefAngel.Params.PH);
				s += intlength(ReefAngel.Relay.RelayData);
				s += intlength(ReefAngel.Relay.RelayMaskOn);
				s += intlength(ReefAngel.Relay.RelayMaskOff);
#ifdef RelayExp
				s += 296;
				for ( byte EID = 0; EID < MAX_RELAY_EXPANSION_MODULES; EID++ )
				{
					s += intlength(ReefAngel.Relay.RelayDataE[EID]);
					s += intlength(ReefAngel.Relay.RelayMaskOnE[EID]);
					s += intlength(ReefAngel.Relay.RelayMaskOffE[EID]);
				}
#endif  // RelayExp
				s += 2;  // one digit for each ATO
				P(WebBodyMsg) = SERVER_HEADER_XML;
				printP(WebBodyMsg);
				Serial.print(s, DEC);
				P(WebBodyMsg1) = SERVER_HEADER3;
				printP(WebBodyMsg1);
				ReefAngel.PCLogging();  // print the XML data
				break;
			}  // REQ_RELAY
			case REQ_M_BYTE:
			case REQ_M_INT:
			{
				int s = 118;
				P(WebBodyMsg) = SERVER_HEADER_XML;
				printP(WebBodyMsg);
				Serial.print(s, DEC);
				P(WebBodyMsg1) = SERVER_HEADER3;
				printP(WebBodyMsg1);

				// webmemoryloc is location
				// weboption is value
				if ( bHasSecondValue && (webmemoryloc >= 0) )
				{
					// if we have a second value, we write the value to memory
					if ( reqtype == REQ_M_BYTE )
						InternalMemory.write(webmemoryloc, weboption);
					else
						InternalMemory.write_int(webmemoryloc, weboption);

					// check if we have to reload any timers
					if ( webmemoryloc == Mem_I_FeedingTimer )
					{
						ReefAngel.Timer[0].SetInterval(weboption);
					}
					else if ( webmemoryloc == Mem_I_LCDTimer )
					{
						ReefAngel.Timer[3].SetInterval(weboption);
					}
#ifdef WavemakerSetup
					else if ( (webmemoryloc == Mem_I_WM1Timer) || (webmemoryloc == Mem_I_WM2Timer) )
					{
						// Restart wavemaker timers once they are set
						byte i = 1;
						byte p = ReefAngel.WM1Port;
						if ( webmemoryloc == Mem_I_WM2Timer )
						{
							i = 2;
							p = ReefAngel.WM2Port;
						}
						ReefAngel.Timer[i].Trigger = 0;
						ReefAngel.Timer[i].SetInterval(weboption);
						ReefAngel.Timer[i].Start();
						ReefAngel.Relay.On(p);
					}
#endif  // WavemakerSetup

					PROGMEMprint(XML_M_OPEN);
					Serial.print(webmemoryloc, DEC);
					PROGMEMprint(XML_CLOSE_TAG);
					PROGMEMprint(XML_OK);
					PROGMEMprint(XML_M_CLOSE);
					Serial.print(webmemoryloc, DEC);
					PROGMEMprint(XML_CLOSE_TAG);
				}
				else if ( !bHasSecondValue && (webmemoryloc >= 0) && !bHasComma )
				{
					// no second value and no comma, so we read the value from memory
					PROGMEMprint(XML_M_OPEN);
					Serial.print(webmemoryloc, DEC);
					PROGMEMprint(XML_CLOSE_TAG);
					if ( reqtype == REQ_M_BYTE )
						Serial.print(InternalMemory.read(webmemoryloc),DEC);
					else
						Serial.print(InternalMemory.read_int(webmemoryloc),DEC);
					PROGMEMprint(XML_M_CLOSE);
					Serial.print(webmemoryloc, DEC);
					PROGMEMprint(XML_CLOSE_TAG);
				}
				else
				{
					PROGMEMprint(XML_M_OPEN);
					PROGMEMprint(XML_CLOSE_TAG);
					PROGMEMprint(XML_ERR);
					PROGMEMprint(XML_M_CLOSE);
					PROGMEMprint(XML_CLOSE_TAG);
				}
				break;
			}  // REQ_M_BYTE || REQ_M_INT
			case REQ_M_ALL:
			{
				int s = 123;  // start with the base size of the headers plus the mem tags
				/*
				Send all the data to the client requesting it.  The values will be sent as follows:
					- wrapped in <MEM></MEM> XML tags
					- individual memory values wrapped in their location XML tags
						Memory 800, value 20 - <M800>20</M800>

				An example would be:
					<MEM>
						<M800>20</M800>
						<M801>0</M801>
						<M802>16</M802>
						...
					</MEM>
				*/
				uint8_t offsets[] = {1,1,1,1,1,1,1,1,2,2,1,1,2,2,2,1,1,2,2,2,2,1,2,2,1,1,1,1,1,1,1,1,2,2};
				uint8_t num = sizeof(offsets)/sizeof(uint8_t);
				// add in the memory location sizes, 13 bytes if the memory location is 4 digits
				s += num*13;
				uint16_t count = VarsStart;
				uint8_t x;
				for ( x = 0; x < num; x++ )
				{
					if ( offsets[x] == 1 )
						s += intlength(InternalMemory.read(count));
					else
						s += intlength(InternalMemory.read_int(count));
					count += offsets[x];
				}  // for x

				P(WebBodyMsg) = SERVER_HEADER_XML;
				printP(WebBodyMsg);
				Serial.print(s, DEC);
				P(WebBodyMsg1) = SERVER_HEADER3;
				printP(WebBodyMsg1);
				PROGMEMprint(XML_MEM_OPEN);
				/*
				Loop through all the memory locations starting at VarsStart
				Check the offset for each location to tell us how many bytes to read and how much to increment
				the memory locations.  Currently there are only 1 and 2 byte locations, so when we check
				we just check for 1 and read a byte, otherwise we read 2 bytes (an int)
				Then we increment the memory position by the offset.
				We could be thrown off if somebody accidentally put a value larger than 2 in the offset array
				*/
				for ( x = 0, count = VarsStart; x < num; x++ )
				{
					PROGMEMprint(XML_M_OPEN);
					Serial.print(count,DEC);
					PROGMEMprint(XML_CLOSE_TAG);
					if ( offsets[x] == 1 )
						Serial.print(InternalMemory.read(count),DEC);
					else
						Serial.print(InternalMemory.read_int(count),DEC);
					PROGMEMprint(XML_M_CLOSE);
					Serial.print(count,DEC);
					PROGMEMprint(XML_CLOSE_TAG);
					count += offsets[x];
				}  // for x
				PROGMEMprint(XML_MEM_CLOSE);
				break;
			}  // REQ_M_ALL
			case REQ_VERSION:
			{
				int s = 117;
				s += strlen(ReefAngel_Version);
				P(WebBodyMsg) = SERVER_HEADER_XML;
				printP(WebBodyMsg);
				Serial.print(s, DEC);
				P(WebBodyMsg1) = SERVER_HEADER3;
				printP(WebBodyMsg1);
				Serial.print("<V>"ReefAngel_Version"</V>");
				break;
			}  // REQ_VERSION
			case REQ_DATE:
			{
				break;
			}  // REQ_DATE
			default:
			case REQ_UNKNOWN:
			{
				P(WebBodyMsg) = SERVER_UKNOWN;
				WebResponse(WebBodyMsg, sizeof(WebBodyMsg) - 1);
				break;
			}
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

