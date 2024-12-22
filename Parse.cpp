    #include <WiFi.h>
    
 
#define         WILL 251
#define         WONT 252
#define         DO 253
#define         DONT 254
#define         IAC 255
#define         ECHO 1
#define         SGA 3
void Display_Char(char ch);

        void ParseTelnet(WiFiClient TCPclient)
        {
            while (TCPclient.available())
            {
                int input = TCPclient.read();
                int inputoption,inputverb;
                switch (input)
                {
                    case -1:
                        break;
                    case IAC:
                        // interpret as command
                        inputverb = TCPclient.read();
                        if (inputverb == -1) break;
                        switch (inputverb)
                        {
                            case IAC:
                                //literal IAC = 255 escaped, so append char 255 to string
                                break;
                            case DO:
                            case DONT:
                            case WILL:
                            case WONT:
                                // reply to all commands with "WONT", unless it is SGA (suppres go ahead)
                                inputoption = TCPclient.read();
                                if (inputoption == -1) break;
                                TCPclient.write((byte)IAC);
                                if (inputoption == (int)SGA || inputoption == ECHO)
                                    TCPclient.write(inputverb == DO ? WILL : DO);
                                else
                                    TCPclient.write(inputverb == DO ? WONT : DONT);
                                TCPclient.write(inputoption);
                                break;
                            default:
                                break;
                        }
                        break;
                    default:
                        Serial.print((char)input);
                        Display_Char((char)input);
                        break;
                }
            }
        }

        void RawTelnet(WiFiClient TCPclient)
        {
          while (TCPclient.available()) {
            int input = TCPclient.read();
            Serial.print((char)input);
            Display_Char((char)input);
          }
        }
