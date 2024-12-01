//#define U8G2_USE_LARGE_FONTS
#include <U8g2lib.h>
#include <Arduino_GFX_Library.h>
#include "src/MyFont.h"
#include <WiFi.h>
void ParseTelnet(WiFiClient TCPclient);
#define GFX_DEV_DEVICE WAVESHARE_ESP32_S3_TFT_4_3
#define GFX_BL 2
const char* ssid = "BT-Q6CTR8";     // CHANGE TO YOUR WIFI SSID
const char* password = "c531a3d358"; // CHANGE TO YOUR WIFI PASSWORD
const int serverPort = 23;
IPAddress raspberryIp(192, 168, 1, 110);    // Change to the address of a Raspberry Pi


Arduino_ESP32RGBPanel* rgbpanel = new Arduino_ESP32RGBPanel(
	5 /* DE */,
	3 /* VSYNC */,
	46 /* HSYNC */,
	7 /* PCLK */,

	1 /* R0 */,
	2 /* R1 */,
	42 /* R2 */,
	41 /* R3 */,
	40 /* R4 */,

	39 /* G0 */,
	0 /* G1 */,
	45 /* G2 */,
	48 /* G3 */,
	47 /* G4 */,
	21 /* G5 */,

	14 /* B0 */,
	38 /* B1 */,
	18 /* B2 */,
	17 /* B3 */,
	10 /* B4 */,

	0 /* hsync_polarity */, 40 /* hsync_front_porch */, 48 /* hsync_pulse_width */, 88 /* hsync_back_porch */,
	0 /* vsync_polarity */, 13 /* vsync_front_porch */, 3 /* vsync_pulse_width */, 32 /* vsync_back_porch */,
	1 /* pclk_active_neg */, 16000000 /* prefer_speed */
);

//void GFXdraw16bitBeRGBBitmap(int16_t x1, int16_t y1, uint16_t* full, int16_t w, int16_t h);

Arduino_RGB_Display* gfx = new Arduino_RGB_Display(
	800 /* width */,
	480 /* height */,
	rgbpanel,
	0 /* rotation */,
	true /* auto_flush */
);
uint16_t* fb;
int lnum;
WiFiClient TCPclient;
int sz = 800 * 480 * 2;

void setup(void) {
	Serial.begin(115200);
	Serial.setRxBufferSize(2048);
	while (!Serial);
	// Connect to Wi-Fi
	WiFi.begin(ssid, password);
	while (WiFi.status() != WL_CONNECTED) {
		delay(1000);
		Serial.println("Connecting to WiFi...");
	}
	Serial.println("Connected to WiFi");
	TCPclient.connect(raspberryIp, serverPort);
	// Init Display
#ifdef GFX_EXTRA_PRE_INIT
	GFX_EXTRA_PRE_INIT();
#endif
	if (!gfx->begin()) {
		Serial.println("gfx->begin() failed!");
	}
	gfx->fillScreen(BLACK);
	gfx->setFont(&exportFont);
#ifdef GFX_BL
	pinMode(GFX_BL, OUTPUT);
	digitalWrite(GFX_BL, HIGH);
#endif
	gfx->setTextSize(1, 1);
	gfx->setCursor(10, 20);
	gfx->setTextColor(RED);
	gfx->print("Terminal online");
	delay(1000);  // 5 seconds
	gfx->fillScreen(BLACK);
	fb = gfx->getFramebuffer();
	gfx->setTextColor(DARKGREEN);
	lnum = 1;
	gfx->setCursor(0, (lnum) * 20);
}

void scroll() {
	memcpy(fb, fb + 20 * 800, sz - 20 * 800);
	memset(fb + (20 * 23) * 800, 0, 20 * 800 * 2);
	Cache_WriteBack_Addr((uint32_t)fb, sz);
}

void Display_Char(char ch) {
	int cx;
	switch (ch) {
	case ('\n'):
		if (++lnum == 25) {
			lnum--;
			scroll();
		}
		cx = gfx->getCursorX();
		gfx->setCursor(cx, (lnum) * 20);
		break;
	case ('\r'):
		gfx->setCursor(0, (lnum) * 20);
		break;
	case (7):   // ^g (BEL)
		break;
	case ('\b'):
		cx = gfx->getCursorX();
		if (cx >= 0) {
			gfx->setCursor(cx - 10, (lnum) * 20);
			gfx->fillRect(cx - 10, lnum * 20, 10, -18, BLACK);
		}
		break;
	case ('\t'):
		cx = (gfx->getCursorX() + 80) / 80;
		gfx->setCursor(cx * 80, (lnum) * 20);
		break;
	default:
		cx = gfx->getCursorX();
		if (cx >= 800) {
			gfx->setCursor(cx = 0, (lnum) * 20);
			Display_Char('\n');
		}
		if (ch != ' ')
			gfx->print(ch);
		gfx->setCursor(cx + 10, (lnum) * 20);
		break;
	}
}

void loop() {

	char ch;

	if (!TCPclient.connected()) {
		Serial.println("Connection is disconnected");
		TCPclient.stop();

		// reconnect to TCP server (Arduino #2)
		if (TCPclient.connect(raspberryIp, serverPort)) {
			Serial.println("Reconnected to TCP server");
		}
		else {
			Serial.println("Failed to reconnect to TCP server");
		}
	}

	ParseTelnet(TCPclient);

	if (Serial.available()) {
		ch = Serial.read();
		TCPclient.write(ch);
	}
	yield();
}
