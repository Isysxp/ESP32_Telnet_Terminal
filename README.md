**ESP32_Telnet_Terminal**<br>
This simple application is a proof of concept build for the Waveshare 4.3" LCD Display
See: https://www.waveshare.com/wiki/ESP32-S3-Touch-LCD-4.3
It implements a basic green screen terminal display and contains a Telnet client with a minimal Telnet protocol function.
The repo contains the app as an Arduino project and also a .sln file that may be open with Visual Studio with the Visual Micro extension.
The critical ESP32 settings are: USB mode and Ulpload mode set to USB-OTG and the PSRAM set to OPI mode.
The ESP32 board is set to ESP32S3 DEV MODULE.
The key library requirement is the GFX_Library_For_Arduino see: https://docs.arduino.cc/libraries/gfx-library-for-arduino/.
ESP_Terminal.ino contains an example of whow to initialise this particular LCD display.
Parse.cpp contains the minimal Telnet client with a basic Telnet protocol negociation process.
This in itself will, I hope, be of use as I was unable to find a suitable Telnet protocol interface app anywhere on the web.
Obviously a terminal reuires a keyboard and this is achieved by running a paralell serial interface.
Any of the standard serial terminal apps can be used here, I use TeraTerm see: https://github.com/TeraTermProject/teraterm/releases
The font used in the LCD display is built from a monospace TTF font converted to FABGL format using the fontool in the Arduino FABGL library.
I will not go into any further detail here as generating suitable fonts is very involved.
The provided font is crammed into a 10x10 pixel block. As a result, the character spacing is such the some overlap of certaing characters can be seen.
This is adequate for test purposes.
The terminal app does not respond to VT100 escape sequnces. These may be disabled in the telnet session using
export TERM=asr33
unset LS_COLORS
With these settings, the display is not too bad and backspace works as well.
I hope you find this of interest.