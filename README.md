**ESP32_Telnet_Terminal**<br>
This simple application is a proof of concept build for the Waveshare 4.3" LCD Display
See: https://www.waveshare.com/wiki/ESP32-S3-Touch-LCD-4.3
It implements a basic green screen terminal display and contains a Telnet client with a minimal Telnet protocol function.
The repo contains the app as an Arduino project and also a .sln file that may be open with Visual Studio with the Visual Micro extension.
The critical ESP32 settings are: USB mode and Upload mode set to USB-OTG and the PSRAM set to OPI mode.
The ESP32 board is set to ESP32S3 DEV MODULE.
The key library requirement is the GFX_Library_For_Arduino see: https://docs.arduino.cc/libraries/gfx-library-for-arduino/.
ESP_Terminal.ino contains an example of how to initialise this particular LCD display.
Parse.cpp contains the minimal Telnet client with a basic Telnet protocol negotiation process.
This in itself will, I hope, be of use as I was unable to find a suitable Telnet protocol interface app anywhere on the web.
Obviously a terminal requires a keyboard and this is achieved by running a paralell serial interface.
Any of the standard serial terminal apps can be used here, I use TeraTerm see: https://github.com/TeraTermProject/teraterm/releases
The font used in the LCD display is built from a monospace TTF font converted to FABGL format using the fontool in the Arduino FABGL library.
I will not go into any further detail here as generating suitable fonts is very involved.
The provided font is crammed into a 10x10 pixel block. As a result, the character spacing is such the some overlap of certain characters can be seen.
This is adequate for test purposes.
The terminal app does not respond to VT100 escape sequnces. These may be disabled in the telnet session using
export TERM=asr33
unset LS_COLORS
With these settings, the display is not too bad and backspace works as well.
I hope you find this of interest.
<br>
Update Dec 2024 <br>
This display app will now work with various versions of vi.<br>
Setting the TERM variable is essential.<br>
For most new Unices use: export TERM=vt52<br>
For old unices eg BSD 2.11 use TERM=vt52; export TERM<br>
The app also transmits ESC[?2l which should change your connected terminal to VT52 mode.<br>
I would note that the original VT52 terminal did not implement some escape codes. See: https://github.com/microsoft/terminal/blob/main/doc/specs/%23976%20-%20VT52%20escape%20sequences.md<br>
Specifically, esc J and esc K are implemented in this app.<br>
<br>
NB: During the development of this app, it has become apparent that the LCD display can interfere with the WIFI such that
a connection to a telnet server becomes very unreliable. After some considerable debugging the following is suggested for the Arduino environment:<br>
1. Boards: ESP32 v 3.11<br>
2. Library: GFX for Arduino v 1.5.2<br>
3. Device: Waveshare ESP32-S3-LCD-4.3<br>
4. Config: Upload mode UART0 / Hardware CDC + USB mode Hardware CDC and JTAG + PSRAM enabled.<br>
#4 are the critical settings. In addition, altering the EXP32 TXPower seems to have an effect as well. Try: iFi.setTxPower(WIFI_POWER_8_5dBm);<br>
And, do not rest the display on an active HDMI cable. This does for the wifi as well!!!!<br>


