/**
 ******************************************************************************
 * @extended SparkPixels.ino:
 *		New mode: DEMO (not available through the SparkPixels app menu)
 *		New mode: TWO COLOR CHASE (reworked POLICE LIGHTS CHASE)
 *		New mode: TWO COLOR WIPE (reworked POLICE LIGHTS WIPE)
 *		Changed mode: RAINBOW BURST (after filling all dots, blank one by one)
 *		Functions: transition(), random_seed_from_cloud(), runMode(),
 * 		modeButton(), smoothSwitch(), resetVariables(), twoColorWipe(),
 * 		twoColorChaser(), cubeGreeting(), runDemo()
 * @author   Werner Moecke
 * @version  V2.0
 * @date     12-October-2015 ~ 25-October-2015
 *
 * @extended SparkPixels.ino - New modes:
 *		ACID DREAM, COLOR BREATHE, COLOR PULSE, COLOR STRIPES, COLOR TRANSITION
 *		FLICKER, POLICE LIGHTS, POLICE LIGHTS CHASER, POLICE LIGHTS WIPE, 
 *		RAINBOW BURST, LISTENER
 * @author   Werner Moecke
 * @version  V1.0.2
 * @date     24-September-2015 ~ 02-October-2015
 *
 * @file     SparkPixels.ino
 * @authors  Kevin Carlborg
 * @version  V1.0.0
 * @date     14-July-2015
 * @brief    Neopixel strip Powered by the Spark Core
 *
 * @extended extra-examples.cpp from NEOPIXEL Library
 * @functions: colorAll,colorWipe,rainbow,rainbowCycle,theaterChase,Wheel 
 * @author   Phil Burgess
 * @version  V0.0.6
 * @date     NA
 ******************************************************************************
 Copyright (c) 2015 Kevin Carlborg  All rights reserved.

 This program is free software; you can redistribute it and/or
 modify it under the terms of the GNU Lesser General Public
 License as published by the Free Software Foundation, either
 version 3 of the License, or (at your option) any later version.

 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 Lesser General Public License for more details.

 You should have received a copy of the GNU Lesser General Public
 License along with this program; if not, see <http://www.gnu.org/licenses/>.
 ******************************************************************************/
 
#include "neopixel/neopixel.h"
#include <math.h>
#define ON                      1
#define OFF                     0

//NEOPIXEL Defines
#define PIXEL_CNT               512
#define PIXEL_PIN               D0
#define PIXEL_TYPE              WS2812B

Adafruit_NeoPixel strip = Adafruit_NeoPixel(PIXEL_CNT, PIXEL_PIN, PIXEL_TYPE);

/* ======================= ADD NEW MODE ID HERE. ======================= */
// Mode ID Defines
const int STANDBY                     = 0;
const int NORMAL                      = 1;
const int COLORALL                    = 2;
const int CHASER                      = 3;
const int ZONE                        = 4;
const int COLORPULSE                  = 5;
const int COLORSTRIPES                = 6;
const int ACIDDREAM                   = 7;
const int COLORWIPE                   = 8;
const int RAINBOW                     = 9;
const int THEATERCHASE                = 10;
const int FROZEN                      = 11;
const int COLLIDE                     = 12;
const int COLORFADE                   = 13;
const int RAINBOW_BURST               = 14;
const int FLICKER                     = 15;
const int COLORBREATHE                = 16;
const int POLICELIGHTS                = 17;
const int TWOCOLORWIPE                = 18;
const int TWOCOLORCHASE               = 19;
const int LISTENER                    = 20;
const int ZONECHASER                  = 21;
const int SPECTRUM                    = 22;

typedef struct modeParams {
   int	modeId;
   char	modeName[64];
   int	numOfColors;       //Tell the android app home many colors to expect. Max number is 6
} modeParams;

/** An RGB color. */
typedef struct Color {
  unsigned char red, green, blue;

  Color(int r, int g, int b) : red(r), green(g), blue(b) {}
  Color() : red(0), green(0), blue(0) {}
} Color;

/** A point in 3D space. */
struct Point {
  float x;
  float y;
  float z;
  Point() : x(0), y(0), z(0) {}
  Point(float _x, float _y, float _z) : x(_x), y(_y), z(_z) {}
};

/** Overloaded != operator. */
bool operator!= (const Color& a, const Color& b) {
    if(a.red != b.red) return true;
    if(a.green != b.green) return true;
    if(a.blue != b.blue) return true;
    return false;
}

bool operator!= (const Point& a, const Point& b) {
    if(a.x != b.x) return true;
    if(a.y != b.y) return true;
    if(a.z != b.z) return true;
    return false;
}

/** Overloaded == operator. */
bool operator== (const Color& a, const Color& b) {
    if(a.red != b.red) return false;
    if(a.green != b.green) return false;
    if(a.blue != b.blue) return false;
    return true;
}

bool operator== (const Point& a, const Point& b) {
    if(a.x != b.x) return false;
    if(a.y != b.y) return false;
    if(a.z != b.z) return false;
    return true;
}

/** Common colors */
const Color black           = Color(0x00, 0x00, 0x00);
const Color grey            = Color(0x92, 0x95, 0x91);
const Color yellow          = Color(0xff, 0xff, 0x14);
const Color incandescent    = Color(0xff, 0xff, 0x9c);    //This seems closer to incandescent color
const Color magenta         = Color(0xc2, 0x00, 0x78);
const Color orange          = Color(0xf9, 0x73, 0x06);
const Color teal            = Color(0x02, 0x93, 0x86);
const Color red             = Color(0xe5, 0x00, 0x00);
const Color brown           = Color(0x65, 0x37, 0x00);
const Color pink            = Color(0xff, 0x81, 0xc0);
const Color blue            = Color(0x03, 0x43, 0xdf);
const Color green           = Color(0x15, 0xb0, 0x1a);
const Color purple          = Color(0x7e, 0x1e, 0x9c);
const Color white           = Color(0xff, 0xff, 0xff);

/* ======================= ADD NEW MODE STRUCT HERE. ======================= */
//modeId and modeName should be the same name to prevent confusion
//Use this struct array to neatly organize and correlate Mode name with number of colors needed
//The Android app uses numOfColors to help populate the view 
//and to know how many colors to ask to update
modeParams modeStruct[] =
{
    /*     modeId       	modeName 		numOfColors
     *     ------------- 	---------------	----------- */
        {  STANDBY,                     "STANDBY",              0   },
        {  NORMAL,                      "INCANDESCENT",         0   },
        {  ACIDDREAM,                   "ACID DREAM",           0   },
        {  SPECTRUM,                    "AUDIO SPECTRUM",	    0   },
        {  COLLIDE,                     "COLLIDE",              0   },
        {  COLORBREATHE,                "COLOR BREATHE",        0   },
        {  COLORPULSE,                  "COLOR PULSE",          0   },
        {  COLORSTRIPES,                "COLOR STRIPES",        0   },
        {  COLORFADE,                   "COLOR TRANSITION",     0   },
        {  COLORWIPE,                   "COLOR WIPE",           0   },
        {  CHASER,                      "CHASER",               1   },
        {  COLORALL,                    "COLOR ALL",            1   },
        {  FLICKER,                     "FLICKER",              1   },
        {  FROZEN,                      "FROZEN",               0   },
        {  LISTENER,                    "LISTENER",             0   },
        {  POLICELIGHTS,                "POLICE LIGHTS",        0   },
        {  RAINBOW,                     "RAINBOW",              0   },
        {  RAINBOW_BURST,               "RAINBOW BURST",        0   },
        {  THEATERCHASE,                "THEATER CHASE",	    0   },
        {  TWOCOLORCHASE,               "TWO COLOR CHASER",     2   },
        {  TWOCOLORWIPE,                "TWO COLOR WIPE",       2   },
        {  ZONE,                        "ZONE",				    4   },
        {  ZONECHASER,                  "ZONE CHASER",			4   }
};

//Preset speed constants
const int speedPresets[] = {120, 100, 80, 70, 50, 30, 20, 10, 1};  //in mSec, slow to fast       

//Temp Sensor constants	
//double refVoltage = 3.3;    //Voltage used as reference for analogRead
//double MINFANTEMP = 100.0;  //Min Temp to turn fan on low speed in Deg Fahrenheit
//double MAXFANTEMP = 115.0;  //Max Temp to turn fan on max speed in Deg Fahrenheit
//double MAXTEMP = 140.0;     //Shut Off Temp in Deg Fahrenheit

//Time Interval constants               hh*mm*ss*ms    
unsigned long oneMinuteInterval =     1*60*1000;	//Read temp every minute
unsigned long twoMinuteInterval =     2*60*1000;	//Change mode every 2 minutes in demo
unsigned long oneHourInterval =      1*60*60*1000;  //auto off in 1 hr when night time
unsigned long oneDayInterval = 		24*60*60*1000;  //time sync interval - 24 hours

//Program Flags
int currentModeID;  //This is the ID of the current mode selected - used in the case statement
bool run;           //Use this for modes that don't need to loop. Set the color, then stop sending commands to the pixels
bool stop;          //Use this to break out of sequence loops when changing to a new mode
bool demo;          //Use this to enable/disable the demo sequence playback 

//Misc variables
int speed; //not to be confused with speedIndex below, this is the local speed (delay) value
unsigned long previousMillis = 0;
unsigned long lastCommandReceived = 0;
unsigned long lastSync = 0;
unsigned long lastModeSet=-twoMinuteInterval;   //Registers last time a mode has been changed in Demo mode
uint32_t defaultColor; //The NORMAL mode color
uint32_t color1;
uint32_t color2;
uint32_t color3;
uint32_t color4;
uint32_t color5;
uint32_t color6;
uint32_t c1, c2;

//Spark Pin Defines
//int FAN_PIN = A0;           //There is a fan in the project box in case it gets too hot in thee=re
//int TEMP_SENSOR_PIN = A7;   //TMP36 sensor on this pin.

//Spark Cloud Variables
int wifi=0;               //used for general info and setup
int tHour=0;              //used for general info and setup
int speedIndex;           //Let the cloud know what speed preset we are using
int brightness;           //How bright do we want these things anyway
//double measuredTemperature = 0.0;   //Let's see how hot our project box is getting since is has the power supply in there
char modeList[622] = "None";           //Holds all mode info comma delimited. Use this to populate the android app
char currentModeName[64] = "None";    //Holds current selected mode
char debug[200];                    //We might want some debug text for development
#define TIME_ZONE_OFFSET	-3		//The offset to obtain your region's time correctly

/* ======================= mode Specific Defines ======================= */
//ZONE mode Start and End Pixels
int zone1Start = 0;
int zone1End   = (PIXEL_CNT / 4) - 1;   //127
int zone2Start = zone1End + 1;          //128
int zone2End   = (zone2Start * 2) - 1;  //255
int zone3Start = PIXEL_CNT / 2;         //256
int zone3End   = zone3Start + zone1End; //383
int zone4Start = zone3End + 1;          //384
int zone4End   = PIXEL_CNT - 1;			//511

//CHASER mode specific Start and End Pixels, re-use some from ZONE mode
//int ChaserZone3Section1End   = 177;
//int chaserZone3Section2Start = 189;
#define CHASER_LENGTH		 PIXEL_CNT

//FROZEN mode defines
int randomFlakes[(int)(PIXEL_CNT*0.1)]; // holds the snowflake positions no more than10% of total number of pixels
Color snowFlakeColor;

/* ========================== LISTENER mode defines ========================== */
UDP Udp;    //an UDP instance to let us receive packets over UDP
//some tpm2.net constants
#define TPM2NET_LISTENING_PORT  65506
#define TPM2NET_HEADER_SIZE     6
#define TPM2NET_HEADER_IDENT    0x9C
#define TPM2NET_CMD_DATAFRAME   0xDA
#define TPM2NET_CMD_COMMAND     0xC0
#define TPM2NET_CMD_ANSWER      0xAC
#define TPM2NET_FOOTER_IDENT    0x36
#define TPM2NET_PACKET_TIMEOUT  0xFA    // 1/4 of a second
#define NR_OF_PANELS            8       // 8x8x8 Cube
#define PIXELS_PER_PANEL        (PIXEL_CNT / NR_OF_PANELS)
#define BPP                     3       // 3 bytes per pixel or 24bit (RGB)
// Package size we expect. The footer byte is not included here!
#define EXPECTED_PACKED_SIZE    (PIXELS_PER_PANEL * BPP + TPM2NET_HEADER_SIZE)

/* ======================= AUDIO SPECTRUM mode defines ======================= */
#define MICROPHONE              12
#define GAIN_CONTROL            11
#define SMOOTH_SW               D2
#define MODE_BT                 D3
#define SIDE                    NR_OF_PANELS // 8
#define TRAIL_LENGTH            50
#define SAMPLES                 2048
#define M                       4   // If the M value changes, then the 'ARRAY_SIZE' constant also needs
#define ARRAY_SIZE              16  // to be changed to reflect the result of the formula: pow(2,M)
#define INPUTLEVEL              63  // This sets the sensitivity for the onboard AGC circuit (0-255); the higher, the more sensitive
bool smooth, dotMode;
float real[ARRAY_SIZE];             //[(int)pow(2,M)]
float imaginary[ARRAY_SIZE];        //[(int)pow(2,M)]
float maxVal=8;

/* ========================= Text Defines / Prototypes ========================= */
char message[80]=" ";
int thickness=0;
void showChar(char a, Point p, Color col);
void marquee(String text, float pos, Color col);
void showMarqueeChar(char a, int pos, Color col);
void showChar(char a, Point origin, Point angle, Color col);
void scrollText(String text, Point initialPosition, Color col);
void scrollSpinningText(String text, Point initialPosition, Color col);
void showChar(char a, Point origin, Point pivot, Point angle, Color col);
/** Font table */
unsigned char fontTable[2048] =
{
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x10, 0x00,	// Char 000 (.)
	0x7E, 0x81, 0xA5, 0x81, 0x9D, 0xB9, 0x81, 0x7E,	// Char 001 (.)
	0x7E, 0xFF, 0xDB, 0xFF, 0xE3, 0xC7, 0xFF, 0x7E,	// Char 002 (.)
	0x6C, 0xFE, 0xFE, 0xFE, 0x7C, 0x38, 0x10, 0x00,	// Char 003 (.)
	0x10, 0x38, 0x7C, 0xFE, 0x7C, 0x38, 0x10, 0x00,	// Char 004 (.)
	0x38, 0x7C, 0x38, 0xFE, 0xFE, 0x10, 0x10, 0x7C,	// Char 005 (.)
	0x00, 0x18, 0x3C, 0x7E, 0xFF, 0x7E, 0x18, 0x7E,	// Char 006 (.)
	0x00, 0x00, 0x18, 0x3C, 0x3C, 0x18, 0x00, 0x00,	// Char 007 (.)
	0xFF, 0xFF, 0xE7, 0xC3, 0xC3, 0xE7, 0xFF, 0xFF,	// Char 008 (.)
	0x00, 0x3C, 0x66, 0x42, 0x42, 0x66, 0x3C, 0x00,	// Char 009 (.)
	0xFF, 0xC3, 0x99, 0xBD, 0xBD, 0x99, 0xC3, 0xFF,	// Char 010 (.)
	0x0F, 0x07, 0x0F, 0x7D, 0xCC, 0xCC, 0xCC, 0x78,	// Char 011 (.)
	0x3C, 0x66, 0x66, 0x66, 0x3C, 0x18, 0x7E, 0x18,	// Char 012 (.)
	0x3F, 0x33, 0x3F, 0x30, 0x30, 0x70, 0xF0, 0xE0,	// Char 013 (.)
	0x7F, 0x63, 0x7F, 0x63, 0x63, 0x67, 0xE6, 0xC0,	// Char 014 (.)
	0x99, 0x5A, 0x3C, 0xE7, 0xE7, 0x3C, 0x5A, 0x99,	// Char 015 (.)
	0x80, 0xE0, 0xF8, 0xFE, 0xF8, 0xE0, 0x80, 0x00,	// Char 016 (.)
	0x02, 0x0E, 0x3E, 0xFE, 0x3E, 0x0E, 0x02, 0x00,	// Char 017 (.)
	0x18, 0x3C, 0x7E, 0x18, 0x18, 0x7E, 0x3C, 0x18,	// Char 018 (.)
	0x66, 0x66, 0x66, 0x66, 0x66, 0x00, 0x66, 0x00,	// Char 019 (.)
	0x7F, 0xDB, 0xDB, 0x7B, 0x1B, 0x1B, 0x1B, 0x00,	// Char 020 (.)
	0x3F, 0x60, 0x7C, 0x66, 0x66, 0x3E, 0x06, 0xFC,	// Char 021 (.)
	0x00, 0x00, 0x00, 0x00, 0x7E, 0x7E, 0x7E, 0x00,	// Char 022 (.)
	0x18, 0x3C, 0x7E, 0x18, 0x7E, 0x3C, 0x18, 0xFF,	// Char 023 (.)
	0x18, 0x3C, 0x7E, 0x18, 0x18, 0x18, 0x18, 0x00,	// Char 024 (.)
	0x18, 0x18, 0x18, 0x18, 0x7E, 0x3C, 0x18, 0x00,	// Char 025 (.)
	0x00, 0x18, 0x0C, 0xFE, 0x0C, 0x18, 0x00, 0x00,	// Char 026 (.)
	0x00, 0x30, 0x60, 0xFE, 0x60, 0x30, 0x00, 0x00,	// Char 027 (.)
	0x00, 0x00, 0xC0, 0xC0, 0xC0, 0xFE, 0x00, 0x00,	// Char 028 (.)
	0x00, 0x24, 0x66, 0xFF, 0x66, 0x24, 0x00, 0x00,	// Char 029 (.)
	0x00, 0x18, 0x3C, 0x7E, 0xFF, 0xFF, 0x00, 0x00,	// Char 030 (.)
	0x00, 0xFF, 0xFF, 0x7E, 0x3C, 0x18, 0x00, 0x00,	// Char 031 (.)
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,	// Char 032 ( )
	0x18, 0x18, 0x18, 0x18, 0x18, 0x00, 0x18, 0x00,	// Char 033 (!)
	0x6C, 0x6C, 0x6C, 0x00, 0x00, 0x00, 0x00, 0x00,	// Char 034 (")
	0x6C, 0x6C, 0xFE, 0x6C, 0xFE, 0x6C, 0x6C, 0x00,	// Char 035 (#)
	0x18, 0x7E, 0xC0, 0x7C, 0x06, 0xFC, 0x18, 0x00,	// Char 036 ($)
	0x00, 0xC6, 0xCC, 0x18, 0x30, 0x66, 0xC6, 0x00,	// Char 037 (%)
	0x38, 0x6C, 0x38, 0x76, 0xDC, 0xCC, 0x76, 0x00,	// Char 038 (&)
	0x30, 0x30, 0x60, 0x00, 0x00, 0x00, 0x00, 0x00,	// Char 039 (')
	0x0C, 0x18, 0x30, 0x30, 0x30, 0x18, 0x0C, 0x00,	// Char 040 (()
	0x30, 0x18, 0x0C, 0x0C, 0x0C, 0x18, 0x30, 0x00,	// Char 041 ())
	0x00, 0x66, 0x3C, 0xFF, 0x3C, 0x66, 0x00, 0x00,	// Char 042 (*)
	0x00, 0x18, 0x18, 0x7E, 0x18, 0x18, 0x00, 0x00,	// Char 043 (+)
	0x00, 0x00, 0x00, 0x00, 0x00, 0x18, 0x18, 0x30,	// Char 044 (,)
	0x00, 0x00, 0x00, 0x7E, 0x00, 0x00, 0x00, 0x00,	// Char 045 (-)
	0x00, 0x00, 0x00, 0x00, 0x00, 0x18, 0x18, 0x00,	// Char 046 (.)
	0x06, 0x0C, 0x18, 0x30, 0x60, 0xC0, 0x80, 0x00,	// Char 047 (/)
	0x7C, 0xCE, 0xDE, 0xF6, 0xE6, 0xC6, 0x7C, 0x00,	// Char 048 (0)
	0x18, 0x38, 0x18, 0x18, 0x18, 0x18, 0x7E, 0x00,	// Char 049 (1)
	0x7C, 0xC6, 0x06, 0x7C, 0xC0, 0xC0, 0xFE, 0x00,	// Char 050 (2)
	0xFE, 0xFE, 0x06, 0x7C, 0x06, 0xFE, 0xFE, 0x00,	// Char 051 (3)
	0x0C, 0xCC, 0xCC, 0xCC, 0xFE, 0x0C, 0x0C, 0x00,	// Char 052 (4)
	0xFE, 0xC0, 0xFC, 0x06, 0x06, 0xC6, 0x7C, 0x00,	// Char 053 (5)
	0x7C, 0xC0, 0xC0, 0xFC, 0xC6, 0xC6, 0x7C, 0x00,	// Char 054 (6)
	0xFE, 0x06, 0x06, 0x0C, 0x18, 0x30, 0x30, 0x00,	// Char 055 (7)
	0x7C, 0xC6, 0xC6, 0x7C, 0xC6, 0xC6, 0x7C, 0x00,	// Char 056 (8)
	0x7C, 0xC6, 0xC6, 0x7E, 0x06, 0x06, 0x7C, 0x00,	// Char 057 (9)
	0x00, 0x18, 0x18, 0x00, 0x00, 0x18, 0x18, 0x00,	// Char 058 (:)
	0x00, 0x18, 0x18, 0x00, 0x00, 0x18, 0x18, 0x30,	// Char 059 (;)
	0x0C, 0x18, 0x30, 0x60, 0x30, 0x18, 0x0C, 0x00,	// Char 060 (<)
	0x00, 0x00, 0x7E, 0x00, 0x7E, 0x00, 0x00, 0x00,	// Char 061 (=)
	0x30, 0x18, 0x0C, 0x06, 0x0C, 0x18, 0x30, 0x00,	// Char 062 (>)
	0x3C, 0x66, 0x0C, 0x18, 0x18, 0x00, 0x18, 0x00,	// Char 063 (?)
	0x7C, 0xC6, 0xDE, 0xDE, 0xDE, 0xC0, 0x7E, 0x00,	// Char 064 (@)
	0x38, 0x6C, 0xC6, 0xC6, 0xFE, 0xC6, 0xC6, 0x00,	// Char 065 (A)
	0xFC, 0xC6, 0xC6, 0xFC, 0xC6, 0xC6, 0xFC, 0x00,	// Char 066 (B)
	0x7C, 0xC6, 0xC0, 0xC0, 0xC0, 0xC6, 0x7C, 0x00,	// Char 067 (C)
	0xF8, 0xCC, 0xC6, 0xC6, 0xC6, 0xCC, 0xF8, 0x00,	// Char 068 (D)
	0xFE, 0xC0, 0xC0, 0xF8, 0xC0, 0xC0, 0xFE, 0x00,	// Char 069 (E)
	0xFE, 0xC0, 0xC0, 0xF8, 0xC0, 0xC0, 0xC0, 0x00,	// Char 070 (F)
	0x7C, 0xC6, 0xC0, 0xC0, 0xCE, 0xC6, 0x7C, 0x00,	// Char 071 (G)
	0xC6, 0xC6, 0xC6, 0xFE, 0xC6, 0xC6, 0xC6, 0x00,	// Char 072 (H)
	0x7E, 0x18, 0x18, 0x18, 0x18, 0x18, 0x7E, 0x00,	// Char 073 (I)
	0x06, 0x06, 0x06, 0x06, 0x06, 0xC6, 0x7C, 0x00,	// Char 074 (J)
	0xC6, 0xCC, 0xD8, 0xF0, 0xD8, 0xCC, 0xC6, 0x00,	// Char 075 (K)
	0xC0, 0xC0, 0xC0, 0xC0, 0xC0, 0xC0, 0xFE, 0x00,	// Char 076 (L)
	0xC6, 0xEE, 0xFE, 0xFE, 0xD6, 0xC6, 0xC6, 0x00,	// Char 077 (M)
	0xC6, 0xE6, 0xF6, 0xDE, 0xCE, 0xC6, 0xC6, 0x00,	// Char 078 (N)
	0x7C, 0xC6, 0xC6, 0xC6, 0xC6, 0xC6, 0x7C, 0x00,	// Char 079 (O)
	0xFC, 0xC6, 0xC6, 0xFC, 0xC0, 0xC0, 0xC0, 0x00,	// Char 080 (P)
	0x7C, 0xC6, 0xC6, 0xC6, 0xD6, 0xDE, 0x7C, 0x06,	// Char 081 (Q)
	0xFC, 0xC6, 0xC6, 0xFC, 0xD8, 0xCC, 0xC6, 0x00,	// Char 082 (R)
	0x7C, 0xC6, 0xC0, 0x7C, 0x06, 0xC6, 0x7C, 0x00,	// Char 083 (S)
	0xFF, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x00,	// Char 084 (T)
	0xC6, 0xC6, 0xC6, 0xC6, 0xC6, 0xC6, 0xFE, 0x00,	// Char 085 (U)
	0xC6, 0xC6, 0xC6, 0xC6, 0xC6, 0x7C, 0x38, 0x00,	// Char 086 (V)
	0xC6, 0xC6, 0xC6, 0xC6, 0xD6, 0xFE, 0x6C, 0x00,	// Char 087 (W)
	0xC6, 0xC6, 0x6C, 0x38, 0x6C, 0xC6, 0xC6, 0x00,	// Char 088 (X)
	0xC6, 0xC6, 0xC6, 0x7C, 0x18, 0x30, 0xE0, 0x00,	// Char 089 (Y)
	0xFE, 0x06, 0x0C, 0x18, 0x30, 0x60, 0xFE, 0x00,	// Char 090 (Z)
	0x3C, 0x30, 0x30, 0x30, 0x30, 0x30, 0x3C, 0x00,	// Char 091 ([)
	0xC0, 0x60, 0x30, 0x18, 0x0C, 0x06, 0x02, 0x00,	// Char 092 (\)
	0x3C, 0x0C, 0x0C, 0x0C, 0x0C, 0x0C, 0x3C, 0x00,	// Char 093 (])
	0x10, 0x38, 0x6C, 0xC6, 0x00, 0x00, 0x00, 0x00,	// Char 094 (^)
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF,	// Char 095 (_)
	0x18, 0x18, 0x0C, 0x00, 0x00, 0x00, 0x00, 0x00,	// Char 096 (`)
	0x00, 0x00, 0x7C, 0x06, 0x7E, 0xC6, 0x7E, 0x00,	// Char 097 (a)
	0xC0, 0xC0, 0xC0, 0xFC, 0xC6, 0xC6, 0xFC, 0x00,	// Char 098 (b)
	0x00, 0x00, 0x7C, 0xC6, 0xC0, 0xC6, 0x7C, 0x00,	// Char 099 (c)
	0x06, 0x06, 0x06, 0x7E, 0xC6, 0xC6, 0x7E, 0x00,	// Char 100 (d)
	0x00, 0x00, 0x7C, 0xC6, 0xFE, 0xC0, 0x7C, 0x00,	// Char 101 (e)
	0x1C, 0x36, 0x30, 0x78, 0x30, 0x30, 0x78, 0x00,	// Char 102 (f)
	0x00, 0x00, 0x7E, 0xC6, 0xC6, 0x7E, 0x06, 0xFC,	// Char 103 (g)
	0xC0, 0xC0, 0xFC, 0xC6, 0xC6, 0xC6, 0xC6, 0x00,	// Char 104 (h)
	0x18, 0x00, 0x38, 0x18, 0x18, 0x18, 0x3C, 0x00,	// Char 105 (i)
	0x06, 0x00, 0x06, 0x06, 0x06, 0x06, 0xC6, 0x7C,	// Char 106 (j)
	0xC0, 0xC0, 0xCC, 0xD8, 0xF8, 0xCC, 0xC6, 0x00,	// Char 107 (k)
	0x38, 0x18, 0x18, 0x18, 0x18, 0x18, 0x3C, 0x00,	// Char 108 (l)
	0x00, 0x00, 0xCC, 0xFE, 0xFE, 0xD6, 0xD6, 0x00,	// Char 109 (m)
	0x00, 0x00, 0xFC, 0xC6, 0xC6, 0xC6, 0xC6, 0x00,	// Char 110 (n)
	0x00, 0x00, 0x7C, 0xC6, 0xC6, 0xC6, 0x7C, 0x00,	// Char 111 (o)
	0x00, 0x00, 0xFC, 0xC6, 0xC6, 0xFC, 0xC0, 0xC0,	// Char 112 (p)
	0x00, 0x00, 0x7E, 0xC6, 0xC6, 0x7E, 0x06, 0x06,	// Char 113 (q)
	0x00, 0x00, 0xFC, 0xC6, 0xC0, 0xC0, 0xC0, 0x00,	// Char 114 (r)
	0x00, 0x00, 0x7E, 0xC0, 0x7C, 0x06, 0xFC, 0x00,	// Char 115 (s)
	0x18, 0x18, 0x7E, 0x18, 0x18, 0x18, 0x0E, 0x00,	// Char 116 (t)
	0x00, 0x00, 0xC6, 0xC6, 0xC6, 0xC6, 0x7E, 0x00,	// Char 117 (u)
	0x00, 0x00, 0xC6, 0xC6, 0xC6, 0x7C, 0x38, 0x00,	// Char 118 (v)
	0x00, 0x00, 0xC6, 0xC6, 0xD6, 0xFE, 0x6C, 0x00,	// Char 119 (w)
	0x00, 0x00, 0xC6, 0x6C, 0x38, 0x6C, 0xC6, 0x00,	// Char 120 (x)
	0x00, 0x00, 0xC6, 0xC6, 0xC6, 0x7E, 0x06, 0xFC,	// Char 121 (y)
	0x00, 0x00, 0xFE, 0x0C, 0x38, 0x60, 0xFE, 0x00,	// Char 122 (z)
	0x0E, 0x18, 0x18, 0x70, 0x18, 0x18, 0x0E, 0x00,	// Char 123 ({)
	0x18, 0x18, 0x18, 0x00, 0x18, 0x18, 0x18, 0x00,	// Char 124 (|)
	0x70, 0x18, 0x18, 0x0E, 0x18, 0x18, 0x70, 0x00,	// Char 125 (})
	0x76, 0xDC, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,	// Char 126 (~)
	0x00, 0x10, 0x38, 0x6C, 0xC6, 0xC6, 0xFE, 0x00,	// Char 127 (.)
	0x7C, 0xC6, 0xC0, 0xC0, 0xC0, 0xD6, 0x7C, 0x30,	// Char 128 (.)
	0xC6, 0x00, 0xC6, 0xC6, 0xC6, 0xC6, 0x7E, 0x00,	// Char 129 (.)
	0x0E, 0x00, 0x7C, 0xC6, 0xFE, 0xC0, 0x7C, 0x00,	// Char 130 (.)
	0x7E, 0x81, 0x3C, 0x06, 0x7E, 0xC6, 0x7E, 0x00,	// Char 131 (.)
	0x66, 0x00, 0x7C, 0x06, 0x7E, 0xC6, 0x7E, 0x00,	// Char 132 (.)
	0xE0, 0x00, 0x7C, 0x06, 0x7E, 0xC6, 0x7E, 0x00,	// Char 133 (.)
	0x18, 0x18, 0x7C, 0x06, 0x7E, 0xC6, 0x7E, 0x00,	// Char 134 (.)
	0x00, 0x00, 0x7C, 0xC6, 0xC0, 0xD6, 0x7C, 0x30,	// Char 135 (.)
	0x7E, 0x81, 0x7C, 0xC6, 0xFE, 0xC0, 0x7C, 0x00,	// Char 136 (.)
	0x66, 0x00, 0x7C, 0xC6, 0xFE, 0xC0, 0x7C, 0x00,	// Char 137 (.)
	0xE0, 0x00, 0x7C, 0xC6, 0xFE, 0xC0, 0x7C, 0x00,	// Char 138 (.)
	0x66, 0x00, 0x38, 0x18, 0x18, 0x18, 0x3C, 0x00,	// Char 139 (.)
	0x7C, 0x82, 0x38, 0x18, 0x18, 0x18, 0x3C, 0x00,	// Char 140 (.)
	0x70, 0x00, 0x38, 0x18, 0x18, 0x18, 0x3C, 0x00,	// Char 141 (.)
	0xC6, 0x10, 0x7C, 0xC6, 0xFE, 0xC6, 0xC6, 0x00,	// Char 142 (.)
	0x38, 0x38, 0x00, 0x7C, 0xC6, 0xFE, 0xC6, 0x00,	// Char 143 (.)
	0x0E, 0x00, 0xFE, 0xC0, 0xF8, 0xC0, 0xFE, 0x00,	// Char 144 (.)
	0x00, 0x00, 0x7F, 0x0C, 0x7F, 0xCC, 0x7F, 0x00,	// Char 145 (.)
	0x3F, 0x6C, 0xCC, 0xFF, 0xCC, 0xCC, 0xCF, 0x00,	// Char 146 (.)
	0x7C, 0x82, 0x7C, 0xC6, 0xC6, 0xC6, 0x7C, 0x00,	// Char 147 (.)
	0x66, 0x00, 0x7C, 0xC6, 0xC6, 0xC6, 0x7C, 0x00,	// Char 148 (.)
	0xE0, 0x00, 0x7C, 0xC6, 0xC6, 0xC6, 0x7C, 0x00,	// Char 149 (.)
	0x7C, 0x82, 0x00, 0xC6, 0xC6, 0xC6, 0x7E, 0x00,	// Char 150 (.)
	0xE0, 0x00, 0xC6, 0xC6, 0xC6, 0xC6, 0x7E, 0x00,	// Char 151 (.)
	0x66, 0x00, 0x66, 0x66, 0x66, 0x3E, 0x06, 0x7C,	// Char 152 (.)
	0xC6, 0x7C, 0xC6, 0xC6, 0xC6, 0xC6, 0x7C, 0x00,	// Char 153 (.)
	0xC6, 0x00, 0xC6, 0xC6, 0xC6, 0xC6, 0xFE, 0x00,	// Char 154 (.)
	0x18, 0x18, 0x7E, 0xD8, 0xD8, 0xD8, 0x7E, 0x18,	// Char 155 (.)
	0x38, 0x6C, 0x60, 0xF0, 0x60, 0x66, 0xFC, 0x00,	// Char 156 (.)
	0x66, 0x66, 0x3C, 0x18, 0x7E, 0x18, 0x7E, 0x18,	// Char 157 (.)
	0xF8, 0xCC, 0xCC, 0xFA, 0xC6, 0xCF, 0xC6, 0xC3,	// Char 158 (.)
	0x0E, 0x1B, 0x18, 0x3C, 0x18, 0x18, 0xD8, 0x70,	// Char 159 (.)
	0x0E, 0x00, 0x7C, 0x06, 0x7E, 0xC6, 0x7E, 0x00,	// Char 160 (.)
	0x1C, 0x00, 0x38, 0x18, 0x18, 0x18, 0x3C, 0x00,	// Char 161 (.)
	0x0E, 0x00, 0x7C, 0xC6, 0xC6, 0xC6, 0x7C, 0x00,	// Char 162 (.)
	0x0E, 0x00, 0xC6, 0xC6, 0xC6, 0xC6, 0x7E, 0x00,	// Char 163 (.)
	0x00, 0xFE, 0x00, 0xFC, 0xC6, 0xC6, 0xC6, 0x00,	// Char 164 (.)
	0xFE, 0x00, 0xC6, 0xE6, 0xF6, 0xDE, 0xCE, 0x00,	// Char 165 (.)
	0x3C, 0x6C, 0x6C, 0x3E, 0x00, 0x7E, 0x00, 0x00,	// Char 166 (.)
	0x3C, 0x66, 0x66, 0x3C, 0x00, 0x7E, 0x00, 0x00,	// Char 167 (.)
	0x18, 0x00, 0x18, 0x18, 0x30, 0x66, 0x3C, 0x00,	// Char 168 (.)
	0x00, 0x00, 0x00, 0xFC, 0xC0, 0xC0, 0x00, 0x00,	// Char 169 (.)
	0x00, 0x00, 0x00, 0xFC, 0x0C, 0x0C, 0x00, 0x00,	// Char 170 (.)
	0xC6, 0xCC, 0xD8, 0x3F, 0x63, 0xCF, 0x8C, 0x0F,	// Char 171 (.)
	0xC3, 0xC6, 0xCC, 0xDB, 0x37, 0x6D, 0xCF, 0x03,	// Char 172 (.)
	0x18, 0x00, 0x18, 0x18, 0x18, 0x18, 0x18, 0x00,	// Char 173 (.)
	0x00, 0x33, 0x66, 0xCC, 0x66, 0x33, 0x00, 0x00,	// Char 174 (.)
	0x00, 0xCC, 0x66, 0x33, 0x66, 0xCC, 0x00, 0x00,	// Char 175 (.)
	0x38, 0x28, 0x28, 0x28, 0x2E, 0x22, 0x3E, 0x00,	// Char 176 (.)
	0x3C, 0x66, 0xC3, 0xA5, 0x99, 0xDB, 0x66, 0x3C,	// Char 177 (.)
	0xFC, 0xCE, 0xC7, 0xC3, 0xC3, 0xC7, 0x0E, 0xFC,	// Char 178 (.)
	0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18,	// Char 179 (.)
	0x18, 0x18, 0x18, 0x18, 0xF8, 0x18, 0x18, 0x18,	// Char 180 (.)
	0x18, 0x18, 0xF8, 0x18, 0xF8, 0x18, 0x18, 0x18,	// Char 181 (.)
	0x36, 0x36, 0x36, 0x36, 0xF6, 0x36, 0x36, 0x36,	// Char 182 (.)
	0x00, 0x00, 0x00, 0x00, 0xFE, 0x36, 0x36, 0x36,	// Char 183 (.)
	0x00, 0x00, 0xF8, 0x18, 0xF8, 0x18, 0x18, 0x18,	// Char 184 (.)
	0x36, 0x36, 0xF6, 0x06, 0xF6, 0x36, 0x36, 0x36,	// Char 185 (.)
	0x36, 0x36, 0x36, 0x36, 0x36, 0x36, 0x36, 0x36,	// Char 186 (.)
	0x00, 0x00, 0xFE, 0x06, 0xF6, 0x36, 0x36, 0x36,	// Char 187 (.)
	0x36, 0x36, 0xF6, 0x06, 0xFE, 0x00, 0x00, 0x00,	// Char 188 (.)
	0x36, 0x36, 0x36, 0x36, 0xFE, 0x00, 0x00, 0x00,	// Char 189 (.)
	0x18, 0x18, 0xF8, 0x18, 0xF8, 0x00, 0x00, 0x00,	// Char 190 (.)
	0x00, 0x00, 0x00, 0x00, 0xF8, 0x18, 0x18, 0x18,	// Char 191 (.)
	0x18, 0x18, 0x18, 0x18, 0x1F, 0x00, 0x00, 0x00,	// Char 192 (.)
	0x18, 0x18, 0x18, 0x18, 0xFF, 0x00, 0x00, 0x00,	// Char 193 (.)
	0x00, 0x00, 0x00, 0x00, 0xFF, 0x18, 0x18, 0x18,	// Char 194 (.)
	0x18, 0x18, 0x18, 0x18, 0x1F, 0x18, 0x18, 0x18,	// Char 195 (.)
	0x00, 0x00, 0x00, 0x00, 0xFF, 0x00, 0x00, 0x00,	// Char 196 (.)
	0x18, 0x18, 0x18, 0x18, 0xFF, 0x18, 0x18, 0x18,	// Char 197 (.)
	0x18, 0x18, 0x1F, 0x18, 0x1F, 0x18, 0x18, 0x18,	// Char 198 (.)
	0x36, 0x36, 0x36, 0x36, 0x37, 0x36, 0x36, 0x36,	// Char 199 (.)
	0x36, 0x36, 0x37, 0x30, 0x3F, 0x00, 0x00, 0x00,	// Char 200 (.)
	0x00, 0x00, 0x3F, 0x30, 0x37, 0x36, 0x36, 0x36,	// Char 201 (.)
	0x36, 0x36, 0xF7, 0x00, 0xFF, 0x00, 0x00, 0x00,	// Char 202 (.)
	0x00, 0x00, 0xFF, 0x00, 0xF7, 0x36, 0x36, 0x36,	// Char 203 (.)
	0x36, 0x36, 0x37, 0x30, 0x37, 0x36, 0x36, 0x36,	// Char 204 (.)
	0x00, 0x00, 0xFF, 0x00, 0xFF, 0x00, 0x00, 0x00,	// Char 205 (.)
	0x36, 0x36, 0xF7, 0x00, 0xF7, 0x36, 0x36, 0x36,	// Char 206 (.)
	0x18, 0x18, 0xFF, 0x00, 0xFF, 0x00, 0x00, 0x00,	// Char 207 (.)
	0x36, 0x36, 0x36, 0x36, 0xFF, 0x00, 0x00, 0x00,	// Char 208 (.)
	0x00, 0x00, 0xFF, 0x00, 0xFF, 0x18, 0x18, 0x18,	// Char 209 (.)
	0x00, 0x00, 0x00, 0x00, 0xFF, 0x36, 0x36, 0x36,	// Char 210 (.)
	0x36, 0x36, 0x36, 0x36, 0x3F, 0x00, 0x00, 0x00,	// Char 211 (.)
	0x18, 0x18, 0x1F, 0x18, 0x1F, 0x00, 0x00, 0x00,	// Char 212 (.)
	0x00, 0x00, 0x1F, 0x18, 0x1F, 0x18, 0x18, 0x18,	// Char 213 (.)
	0x00, 0x00, 0x00, 0x00, 0x3F, 0x36, 0x36, 0x36,	// Char 214 (.)
	0x36, 0x36, 0x36, 0x36, 0xFF, 0x36, 0x36, 0x36,	// Char 215 (.)
	0x18, 0x18, 0xFF, 0x18, 0xFF, 0x18, 0x18, 0x18,	// Char 216 (.)
	0x18, 0x18, 0x18, 0x18, 0xF8, 0x00, 0x00, 0x00,	// Char 217 (.)
	0x00, 0x00, 0x00, 0x00, 0x1F, 0x18, 0x18, 0x18,	// Char 218 (.)
	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,	// Char 219 (.)
	0x00, 0x00, 0x00, 0x00, 0xFF, 0xFF, 0xFF, 0xFF,	// Char 220 (.)
	0xF0, 0xF0, 0xF0, 0xF0, 0xF0, 0xF0, 0xF0, 0xF0,	// Char 221 (.)
	0x0F, 0x0F, 0x0F, 0x0F, 0x0F, 0x0F, 0x0F, 0x0F,	// Char 222 (.)
	0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0x00,	// Char 223 (.)
	0x00, 0x00, 0x76, 0xDC, 0xC8, 0xDC, 0x76, 0x00,	// Char 224 (.)
	0x38, 0x6C, 0x6C, 0x78, 0x6C, 0x66, 0x6C, 0x60,	// Char 225 (.)
	0x00, 0xFE, 0xC6, 0xC0, 0xC0, 0xC0, 0xC0, 0x00,	// Char 226 (.)
	0x00, 0x00, 0xFE, 0x6C, 0x6C, 0x6C, 0x6C, 0x00,	// Char 227 (.)
	0xFE, 0x60, 0x30, 0x18, 0x30, 0x60, 0xFE, 0x00,	// Char 228 (.)
	0x00, 0x00, 0x7E, 0xD8, 0xD8, 0xD8, 0x70, 0x00,	// Char 229 (.)
	0x00, 0x66, 0x66, 0x66, 0x66, 0x7C, 0x60, 0xC0,	// Char 230 (.)
	0x00, 0x76, 0xDC, 0x18, 0x18, 0x18, 0x18, 0x00,	// Char 231 (.)
	0x7E, 0x18, 0x3C, 0x66, 0x66, 0x3C, 0x18, 0x7E,	// Char 232 (.)
	0x3C, 0x66, 0xC3, 0xFF, 0xC3, 0x66, 0x3C, 0x00,	// Char 233 (.)
	0x3C, 0x66, 0xC3, 0xC3, 0x66, 0x66, 0xE7, 0x00,	// Char 234 (.)
	0x0E, 0x18, 0x0C, 0x7E, 0xC6, 0xC6, 0x7C, 0x00,	// Char 235 (.)
	0x00, 0x00, 0x7E, 0xDB, 0xDB, 0x7E, 0x00, 0x00,	// Char 236 (.)
	0x06, 0x0C, 0x7E, 0xDB, 0xDB, 0x7E, 0x60, 0xC0,	// Char 237 (.)
	0x38, 0x60, 0xC0, 0xF8, 0xC0, 0x60, 0x38, 0x00,	// Char 238 (.)
	0x78, 0xCC, 0xCC, 0xCC, 0xCC, 0xCC, 0xCC, 0x00,	// Char 239 (.)
	0x00, 0x7E, 0x00, 0x7E, 0x00, 0x7E, 0x00, 0x00,	// Char 240 (.)
	0x18, 0x18, 0x7E, 0x18, 0x18, 0x00, 0x7E, 0x00,	// Char 241 (.)
	0x60, 0x30, 0x18, 0x30, 0x60, 0x00, 0xFC, 0x00,	// Char 242 (.)
	0x18, 0x30, 0x60, 0x30, 0x18, 0x00, 0xFC, 0x00,	// Char 243 (.)
	0x0E, 0x1B, 0x1B, 0x18, 0x18, 0x18, 0x18, 0x18,	// Char 244 (.)
	0x18, 0x18, 0x18, 0x18, 0x18, 0xD8, 0xD8, 0x70,	// Char 245 (.)
	0x18, 0x18, 0x00, 0x7E, 0x00, 0x18, 0x18, 0x00,	// Char 246 (.)
	0x00, 0x76, 0xDC, 0x00, 0x76, 0xDC, 0x00, 0x00,	// Char 247 (.)
	0x38, 0x6C, 0x6C, 0x38, 0x00, 0x00, 0x00, 0x00,	// Char 248 (.)
	0x00, 0x00, 0x00, 0x18, 0x18, 0x00, 0x00, 0x00,	// Char 249 (.)
	0x00, 0x00, 0x00, 0x00, 0x18, 0x00, 0x00, 0x00,	// Char 250 (.)
	0x0F, 0x0C, 0x0C, 0x0C, 0xEC, 0x6C, 0x3C, 0x1C,	// Char 251 (.)
	0x78, 0x6C, 0x6C, 0x6C, 0x6C, 0x00, 0x00, 0x00,	// Char 252 (.)
	0x7C, 0x0C, 0x7C, 0x60, 0x7C, 0x00, 0x00, 0x00,	// Char 253 (.)
	0x00, 0x00, 0x3C, 0x3C, 0x3C, 0x3C, 0x00, 0x00,	// Char 254 (.)
	0x00, 0x10, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00	// Char 255 (.)
};

/* ========================= RAINBOW BURST mode Defines ========================= */
int idex = 0, ihue = 0; //We define these here as they serve to flag if we need
                        //to blank the cube every time the mode is called
/* ============================ Required Prototypes ============================ */
//int getTemperature(void);
int showPixels(void);
int antipodal_index(int i);
int setNewMode(int newMode);
int getModeIndexFromID(int id);
int isValidMode(String newMode);
int getModeIndexFromName(String name);
void initMicrophone(void);
void background(Color col);
void setPixelColor(Point p, Color col);
void transition(Color bgcolor, bool loop);
void fadeInToColor(uint32_t index, Color col);
void random_seed_from_cloud(unsigned int seed); //Disable random seed from the cloud 
void fadeOutFromColor(uint32_t index, Color col);
void setPixelColor(int x, int y, int z, Color col);
Color getPixelColor(int index);
Color getColorFromInteger(uint32_t col);
uint32_t Wheel(byte WheelPos);
uint32_t getHighestValFromRGB(Color col);
uint32_t colorMap(float val, float minVal, float maxVal);
uint32_t lerpColor(uint32_t c1, uint32_t c2, uint32_t val, uint32_t minVal, uint32_t maxVal);

/* ======================= Spark Pixel Prototypes =============================== */
int colorWipe(uint32_t c);
int colorAll(uint32_t c, bool loop);    /* THIS FUNCTION HAS BEEN DEPRECATED - Use transition() */
int colorZone(uint32_t c1, uint32_t c2, uint32_t c3, uint32_t c4, bool loop);
void FFTJoy(void);
void frozen(void);
void listen(void);
void collide(void);
void rainbow(void);
void runMode(void);
void runDemo(void);
void cycleLerp(void);
void cycleWipe(void);
void color_fade(void);
void colorPulse(void);
void modeButton(void);
void smoothSwitch(void);
void colorStripes(void);
void random_burst(void);
void rainbowCycle(void);
void pulse_oneColorAll(void);
void police_light_strobo(void);
void theaterChaseRainbow(void);
void flicker(uint32_t c);
void colorChaser(uint32_t c);
void resetVariables(int modeIndex);
void findRandomSnowFlakesPositions(int numFlakes);
void twoColorWipe(uint32_t color1, uint32_t color2);
void twoColorChaser(uint32_t color1, uint32_t color2);
void cubeGreeting(int textMode, int frameCount, float pos);
void colorZoneChaser(uint32_t c1, uint32_t c2, uint32_t c3, uint32_t c4);
short FFT(short int dir,int m,float *x,float *y);

/* ============ DISABLED AS IT INTERFERES WITH UDP LISTENING ============
 * Don't connect to the cloud first so we can turn on the lights right away
 * SYSTEM_MODE(SEMI_AUTOMATIC);
 * ====================================================================== */

void setup() {
    Particle.function("SetMode", SetMode);
    Particle.variable("wifi",          &wifi,                  INT);
    Particle.variable("tHour",         &tHour,                 INT);
    Particle.variable("speed",         &speedIndex,            INT);
    Particle.variable("brightness",    &brightness,            INT);
    //Particle.variable("temp",          &measuredTemperature,   DOUBLE);
	Particle.variable("modeList",      modeList,              STRING);
	Particle.variable("mode",          currentModeName,       STRING);
    Particle.variable("debug",         debug,                 STRING);
    
    //pinMode(TEMP_SENSOR_PIN,INPUT);
    pinMode(PIXEL_PIN, OUTPUT);
    //pinMode(FAN_PIN, OUTPUT);
    
    //analogWrite(FAN_PIN, 0); //Turn Fan off
  
	//Initialize
    color1 = 0;
    color2 = 0;
    color3 = 0;
    color4 = 0;
    speedIndex = 5;
    speed = speedPresets[speedIndex];
    brightness = 20 * (255 * .01);	//Scale 0-100 to 0-255;
    run = TRUE;                     //was: FALSE;
    stop = FALSE;
    demo = TRUE;
	currentModeID = getModeIndexFromID(NORMAL);
	defaultColor = strip.Color(incandescent.red, incandescent.green, incandescent.blue);
	snowFlakeColor = getColorFromInteger(0xFFFFFF);
	lastSync = lastCommandReceived = previousMillis = millis(); //Take a time stamp
    
    c1 = Wheel(random(2, 256));
    c2 = Wheel(random(2, 256));

	strip.begin();          //Start up the Neopixels     	
    //colorAll(defaultColor);     //Turn on the NORMAL Mode
    //We are already connected: SYSTEM_MODE is no longer set to SEMI_AUTOMATIC
    //Particle.connect();   //Now connect to the cloud
    Time.zone(TIME_ZONE_OFFSET);    //Set time zone
    
    //Clear the mode list variable
	sprintf(modeList,"");
    //Assemble Spark Cloud available modes variable
    for(int i=0; i<sizeof(modeStruct) / sizeof(modeStruct[0]); i++) {
        char cBuff[20];
        sprintf(cBuff,"%s,%i,",modeStruct[i].modeName, modeStruct[i].numOfColors);
	    strcat(modeList,cBuff);
    }

	//getTemperature();
    tHour = Time.hour();	//used to check for correct time zone
    
    // Start the UDP
    if(Udp.setBuffer(EXPECTED_PACKED_SIZE))
        Udp.begin(TPM2NET_LISTENING_PORT);
    
    // Initialize audio capture
    initMicrophone();
    
    // Set the inputs for the AUDIO SPECTRUM mode
    pinMode(MODE_BT, INPUT_PULLUP);
    pinMode(SMOOTH_SW, INPUT_PULLUP);
    smooth = !digitalRead(SMOOTH_SW);
    dotMode = digitalRead(MODE_BT);
    attachInterrupt(MODE_BT, modeButton, FALLING);
    attachInterrupt(SMOOTH_SW, smoothSwitch, CHANGE);
}

//delay (or speed) is passed 
void loop() {
    if(run) {
		stop = FALSE;
        if(demo) {runDemo();}
		else {runMode();}
    }
    
    unsigned long currentMillis = millis();
 
    if(currentMillis - previousMillis > oneMinuteInterval) {
        previousMillis = currentMillis;
        //getTemperature();
        tHour = Time.hour();

        /*if(measuredTemperature < MINFANTEMP) {
			//Turn fan Off
            analogWrite(FAN_PIN, 0); 
            //Spark.publish("Fan", "OFF", 36000, PRIVATE);
            //Spark.publish("Fan", "Off");
            //Spark.publish("Fan Off", NULL, 36000, PRIVATE);
        }
        else if(measuredTemperature > MINFANTEMP && measuredTemperature <  MAXFANTEMP) {
			//Turn fan on at low speed
            analogWrite(FAN_PIN, 100); 
            //Spark.publish("Fan", "Low", 36000, PRIVATE);
            //Spark.publish("Fan", "Low");
            //Spark.publish("Fan Low", NULL, 36000, PRIVATE);
        }
        else if(measuredTemperature >  MAXFANTEMP) {
			//Turn fan on at full speed
            analogWrite(FAN_PIN, 255); 
            //Spark.publish("Fan", "High", 36000, PRIVATE);
            //Spark.publish("Fan", "High");
           // Spark.publish("Fan High", NULL, 36000, PRIVATE);
        }
        if(measuredTemperature > MAXTEMP) {
			//Turn fan on at full speed and turn Off the lights
            analogWrite(FAN_PIN, 255); 
			setNewMode(getModeIndexFromID(OFF));
        }*/
        //Put other timing stuff in here to speed up main loop
        if (currentMillis - lastSync > oneDayInterval) {
            // Request time time synchronization from the Spark Cloud
            //sprintf(debug,"Last sync time = %i,", (int)(currentMillis - lastSync));
			Particle.syncTime();
            lastSync = currentMillis;
        }
        if (currentMillis - lastCommandReceived > oneHourInterval) {
            //Auto Off Criteria
            //If it's Monday through Friday between 8am and 6pm or between 11pm and 5am any day, turn Off the lights
            if(((Time.weekday() >= 2 && Time.weekday() <=6) && (Time.hour() >= 8 && Time.hour() <= 18)) || (Time.hour() >= 23) || (Time.hour() <= 5)) {
                //No one is home or everyone is sleeping. So shut it down
				//sprintf(debug,"Last auto Off time = %i,", (int)(currentMillis - lastCommandReceived));
				lastCommandReceived = currentMillis;
				wifi = WiFi.RSSI();
				setNewMode(getModeIndexFromID(STANDBY));
                run = TRUE;
                demo = FALSE;
            }
        }
    }
}

/*int getTemperature(void){
    double measuredVoltage = refVoltage * analogRead(TEMP_SENSOR_PIN) / 4095.0;
    measuredTemperature = (measuredVoltage - 0.5) * 100.0;          //Celcius Temp
    measuredTemperature = (measuredTemperature * 9.0 / 5.0) + 32.0; //Convert to Fahrenheit

    return 1;
}*/

//Disable random seed from the cloud
void random_seed_from_cloud(unsigned int seed) {
   // don't do anything with this. Continue with existing seed.
}

void runDemo() {
    static int textMode=0, frameCount=0, previousModeID=currentModeID;
    static int endOfMessage=0, cycleCount=-1;
    static bool isTextDone=true;
    static float pos=0, posInc=.2;
    run = TRUE;

    if(stop || !demo) {run = TRUE; demo = FALSE; return;}

    if(isTextDone && (millis() - lastModeSet > twoMinuteInterval)) {
        color1 = Wheel(random(2, 256));
        color2 = Wheel(random(2, 256));
        color3 = Wheel(random(2, 256));
        color4 = Wheel(random(2, 256));
        
        cycleCount++;
        if(cycleCount >= (sizeof(modeStruct) / sizeof(modeStruct[0]))-4) {
            srand(analogRead(MICROPHONE));
            cycleCount = 0;
            textMode = 0;
        }

        while((currentModeID == previousModeID) || 
              (currentModeID == getModeIndexFromID(COLORALL)) || 
              (currentModeID == getModeIndexFromID(NORMAL)) || 
              (currentModeID == getModeIndexFromID(STANDBY)) || 
              (currentModeID == getModeIndexFromID(LISTENER))) {
            
            int i = random((int)(sizeof(modeStruct) / sizeof(modeStruct[0])));
            currentModeID = modeStruct[i].modeId;
        }
        previousModeID = currentModeID;
        setNewMode(currentModeID);

		frameCount = 0;
        isTextDone = FALSE;
        transition(black, true);
    }
    
    while(!isTextDone) {
        if(stop || !demo) {run = TRUE; demo = FALSE; return;}
        if(textMode <= 2) {endOfMessage = 13;}
        if(textMode == 3) {endOfMessage = SIDE;}
        if(textMode >= 4) {endOfMessage = 1.25 * SIDE;}
        cubeGreeting(textMode, frameCount, pos);
        
        frameCount++;
        pos += posInc;
        if (pos >= (strlen(message)*endOfMessage)) {
            pos = 0;
            textMode++;
            if(textMode > 5) {
                if(cycleCount == floor((sizeof(modeStruct) / sizeof(modeStruct[0]))*.25)-1)
                    textMode = 0;
                else 
                    textMode = 4;
                isTextDone = TRUE;
                //We reset the mode cycle timer here for the scrolling text
                //not to interfere with the timeout count and vice-versa.
                lastModeSet = millis();
            } 
            if(textMode <= 4) {
                delay(800);
                transition(black, true);
            }
        }
    }
    runMode();
}

void runMode() {
    switch (currentModeID) {
        case STANDBY:
		    transition(black, true);  //colorAll(0, demo);
		    break;
     	case COLORALL:
	        transition(getColorFromInteger(color1), false);  //colorAll(color1, demo);
	        break;
     	case COLORBREATHE:
	        pulse_oneColorAll();
	        break;
     	case CHASER:
		    colorChaser(color1);
			break;
		case ZONE:
	        colorZone(color1, color2, color3, color4, demo); 
	        break;
		case ZONECHASER:
	        colorZoneChaser(color1, color2, color3, color4); 
	        break;
		case COLORPULSE:
		    colorPulse();
		    break;
		case COLORSTRIPES:
		    colorStripes();
		    break;
		case ACIDDREAM:
		    cycleLerp();
		    break;    
		case COLORWIPE:
		    cycleWipe();
		    break;    
		case RAINBOW:
		    rainbowCycle();
		    break;
		case SPECTRUM:
		    FFTJoy();
		    break;
		case THEATERCHASE:
		    theaterChaseRainbow();
		    break;
		case FROZEN:
		    frozen();
		    break;
		case LISTENER:
		    listen();
		    break;
		case COLLIDE:
		    collide();
		    break;
		case POLICELIGHTS:
		    police_light_strobo();
		    break;
		case TWOCOLORWIPE:
		    twoColorWipe(color1, color2);
		    break;
		case TWOCOLORCHASE:
		    twoColorChaser(color1, color2);
		    break;
		case COLORFADE:
		    color_fade();
		    break;
		case RAINBOW_BURST:
		    random_burst();
		    break;
     	case FLICKER:
		    flicker(color1);
			break;
		case NORMAL:
		default:
		    transition(incandescent, false);    //colorAll(defaultColor, demo);
			break;
    }    
}

void cubeGreeting(int textMode, int frameCount, float pos) {
    background(black);
    switch(textMode) {
        case(0):
            sprintf(message, " ");
            thickness = 0;
            showChar(0xB0, Point(ceil((SIDE-1)*.5), ceil((SIDE-1)*.5), ceil((SIDE-1)*.5)), Point(floor((SIDE-1)*.5), ceil((SIDE-1)*.5), ceil((SIDE-1)*.5)), Point(0, pos, 0), getColorFromInteger(Wheel(frameCount%500)));
            break;  
        case(1):
            sprintf(message, " ");
            thickness = 0;
            showChar(0x33, Point(1, ceil((SIDE-1)*.5), ceil((SIDE-1)*.5)), Point(0, ceil((SIDE-1)*.5), 0), Point(pos, 0, 0), getColorFromInteger(Wheel(frameCount%500)));
            break;  
        case(2):
            sprintf(message, " ");
            thickness = 0;
            showChar(0xB2, Point(ceil((SIDE-1)*.5), ceil((SIDE-1)*.5), ceil((SIDE-1)*.5)), Point(floor((SIDE-1)*.5), ceil((SIDE-1)*.5), ceil((SIDE-1)*.5)), Point(0, pos, 0), getColorFromInteger(Wheel(frameCount%500)));
            break;  
        case(3):
            thickness = 1;
            sprintf(message, "Hello from %c%c%c %c ", 0xB0, 0x33, 0xB2, 0xB1);
            marquee(message, pos, getColorFromInteger(Wheel(frameCount%500)));
            break;  
        case(4):
            thickness = 1;
            sprintf(message, "Select a mode from the SparkPixels app      Now showcasing...");
            scrollText(message, Point(pos - strlen(message), 0, 6), getColorFromInteger(Wheel(frameCount%500)));
            break;  
        case(5):
            thickness = 1;
            sprintf(message, currentModeName);
            scrollSpinningText(message, Point(pos - strlen(message), 0, ceil((SIDE-1)*.5)), getColorFromInteger(Wheel(frameCount%500)));
            break;  
    }
    showPixels();
}

//Used to make smooth transitions between colors; we give it the color we
//want to get to, and it figures out how to make it happen by estimating the
//maximum color level achievable from the given color (if that makes sense)
// @loop: always FALSE when fading IN to a color; TRUE when fading to black.
void transition(Color bgcolor, bool loop) {
    uint32_t maxColorPixel = getHighestValFromRGB(bgcolor);
    uint32_t top = maxColorPixel > 0 ? maxColorPixel : 0xFF;
    Color col2;
    run = loop;
    
    for(int i=0; i<=top; i+=top*.125) {
        for(int index = 0; index < strip.numPixels(); index++) {
            if(maxColorPixel > 0) {
                //Fade in to color
                if(i < bgcolor.red) col2.red = i;
                if(i < bgcolor.green) col2.green = i;
                if(i < bgcolor.blue) col2.blue = i;
            }
            else {
                //Fade out from color
                col2 = getPixelColor(index);
                if(col2.red > 0) col2.red -= col2.red*.125;
                if(col2.green > 0) col2.green -= col2.green*.125;
                if(col2.blue > 0) col2.blue -= col2.blue*.125;
            }
            strip.setPixelColor(index, strip.Color(col2.red, col2.green, col2.blue));
        }
        if(stop) {demo = FALSE; return;}
        showPixels();
        delay(speed);
    }
}

/** Set the entire cube to one color.
  @param col The color to set all LEDs in the cube to.
*/
void background(Color col) {
    for(int index = 0; index < strip.numPixels(); index++)
        strip.setPixelColor(index, strip.Color(col.red, col.green, col.blue));
}

//Used in all modes to set the brightness, show the lights, process Spark events and delay
int showPixels(void) {
	strip.setBrightness(brightness);
    strip.show();
    Particle.process();    //process Spark events
	
	return 1;
}

// Set all pixels in the strip to a solid color
/* THIS FUNCTION HAS BEEN DEPRECATED - Use 'void transition(Color bgcolor, bool loop)' */
int colorAll(uint32_t c, bool loop) {
    if(c > 0) {
        Color c2, c1 = getColorFromInteger(c);
        uint32_t maxColorPixel = getHighestValFromRGB(c1);
        
        for(int i=0; i<=maxColorPixel; i+=(maxColorPixel*.05)) {
                if(i <= c1.red) c2.red = i;
                if(i <= c1.green) c2.green = i;
                if(i <= c1.blue) c2.blue = i;
                for(int j=0; j<strip.numPixels(); j++)
                    strip.setPixelColor(j, strip.Color(c2.red, c2.green, c2.blue));
            if(stop) {demo = FALSE; return 0;}
            if(demo) {if(millis() - lastModeSet > twoMinuteInterval) {return 0;}}
                showPixels();
            }
    }
    else {
        Color c2, c1;

        for(int i=0; i<strip.numPixels(); i++) {
            c = strip.getPixelColor(i);
            if(c > 0) {
                c1 = getColorFromInteger(c);
                break;
            }
        }
        if(c > 0) {
            uint32_t maxColorPixel = getHighestValFromRGB(c1);

            for(int i=maxColorPixel; i>=0; i-=(maxColorPixel*.05)) {
                if(i <= c1.red) c2.red = i;
                if(i <= c1.green) c2.green = i;
                if(i <= c1.blue) c2.blue = i;
                for(int j=0; j<strip.numPixels(); j++)
                    strip.setPixelColor(j, strip.Color(c2.red, c2.green, c2.blue));
                if(stop) {demo = FALSE; return 0;}
                if(demo) {if(millis() - lastModeSet > twoMinuteInterval) {return 0;}}
                showPixels();
            }
            //Ensure every pixel has been fully blanked
            for(int i=0; i<strip.numPixels(); i++)
                strip.setPixelColor(i, 0);
            if(stop) {demo = FALSE; return 0;}
            if(demo) {if(millis() - lastModeSet > twoMinuteInterval) {return 0;}}
            showPixels();
        }
        /*else {
            for(int i=0; i<strip.numPixels(); i++)
                strip.setPixelColor(i, 0);
            showPixels();
        }*/
    }
    return 1;
}

void colorChaser(uint32_t c) {
    static int idex = 0;
    static bool bounce = false;
    Color c1, c2 = getColorFromInteger(c);
    uint32_t maxColorPixel = getHighestValFromRGB(c2);
    uint32_t increment = map(speed, 1, 120, maxColorPixel*.25, 5);
	run = TRUE;

    for(int i=0; i<=0xFF; i+=increment) {
        //Slowly fade in the currently indexed pixel to the chosen color
        if(i <= c2.red) c1.red = i;
        if(i <= c2.green) c1.green = i;
        if(i <= c2.blue) c1.blue = i;
        strip.setPixelColor(idex, strip.Color(c1.red, c1.green, c1.blue));

        //Then slowly fade out previously-lit pixels to black, leaving a nice "trailing" effect
        for(int j=0; j<PIXEL_CNT; j++) {
            if (j != idex) {
                Color pixelColor = getColorFromInteger(strip.getPixelColor(j));
                if(pixelColor.red > 0) pixelColor.red-=pixelColor.red*.125;
                if(pixelColor.green > 0) pixelColor.green-=pixelColor.green*.125;
                if(pixelColor.blue > 0) pixelColor.blue-=pixelColor.blue*.125;
                strip.setPixelColor(j, strip.Color(pixelColor.red, pixelColor.green, pixelColor.blue));
            }
        }
        if(stop) {demo = FALSE; return;}
        if(demo) {if(millis() - lastModeSet > twoMinuteInterval) {return;}}
        showPixels();
        delay(speed);
    }
    
    if(bounce) {idex--;} else {idex++;}
    
    if (idex <= 0) {
        idex = 0;
        bounce = false;
    }
    if (idex >= PIXEL_CNT) {
        idex = PIXEL_CNT-2;
        bounce = true;
    }
}

int colorZone(uint32_t c1, uint32_t c2, uint32_t c3, uint32_t c4, bool loop) {
    uint32_t maxColorPixel, increment;
    static Color col1 = getColorFromInteger(c1);
    static Color col2 = getColorFromInteger(c2);
    static Color col3 = getColorFromInteger(c3);
    static Color col4 = getColorFromInteger(c4);
    static Color c;
    run = loop;
    
    maxColorPixel = getHighestValFromRGB(col1);
    increment = map(speed, 1, 120, maxColorPixel*.25, 5);
    for(int j=0; j<=maxColorPixel; j+=increment) {
        if(stop) {demo = FALSE; run = TRUE; return 0;}
        if(demo) {if(millis() - lastModeSet > twoMinuteInterval) {run = TRUE; return 0;}}
        if(run || (c != col1)) {
            if(j <= col1.red) c.red = j;
            if(j <= col1.green) c.green = j;
            if(j <= col1.blue) c.blue = j;
            for(int i=zone1Start; i<=zone1End; i++)
                strip.setPixelColor(i, strip.Color(c.red, c.green, c.blue));
            showPixels();
            delay(speed);
        }
    }
    maxColorPixel = getHighestValFromRGB(col2);
    increment = map(speed, 1, 120, maxColorPixel*.25, 5);
    for(int j=0; j<=maxColorPixel; j+=increment) {
        if(stop) {demo = FALSE; run = TRUE; return 0;}
        if(demo) {if(millis() - lastModeSet > twoMinuteInterval) {run = TRUE; return 0;}}
        if(run || (c != col2)) {
            if(j <= col2.red) c.red = j;
            if(j <= col2.green) c.green = j;
            if(j <= col2.blue) c.blue = j;
            for(int i=zone2Start; i<=zone2End; i++)
                strip.setPixelColor(i, strip.Color(c.red, c.green, c.blue));
            showPixels();
            delay(speed);
        }
    }
    maxColorPixel = getHighestValFromRGB(col3);
    increment = map(speed, 1, 120, maxColorPixel*.25, 5);
    for(int j=0; j<=maxColorPixel; j+=increment) {
        if(stop) {demo = FALSE; run = TRUE; return 0;}
        if(demo) {if(millis() - lastModeSet > twoMinuteInterval) {run = TRUE; return 0;}}
        if(run || (c != col3)) {
            if(j <= col3.red) c.red = j;
            if(j <= col3.green) c.green = j;
            if(j <= col3.blue) c.blue = j;
            for(int i=zone3Start; i<=zone3End; i++)
                strip.setPixelColor(i, strip.Color(c.red, c.green, c.blue));
            showPixels();
            delay(speed);
        }
    }
    maxColorPixel = getHighestValFromRGB(col4);
    increment = map(speed, 1, 120, maxColorPixel*.25, 5);
    for(int j=0; j<=maxColorPixel; j+=increment) {
        if(stop) {demo = FALSE; run = TRUE; return 0;}
        if(demo) {if(millis() - lastModeSet > twoMinuteInterval) {run = TRUE; return 0;}}
        if(run || (c != col4)) {
            if(j <= col4.red) c.red = j;
            if(j <= col4.green) c.green = j;
            if(j <= col4.blue) c.blue = j;
            for(int i=zone4Start; i<=zone4End; i++)
                strip.setPixelColor(i, strip.Color(c.red, c.green, c.blue));
            showPixels();
            delay(speed);
        }
    }
    return 1;
}

//Creates 4 color zones, with each one having its own "chaser pixel"
void colorZoneChaser(uint32_t c1, uint32_t c2, uint32_t c3, uint32_t c4) {
    static int idexZone1 = random(zone1Start, zone1End+1);  //zone1Start + (rand() % zone1End);
    static int idexZone2 = random(zone2Start, zone2End+1);  //zone2Start + (rand() % zone2End);
    static int idexZone3 = random(zone3Start, zone3End+1);  //zone3Start + (rand() % zone3End);
    static int idexZone4 = random(zone4Start, zone4End+1);  //zone4Start + (rand() % zone4End);
    static bool bounce1 = false;
    static bool bounce2 = false;
    static bool bounce3 = false;
    static bool bounce4 = false;
    Color col1 = getColorFromInteger(c1);
    Color col2 = getColorFromInteger(c2);
    Color col3 = getColorFromInteger(c3);
    Color col4 = getColorFromInteger(c4);
    Color colZ1, colZ2, colZ3, colZ4;
    uint32_t maxColorPixel = max(max(getHighestValFromRGB(col1), getHighestValFromRGB(col2)), max(getHighestValFromRGB(col3), getHighestValFromRGB(col4)));
    uint32_t increment = map(speed, 1, 120, maxColorPixel*.25, 5);
	run = TRUE;
    
    for(int i=0; i<=maxColorPixel; i+=increment) {
        //Slowly fade in the currently indexed pixels
        if(i <= col1.red) colZ1.red = i;
        if(i <= col1.green) colZ1.green = i;
        if(i <= col1.blue) colZ1.blue = i;
        if(i <= col2.red) colZ2.red = i;
        if(i <= col2.green) colZ2.green = i;
        if(i <= col2.blue) colZ2.blue = i;
        if(i <= col3.red) colZ3.red = i;
        if(i <= col3.green) colZ3.green = i;
        if(i <= col3.blue) colZ3.blue = i;
        if(i <= col4.red) colZ4.red = i;
        if(i <= col4.green) colZ4.green = i;
        if(i <= col4.blue) colZ4.blue = i;
        strip.setPixelColor(idexZone1, strip.Color(colZ1.red, colZ1.green, colZ1.blue));
        strip.setPixelColor(idexZone2, strip.Color(colZ2.red, colZ2.green, colZ2.blue));
        strip.setPixelColor(idexZone3, strip.Color(colZ3.red, colZ3.green, colZ3.blue));
        strip.setPixelColor(idexZone4, strip.Color(colZ4.red, colZ4.green, colZ4.blue));
        
        //Then slowly fade out previously-lit pixels to black, leaving a nice "trailing" effect
        for(int j=0; j<PIXEL_CNT; j++) {
            if ((j != idexZone1) && (j != idexZone2) && (j != idexZone3) && (j != idexZone4)) {
                Color pixelColor = getColorFromInteger(strip.getPixelColor(j));
                if(pixelColor.red > 0) pixelColor.red-=pixelColor.red*.125;
                if(pixelColor.green > 0) pixelColor.green-=pixelColor.green*.125;
                if(pixelColor.blue > 0) pixelColor.blue-=pixelColor.blue*.125;
                strip.setPixelColor(j, strip.Color(pixelColor.red, pixelColor.green, pixelColor.blue));
            }
        }
        if(stop) {demo = FALSE; return;}
        if(demo) {if(millis() - lastModeSet > twoMinuteInterval) {return;}}
        showPixels();
        delay(speed);
    }
  
    //Check direction
    if(bounce1) {idexZone1--;} else {idexZone1++;}
    if(bounce2) {idexZone2--;} else {idexZone2++;}
    if(bounce3) {idexZone3--;} else {idexZone3++;}
    if(bounce4) {idexZone4--;} else {idexZone4++;}
    
    //Check beginning-of-trail
    if (idexZone1 <= zone1Start) {
        idexZone1 = zone1Start;
        bounce1 = false;
    }
    if (idexZone2 <= zone2Start) {
        idexZone2 = zone2Start;
        bounce2 = false;
    }
    if (idexZone3 <= zone3Start) {
        idexZone3 = zone3Start;
        bounce3 = false;
    }
    if (idexZone4 <= zone4Start) {
        idexZone4 = zone4Start;
        bounce4 = false;
    }
    
    //Check end-of-trail
    if (idexZone1 >= zone1End) {
        idexZone1 = zone1End;
        bounce1 = true;
    }
    if (idexZone2 >= zone2End) {
        idexZone2 = zone2End;
        bounce2 = true;
    }
    if (idexZone3 >= zone3End) {
        idexZone3 = zone3End;
        bounce3 = true;
    }
    if (idexZone4 >= zone4End-1) {
        idexZone4 = zone4End-1;
        bounce4 = true;
    }
}

//Fade through colors over all LEDs
void color_fade() { //OK //-FADE ALL LEDS THROUGH HSV RAINBOW
   static int ihue = -1;
   run = TRUE;

   ihue++;
   if (ihue > 255) {
      ihue = 0;
   }
   for(int idex = 0 ; idex < PIXEL_CNT; idex++ ) {
      strip.setPixelColor(idex, Wheel(ihue));
   }
    if(stop) {demo = FALSE; return;}
    if(demo) {if(millis() - lastModeSet > twoMinuteInterval) {return;}}
   showPixels();
   delay(speed);
}

//Flicker effect - random flashing of all LEDs
void flicker(uint32_t c) {
    Color c2, c1 = getColorFromInteger(c);
    int ibright = random(brightness*.25, brightness+1);  //(brightness*.25) + (rand() % brightness);
    int random_delay = random(1, map(speed, 1, 120, 100, 10)+1);  //1 + (rand() % map(speed, 1, 120, 100, 10));
    uint32_t maxColorPixel = floor(max(ibright, getHighestValFromRGB(c1)));
    uint32_t increment = map(speed, 1, 120, maxColorPixel*.25, 5);
	run = TRUE;

    for(int j=random((int)(maxColorPixel*.25), (int)(maxColorPixel*.5)+1); j<=maxColorPixel; j+=increment) {
        for(int i = 0; i < PIXEL_CNT; i++ ) {
            if(j <= c1.red) c2.red = j;
            if(j <= c1.green) c2.green = j;
            if(j <= c1.blue) c2.blue = j;
            strip.setPixelColor(i, strip.Color(c2.red, c2.green, c2.blue));
        }
        if(stop) {demo = FALSE; return;}
        if(demo) {if(millis() - lastModeSet > twoMinuteInterval) {return;}}
        strip.show();
        Particle.process();    //process Spark events
        delay(random_delay);
    }
}

//Fade in/out a color using brightness/saturation
void pulse_oneColorAll() { //-PULSE BRIGHTNESS ON ALL LEDS TO ONE COLOR 
    static int ival = 0;
    static uint32_t xhue = Wheel(random(256));
    static int bouncedirection = 0;
    float isteps = constrain(brightness*.03, .6, brightness*.25);
	run = TRUE;
    
    if (bouncedirection == 0) {
    ival += isteps;
    if (ival >= brightness)
        bouncedirection = 1;
    }
    if (bouncedirection == 1) {
        ival -= isteps;
        if (ival <= 0) {
            bouncedirection = 0;
            xhue = Wheel(random(256));
        }
    }
    for(int i = 0; i < PIXEL_CNT; i++) {
        strip.setPixelColor(i, xhue);
        strip.setBrightness(ival);
    }
  
    if(stop) {demo = FALSE; return;}
    if(demo) {if(millis() - lastModeSet > twoMinuteInterval) {return;}}
    strip.show();
    Particle.process();    //process Spark events
    delay(speed);
}

//Police light - red/blue strobo on each half of stripe
void police_light_strobo() {
    int middle = zone2End;
    static int color = 0;  
    static int left_right = 0;
	run = TRUE;
    
    if (left_right > 19)
        left_right = 0;
    
    if (color == 1)
        color = 0;
    else
        color = 1;
    
    for (int i = 0; i < PIXEL_CNT; i++) {
        if (i <= middle && left_right < 10) {
            if (color)
                strip.setPixelColor(i, strip.Color(0, 0, 255));
            else
                strip.setPixelColor(i, 0);
        }
        else if (i > middle && left_right >= 10) {
            if (color)
                strip.setPixelColor(i, strip.Color(255, 0, 0));
            else
                strip.setPixelColor(i, 0);
        }
        if(stop) {demo = FALSE; return;}
        if(demo) {if(millis() - lastModeSet > twoMinuteInterval) {return;}}
    }
    
    if(stop) {demo = FALSE; return;}
    if(demo) {if(millis() - lastModeSet > twoMinuteInterval) {return;}}
    showPixels();
    delay(speed);
    left_right++;
}

//Dual wipe all LEDs 
void twoColorWipe(uint32_t color1, uint32_t color2) { //-COLOR WIPE (TWO COLOR SOLID)
    static int idex1 = random(zone1Start, zone2End+1);
    static int idex2 = random(zone3Start, zone4End+1);
    Color col1, c1 = getColorFromInteger(color1);
    Color col2, c2 = getColorFromInteger(color2);
    uint32_t maxColorPixel = max(getHighestValFromRGB(c1), getHighestValFromRGB(c2));
    uint32_t increment = map(speed, 1, 120, maxColorPixel*.25, 5);
	run = TRUE;
    
    for(int i=0; i<=maxColorPixel; i+=increment) {
        if(i <= c1.red) col1.red = i;
        if(i <= c1.green) col1.green = i;
        if(i <= c1.blue) col1.blue = i;
        if(i <= c2.red) col2.red = i;
        if(i <= c2.green) col2.green = i;
        if(i <= c2.blue) col2.blue = i;
        strip.setPixelColor(idex1, strip.Color(col1.red, col1.green, col1.blue));
        strip.setPixelColor(idex2, strip.Color(col2.red, col2.green, col2.blue));
        if(stop) {demo = FALSE; return;}
        if(demo) {if(millis() - lastModeSet > twoMinuteInterval) {return;}}
        showPixels();
        delay(speed);
    }

    idex1++;
    idex2++;
    if (idex1 == PIXEL_CNT) {idex1 = 0;}
    if (idex2 == PIXEL_CNT) {idex2 = 0;}
}

//Dual color chaser
void twoColorChaser(uint32_t color1, uint32_t color2) { //-COLOR CHASER (TWO COLOR SINGLE LED)
    static int idex1 = random(zone1Start, zone2End+1);  //idex;
    static int idex2 = random(zone3Start, zone4End+1);  //antipodal_index(idexR);
    static bool bounce1 = false;
    static bool bounce2 = false;
    Color col1, c1 = getColorFromInteger(color1);
    Color col2, c2 = getColorFromInteger(color2);
    uint32_t maxColorPixel = max(getHighestValFromRGB(c1), getHighestValFromRGB(c2));
    uint32_t increment = map(speed, 1, 120, maxColorPixel*.25, 5);
	run = TRUE;
    
    for(int i=0; i<=maxColorPixel; i+=increment) {
        //Slowly fade in the currently indexed pixels to blue and red
        if(i <= c1.red) col1.red = i;
        if(i <= c1.green) col1.green = i;
        if(i <= c1.blue) col1.blue = i;
        if(i <= c2.red) col2.red = i;
        if(i <= c2.green) col2.green = i;
        if(i <= c2.blue) col2.blue = i;
        strip.setPixelColor(idex1, strip.Color(col1.red, col1.green, col1.blue));
        strip.setPixelColor(idex2, strip.Color(col2.red, col2.green, col2.blue));
        
        //Then slowly fade out previously-lit pixels to black, leaving a nice "trailing" effect
        for(int j=0; j<PIXEL_CNT; j++) {
            if ((j != idex1) && (j != idex2)) {
                Color pixelColor = getColorFromInteger(strip.getPixelColor(j));
                if(pixelColor.red > 0) pixelColor.red-=pixelColor.red*.125;
                if(pixelColor.green > 0) pixelColor.green-=pixelColor.green*.125;
                if(pixelColor.blue > 0) pixelColor.blue-=pixelColor.blue*.125;
                strip.setPixelColor(j, strip.Color(pixelColor.red, pixelColor.green, pixelColor.blue));
            }
        }
        if(stop) {demo = FALSE; return;}
        if(demo) {if(millis() - lastModeSet > twoMinuteInterval) {return;}}
        showPixels();
        delay(speed);
    }
    
    //Check direction
    if(bounce1) {idex1--;} else {idex1++;}
    if(bounce2) {idex2--;} else {idex2++;}
    
    //Check beginning-of-trail
    if (idex1 <= zone1Start) {
        idex1 = zone1Start;
        bounce1 = FALSE;
    }
    if (idex2 <= zone3Start) {
        idex2 = zone3Start;
        bounce2 = FALSE;
    }
    
    //Check end-of-trail
    if (idex1 >= zone2End) {
        idex1 = zone2End;
        bounce1 = TRUE;
    }
    if (idex2 >= zone4End-1) {
        idex2 = zone4End-1;
        bounce2 = TRUE;
    }
}

//Pulse all dots with pseudo-random colors
void colorPulse() {
    uint32_t pulseRate;
	run = TRUE;

    for(int j=0; j<=c2; j++) {
        for(int i=0; i<strip.numPixels(); i++) {
            pulseRate = abs(255-(speed+j));
            strip.setPixelColor(i, Wheel(lerpColor(j, speed, pulseRate, 0, 255)));
        }
        if(pulseRate > 3840) {return;}  //Too fast - bail out.
        if(stop) {demo = FALSE; return;}
        if(demo) {if(millis() - lastModeSet > twoMinuteInterval) {return;}}
        showPixels();
		delay(speed);
    }
}

//Fill the dots with patterns from colors interpolated between 2 random colors
void cycleLerp() {
    uint32_t rate;
	run = TRUE;

    for(int j=0; j<=c2; j++) {
        for(int i=0; i<strip.numPixels(); i++) {
            rate = abs(i-(speed+j));
            strip.setPixelColor(i, Wheel(lerpColor(j, c1, rate, 0, 255)));
        }
        if(rate > 7680) {return;}   //Gettin' weird - bail out.
        if(stop) {demo = FALSE; return;}
        if(demo) {if(millis() - lastModeSet > twoMinuteInterval) {return;}}
        showPixels();
		delay(speed);
    }
}

//Randomly fill the dots with colors interpolated between the 2 chosen colors
void colorStripes() {
	run = TRUE;

    for(int j=0; j<=c2; j++) {
        if(stop) {return;}
        if(demo) {if(millis() - lastModeSet > twoMinuteInterval) {return;}}
        for(int i=0; i<strip.numPixels(); i++)
            strip.setPixelColor(i, lerpColor(j, c1, abs(i-j), 0, 255));
        showPixels();
        delay(speed);
    }
}

//Fill the dots one after the other with a color, wait (ms) after each one
int colorWipe(uint32_t c) {
    Color c2 = getColorFromInteger(c);
    run = FALSE;
  
    for(int i=0; i<strip.numPixels(); i++) {
        if(stop) {demo = FALSE; return 0;}
        if(demo) {if(millis() - lastModeSet > twoMinuteInterval) {return 0;}}
        int c3 = strip.getPixelColor(i);
        if(c3 > 0)
            fadeOutFromColor(i, getColorFromInteger(c3));
        fadeInToColor(i, c2);   //strip.setPixelColor(i, c);
        showPixels();
		delay(speed);
    }
    return 1;
}

//use colorWipe to cycle through all the colors 
void cycleWipe(void) {
    int retVal=1;
    run = TRUE;

    for(int j=0; j<256; j+=5) {
        if(demo) {if(millis() - lastModeSet > twoMinuteInterval) {return;}}
        if(j%2==0)
            retVal = colorWipe(Wheel(j & 0xFF));
        else
            retVal = colorWipe(Wheel((j+80) & 0xFF));
        if(stop || !retVal) {
            //colorWipe turns off the run loop, so let's turn it back on
            run = TRUE; 
            return;
        }
		delay(speed);
    }
    //colorWipe turns off the run loop, so let's turn it back on
    run = TRUE; 
}

void rainbow(void) {
	run = TRUE;
    
    for(int j=0; j<256; j++) {
        for(int i=0; i<strip.numPixels(); i++)
            strip.setPixelColor(i, Wheel((i+j) & 255));
        if(stop) {demo = FALSE; return;}
        if(demo) {if(millis() - lastModeSet > twoMinuteInterval) {return;}}
        showPixels();
        delay(speed);
    }
}

// Slightly different, this makes the rainbow equally distributed throughout, then wait (ms)
void rainbowCycle(void) {
  run = TRUE;

    for(int j=0; j<256; j++) { // 1 cycle of all colors on wheel
        for(int i=0; i< strip.numPixels(); i++)
            strip.setPixelColor(i, Wheel(((i * 256 / strip.numPixels()) + j) & 255));
        if(stop) {demo = FALSE; return;}
        if(demo) {if(millis() - lastModeSet > twoMinuteInterval) {return;}}
        showPixels();
        delay(speed);
    }
}

//Random burst - Random colors on each LED
void random_burst() { //-RANDOM INDEX/COLOR
    static int pixelCount = 0;
    static bool isAllFilled = FALSE;
    Color c1;
    uint32_t c2;
	run = TRUE;

    if(idex + ihue == 0) {
        pixelCount = 0;         //Careful to reset our variables to
        isAllFilled = FALSE;    //ensure a new cycle will start fresh
        //Then we need to fade any voxels still lit
        for(int idx = 0; idx < PIXEL_CNT; idx++)
            if(strip.getPixelColor(idx) > 0) {
                transition(black, true);
                return;
            }
    }
    idex = random(PIXEL_CNT);
    ihue = random(2, 256);
    
    ///DEBUG: sprintf(debug,"pixelCount = %i, isAllFilled = %i", pixelCount, isAllFilled);
    if(stop) {demo = FALSE; return;}
    if(demo) {if(millis() - lastModeSet > twoMinuteInterval) {return;}}

    //Filling every voxel with a random color - this may
    //reset voxels already filled before; we want this
    if(!isAllFilled) {
        c1 = getColorFromInteger(Wheel(ihue));
        c2 = strip.getPixelColor(idex);
        if(c2 > 0) {fadeOutFromColor(idex, getColorFromInteger(c2));}
        else {pixelCount++;}
        if(pixelCount < PIXEL_CNT) {fadeInToColor(idex, c1);}
        else {pixelCount--; isAllFilled = TRUE;}
    }
    else {  //When all voxels are filled, we begin fading them
        c2 = strip.getPixelColor(idex);
        if(c2 > 0) {pixelCount--;}
        if(pixelCount > 1) {fadeOutFromColor(idex, getColorFromInteger(c2));}
        else {idex = 0; ihue = 0;}  //When all but 1 voxels are blanked, we reset our variables for another
    }                               //cycle (we let transition() take care of that one last voxel)
    delay(speed);
}

//Theatre-style crawling lights with rainbow effect
void theaterChaseRainbow(void) {
	run = TRUE;

    for (int j=0; j<256; j++) {     //cycle all 256 colors in the wheel
        for (int q=0; q<3; q++) {
            for (int i=0; i < strip.numPixels(); i+=3)
                strip.setPixelColor(i+q, Wheel( (i+j) % 255));  //turn every third pixel on
            if(stop) {demo = FALSE; return;}
            if(demo) {if(millis() - lastModeSet > twoMinuteInterval) {return;}}
            showPixels();
        	delay(speed);
            for (int i=0; i < strip.numPixels(); i=i+3)
                strip.setPixelColor(i+q, 0);    //turn every third pixel off
        }
    }
}

//Fade from teal to blue to purple then back to blue then teal, repeat
//Random snowflakes twinkle white for random amounts of time 
//The snowflakes will hang around no faster than MIN_DELAY and no slower than MAX_DELAY
void frozen(void) {
    uint32_t startColor = 140;  //Hue for teal
    uint32_t stopColor  = 210;  //Hue for purple
    const int MIN_DELAY = 400;  //in ms
    const int MAX_DELAY = 700;  //in ms
    const int MIN_FLAKES = strip.numPixels()*0.01;	//  1% of total number of pixels
    const int MAX_FLAKES = strip.numPixels()*0.1;	// 10% of total number of pixels
    int numSnowFlakes = random(MIN_FLAKES, MAX_FLAKES+1);	//How many flakes should we have at a time
    unsigned long previousMillis = millis();
    unsigned long flakeLifeSpan = random(MIN_DELAY, MAX_DELAY+1); //How long will a snowflake last
    int increment = constrain(map(speed, 1, 120, 4, 8), 4, 8);  //How fast/slow a snowflake will dim
	run = TRUE;
    
    //forwards
    for(int j=startColor; j<stopColor; j++) { 	//cycle through the colors
        for(int i=0; i<strip.numPixels(); i++) {
            strip.setPixelColor(i, Wheel(j));	//first set all pixels to the same color
            if(millis() - previousMillis > flakeLifeSpan) {
        		previousMillis = millis();						//reset time interval
        		flakeLifeSpan = random(MIN_DELAY, MAX_DELAY+1);	//set new lifespan	
        		numSnowFlakes = random(MIN_FLAKES, MAX_FLAKES+1); //set new number of flakes
                findRandomSnowFlakesPositions(numSnowFlakes);	//get the list of flake positions
                //avoid our snowflakes fading out by brightening them up every now and then
                if(strip.Color(snowFlakeColor.red, snowFlakeColor.green, snowFlakeColor.blue) < 0xFFFFFF) {
                    if(snowFlakeColor.red < 0xFF) snowFlakeColor.red+=(0xFF-(snowFlakeColor.red))*.5;
                    if(snowFlakeColor.green < 0xFF) snowFlakeColor.green+=(0xFF-(snowFlakeColor.green))*.5;
                    if(snowFlakeColor.blue < 0xFF) snowFlakeColor.blue+=(0xFF-(snowFlakeColor.blue))*.5;
                    for(int k=0; k<numSnowFlakes; k++)
                        strip.setPixelColor(randomFlakes[k], strip.Color(snowFlakeColor.red, snowFlakeColor.green, snowFlakeColor.blue)); //Set snowflake
                    if(stop) {demo = FALSE; return;}
                    if(demo) {if(millis() - lastModeSet > twoMinuteInterval) {return;}}
                    showPixels();
                }
                increment = constrain(map(speed, 1, 120, 4, 8), 4, 8);  //refresh dim rate
            }
        }
        if(snowFlakeColor.red > 0) snowFlakeColor.red-=increment;
        if(snowFlakeColor.green > 0) snowFlakeColor.green-=increment;
        if(snowFlakeColor.blue > 0) snowFlakeColor.blue-=increment;
        for(int k=0; k<numSnowFlakes; k++)
            strip.setPixelColor(randomFlakes[k], strip.Color(snowFlakeColor.red, snowFlakeColor.green, snowFlakeColor.blue)); //Set snowflake
        if(stop) {demo = FALSE; return;}
        if(demo) {if(millis() - lastModeSet > twoMinuteInterval) {return;}}
        showPixels();
        delay(speed);
        increment = constrain(map(speed, 1, 120, 4, 8), 4, 8);  //refresh dim rate
    }
    
    //backwards
    for(int j=stopColor; j>startColor; j--) { 	//cycle through the colors
        for(int i=0; i<strip.numPixels(); i++) {
            strip.setPixelColor(i, Wheel(j));	//first set all pixels to the same color
            if(millis() - previousMillis > flakeLifeSpan) {
        		previousMillis = millis();						//reset time interval
        		flakeLifeSpan = random(MIN_DELAY, MAX_DELAY+1);	//set new lifespan	
        		numSnowFlakes = random(MIN_FLAKES, MAX_FLAKES+1); //set new number of flakes
                findRandomSnowFlakesPositions(numSnowFlakes);	//get the list of flake positions
                //avoid our snowflakes fading out by brightening them up every now and then
                if(strip.Color(snowFlakeColor.red, snowFlakeColor.green, snowFlakeColor.blue) < 0xFFFFFF) {
                    if(snowFlakeColor.red < 0xFF) snowFlakeColor.red+=(0xFF-(snowFlakeColor.red))*.5;
                    if(snowFlakeColor.green < 0xFF) snowFlakeColor.green+=(0xFF-(snowFlakeColor.green))*.5;
                    if(snowFlakeColor.blue < 0xFF) snowFlakeColor.blue+=(0xFF-(snowFlakeColor.blue))*.5;
                    for(int k=0; k<numSnowFlakes; k++)
                        strip.setPixelColor(randomFlakes[k], strip.Color(snowFlakeColor.red, snowFlakeColor.green, snowFlakeColor.blue)); //Set snowflake
                    if(stop) {demo = FALSE; return;}
                    if(demo) {if(millis() - lastModeSet > twoMinuteInterval) {return;}}
                    showPixels();
                }
                increment = constrain(map(speed, 1, 120, 4, 8), 4, 8);  //refresh dim rate
                if(strip.Color(snowFlakeColor.red, snowFlakeColor.green, snowFlakeColor.blue) <= 0) snowFlakeColor = getColorFromInteger(0xFFFFFF);
            }
        }
        if(snowFlakeColor.red > 0) snowFlakeColor.red-=increment;
        if(snowFlakeColor.green > 0) snowFlakeColor.green-=increment;
        if(snowFlakeColor.blue > 0) snowFlakeColor.blue-=increment;
        for(int k=0; k<numSnowFlakes; k++)
            strip.setPixelColor(randomFlakes[k], strip.Color(snowFlakeColor.red, snowFlakeColor.green, snowFlakeColor.blue)); //Set snowflake
        if(stop) {demo = FALSE; return;}
        if(demo) {if(millis() - lastModeSet > twoMinuteInterval) {return;}}
        showPixels();
        delay(speed);
        increment = constrain(map(speed, 1, 120, 4, 8), 4, 8);  //refresh dim rate
    }
}

void findRandomSnowFlakesPositions(int numFlakes) {
    for(int i=0;i<numFlakes;i++) {
        randomFlakes[i] = random(PIXEL_CNT);
        if(i > 0) {
            for(int j=0; j<i; j++) {
				//make sure we didn't already set that position
                while(randomFlakes[i] == randomFlakes[j])
                    randomFlakes[i] = random(PIXEL_CNT);
            }
        }
    }
}

//Color Blender
//Red starts at 1 end and green at the other. They increment towards each other. 
//When they meet, they eplode into the combined color
void collide(void) {
	uint32_t color1, color2;
	run = TRUE;
	
	for(color1=0; color1<256; color1+=85) {
	//for(color1=0; color1<=255; color1+=170) {
	    for(color2=color1+42; color2<=color1+85; color2+=43) {
	    //for(color2=color1+85; color2<=color1+170; color2+=85) {
    		for(int i=0; i<=strip.numPixels()/2; i++) { 
    			if(i*2 >= strip.numPixels()) {
    				//Explode the colors
    				for(int j=0; j<strip.numPixels(); j++) {
    					strip.setPixelColor(j, Wheel((((color2-color1)/2)+color1)&0xFF));
    					if(j%5==0)
    					    showPixels();
    				}
                    if(stop) {demo = FALSE; return;}
                    if(demo) {if(millis() - lastModeSet > twoMinuteInterval) {return;}}
    				showPixels();
					delay(speed);
    				break;
    			}
    			else {
    				//Grow the two colors from either end of the strip
    				strip.setPixelColor(i, Wheel(color1));
    				strip.setPixelColor(strip.numPixels()-i, Wheel((color2)&0xFF));
                    if(stop) {demo = FALSE; return;}
                    if(demo) {if(millis() - lastModeSet > twoMinuteInterval) {return;}}
    				showPixels();
					delay(speed);
    			}
    		}
	    }
	}
}

// Input a value 0 to 255 to get a color value.
// The colours are a transition r - g - b - back to r.
uint32_t Wheel(byte WheelPos) {
    if(WheelPos < 85)
        return strip.Color(WheelPos * 3, 255 - WheelPos * 3, 0);
    else if(WheelPos < 170) {
        WheelPos -= 85;
        return strip.Color(255 - WheelPos * 3, 0, WheelPos * 3);
    } 
    else {
        WheelPos -= 170;
        return strip.Color(0, WheelPos * 3, 255 - WheelPos * 3);
    }
}

/** Linear interpolation between colors.
  @param a, b The colors to interpolate between.
  @param val Position on the line between color a and color b.
  When equal to min the output is color a, and when equal to max the output is color b.
  @param minVal Minimum value that val will take.
  @param maxVal Maximum value that val will take.

  @return Color between colors a and b.*/
uint32_t lerpColor(uint32_t c1, uint32_t c2, uint32_t val, uint32_t minVal, uint32_t maxVal) {
    Color a = getColorFromInteger(c1);
    Color b = getColorFromInteger(c2);
    int red = a.red + (b.red-a.red) * (val-minVal) / (maxVal-minVal);
    int green = a.green + (b.green-a.green) * (val-minVal) / (maxVal-minVal);
    int blue = a.blue + (b.blue-a.blue) * (val-minVal) / (maxVal-minVal);
    
    return strip.Color(red, green, blue);
}

/** Returns a color from a set of colors fading from blue to green to red and back again
    the color is returned based on where the parameter *val* falls between the parameters
    *min* and *max*.  If *val* is min, the function returns a blue color.  If *val* is halfway
    between *min* and *max*, the function returns a yellow color.*/  
uint32_t colorMap(float val, float minVal, float maxVal) {
    const float range = 2048;
    val = range * (val-minVal) / (maxVal-minVal);
    
    Color colors[6];
    
    colors[0].red = 0;
    colors[0].green = 0;
    colors[0].blue = brightness;
    
    colors[1].red = 0;
    colors[1].green = brightness;
    colors[1].blue = brightness;
    
    colors[2].red = 0;
    colors[2].green = brightness;
    colors[2].blue = 0;
    
    colors[3].red = brightness;
    colors[3].green = brightness;
    colors[3].blue = 0;
    
    colors[4].red = brightness;
    colors[4].green = 0;
    colors[4].blue = 0;
    
    colors[5].red = brightness;
    colors[5].green = 0;
    colors[5].blue = brightness;
    
    if(val <= range/6)
        return lerpColor(strip.Color(colors[0].red, colors[0].green, colors[0].blue), strip.Color(colors[1].red, colors[1].green, colors[1].blue), val, 0, range/6);
    else if(val <= 2 * range / 6)
        return(lerpColor(strip.Color(colors[1].red, colors[1].green, colors[1].blue), strip.Color(colors[2].red, colors[2].green, colors[2].blue), val, range / 6, 2 * range / 6));
    else if(val <= 3 * range / 6)
        return(lerpColor(strip.Color(colors[2].red, colors[2].green, colors[2].blue), strip.Color(colors[3].red, colors[3].green, colors[3].blue), val, 2 * range / 6, 3*range / 6));
    else if(val <= 4 * range / 6)
        return(lerpColor(strip.Color(colors[3].red, colors[3].green, colors[3].blue), strip.Color(colors[4].red, colors[4].green, colors[4].blue), val, 3 * range / 6, 4*range / 6));
    else if(val <= 5 * range / 6)
        return(lerpColor(strip.Color(colors[4].red, colors[4].green, colors[4].blue), strip.Color(colors[5].red, colors[5].green, colors[5].blue), val, 4 * range / 6, 5*range / 6));
    else
        return(lerpColor(strip.Color(colors[5].red, colors[5].green, colors[5].blue), strip.Color(colors[0].red, colors[0].green, colors[0].blue), val, 5 * range / 6, range));
}

/** Find index of antipodal opposite LED **/
int antipodal_index(int i) {
    int iN = i + zone2End;
    if (i >= zone2End)
        iN = ( i + zone2End ) % PIXEL_CNT; 
    return iN;
}

/** Break down an int color into its 3 rgb components for the given pixel 
  @param index The strip index of the pixel.
  @return The rgb Color for the given pixel.*/
Color getPixelColor(int index) {
    uint32_t col = strip.getPixelColor(index);
    return getColorFromInteger(col);
}

/** Set a voxel at a position to a color.
  @param x, y, z Coordinate of the LED to set.
  @param col Color to set the LED to.
  */
void setPixelColor(int x, int y, int z, Color col) {
  if(x >= 0 && y >= 0 && z >= 0 && 
     x < SIDE && y < SIDE && z < SIDE) {
    int index = (z*64) + (x*8) + y;
    strip.setPixelColor(index, strip.Color(col.red, col.green, col.blue));
  }
}

/** Set a voxel at a position to a color.
  @param p Coordinate of the LED to set.
  @param col Color to set the LED to.
  */
void setPixelColor(Point p, Color col) {
  setPixelColor(p.x, p.y, p.z, col);
}

/** Break down an int color into its 3 rgb components for the given integer
  @param col The color value, represented by an integer
  @return The rgb Color for the given integer.*/
Color getColorFromInteger(uint32_t col) {
    Color retVal;
    
    retVal.red   = (uint8_t)(col >> 16),
    retVal.green = (uint8_t)(col >>  8),
    retVal.blue  = (uint8_t)col;
    return retVal;
}

/** Fade in a pixel color from 0 to the given Color
  @param index The strip index of the pixel.
  @param col The Color to fade into.*/
void fadeInToColor(uint32_t index, Color col) {
    uint32_t maxColorPixel = getHighestValFromRGB(col);
    uint32_t increment = map(speed, 1, 120, maxColorPixel*.125, 5);
    Color col2;
    
    for(int i=0; i<=maxColorPixel; i+=increment) {
        if(i <= col.red) col2.red = i;
        if(i <= col.green) col2.green = i;
        if(i <= col.blue) col2.blue = i;
        strip.setPixelColor(index, strip.Color(col2.red, col2.green, col2.blue));
        if(stop) {demo = FALSE; return;}
        if(demo) {if(millis() - lastModeSet > twoMinuteInterval) {return;}}
        showPixels();
    }
}

/** Fade out a pixel color from the given Color to 0
  @param index The strip index of the pixel.
  @param col The Color to fade out from.*/
void fadeOutFromColor(uint32_t index, Color col) {
    uint32_t maxColorPixel = getHighestValFromRGB(col);
    uint32_t increment = map(speed, 1, 120, maxColorPixel*.125, 5);
    Color col2;
    
    if(maxColorPixel == 0) {return;}
    
    for(int i=maxColorPixel; i>=0; i-=increment) {
        if(i <= col.red) col2.red = i;
        if(i <= col.green) col2.green = i;
        if(i <= col.blue) col2.blue = i;
        strip.setPixelColor(index, strip.Color(col2.red, col2.green, col2.blue));
        if(stop) {demo = FALSE; return;}
        if(demo) {if(millis() - lastModeSet > twoMinuteInterval) {return;}}
        showPixels();
    }
    //Ensure the pixel has been fully blanked
    strip.setPixelColor(index, 0);
    showPixels();
}

/** Return the highest value among the 3 rgb components for the given Color
  @param col The Color to be evaluated.
  @return The highest value, be it r, g or b, from the given Color.*/
uint32_t getHighestValFromRGB(Color col) {
    if(col.red-col.green > 0)
        if(col.red-col.blue > 0)
            return col.red; 
        else 
            return col.blue;
    else if(col.green-col.blue > 0) 
        return col.green;
    else 
        return col.blue;
}

//Spark Cloud Mode
//Expect a string like thisto change the mode Mode: M:ZONE,W:30,B:120,C1:002BFF,C2:FF00DB,C3:FF4600,C4:23FF00,
//Or simply this to just update speed or brightness:        W:30,B:120,
//Received command should have an ending comma, it makes this code easier
//All colors are in hex format
//If the mode Mode is changing, return the enum value of the mode
//Else if only the speed or brightness is being updated return the following:
//returnValue = 1000 - command was recieved to update speed or brightness, but new values  are == to old values
//returnValue = 1001 - Brightness has been updated
//returnValue = 1002 - Speed has been updated
int SetMode(String command) {
	int beginIdx = 0;
	int returnValue = -1;
	int idx = command.indexOf(',');
    bool isNewMode = FALSE;
	bool isNewSpeed = FALSE;
	bool isNewBrightness = FALSE;
    
    // Trim extra spaces
    command.trim();
    // Convert it to upper-case for easier matching
    command.toUpperCase();
    
    //keep track or the last command received for the auto off feature
    lastCommandReceived = millis();
    
    //sprintf(debug,"%s", command.c_str());

	while(idx != -1) {
		if(command.charAt(beginIdx) == 'M') {
			//set the new mode from modeStruct array index
			returnValue = setNewMode(getModeIndexFromName(command.substring(beginIdx+2, idx).c_str())); 
			isNewMode = TRUE;
		}
		else if(command.charAt(beginIdx) == 'S') {
		    int receivedSpeedValue = command.substring(beginIdx+2, idx).toInt();
		    if(receivedSpeedValue > (int)(sizeof(speedPresets)/sizeof(int)))
		        receivedSpeedValue = sizeof(speedPresets)/sizeof(int) - 1;
		    if (speedIndex != receivedSpeedValue) {
		        //we don't update the speed when currently in LISTENER mode
				if(currentModeID != LISTENER) isNewSpeed = TRUE;
			}
			//we don't update the speed when currently in LISTENER mode
			if(currentModeID != LISTENER) {
    			speedIndex = receivedSpeedValue;
    			speed = speedPresets[speedIndex];
			}
		}
		else if(command.charAt(beginIdx) == 'B') {
		    int newBrightness = command.substring(beginIdx+2, idx).toInt() * (255 * .01);	//Scale 0-100 to 0-255
			if(brightness != newBrightness) {
			    //we don't update the brightness when currently in LISTENER mode
				if(currentModeID != LISTENER) isNewBrightness = TRUE;
			}
			//we don't update the brightness when currently in LISTENER mode
			if(currentModeID != LISTENER) brightness = newBrightness;
		}
        else if(command.charAt(beginIdx) == 'C') {
            char * p;
            switch(command.charAt(beginIdx+1)) {
                case '1':
                    color1 = strtoul( command.substring(beginIdx+3, idx).c_str(), & p, 16 );  //Convert hex string to int
                    break;
                case '2':
                    color2 = strtoul( command.substring(beginIdx+3, idx).c_str(), & p, 16 );  //Convert hex string to int
                    break;
                case '3':
                    color3 = strtoul( command.substring(beginIdx+3, idx).c_str(), & p, 16 );  //Convert hex string to int
                    break;
                case '4':
                    color4 = strtoul( command.substring(beginIdx+3, idx).c_str(), & p, 16 );  //Convert hex string to int
                    break;
                case '5':
                    color5 = strtoul( command.substring(beginIdx+3, idx).c_str(), & p, 16 );  //Convert hex string to int
                    break;
                case '6':
                    color6 = strtoul( command.substring(beginIdx+3, idx).c_str(), & p, 16 );  //Convert hex string to int
                    break;
            }
		}
		beginIdx = idx + 1;
		idx = command.indexOf(',', beginIdx);
	}

	//Set the flags if it's a new mode
	//Need this when just updating speed and brightness so a currently running mode doesn't start over
    if(isNewMode) {
        run = TRUE;
	    stop = TRUE;
	    demo = FALSE;
    }
    else {
		//I guess we're updating only the speed or brightness, so let's update the Pixels
		//we don't update the speed or brightness when currently in LISTENER mode
		if(currentModeID != LISTENER) showPixels();
		
		if(isNewBrightness) {
			//Let the sender know we got the new brightness command
			returnValue = 1001;
		}
		else if(isNewSpeed==true) {
			//Let the sender know we got the new speed command
			returnValue = 1002;
		}
		else {
			//If we got here, it's possible that a command was set to update speed or brightness
			//But neither one was a new value. The new values, equal the current values
			returnValue = 1000;
		}
    }

	// still here, so everything must be fine
	return returnValue;
}

//Change Mode based on the modeStruct array index
int setNewMode(int newModeIndex) {
    currentModeID = modeStruct[newModeIndex].modeId;
    sprintf(currentModeName,"%s", modeStruct[newModeIndex].modeName);
	resetVariables(modeStruct[newModeIndex].modeId);
	return newModeIndex;
}

int getModeIndexFromName(String name) {
    for(int i=0;i<(int)(sizeof modeStruct / sizeof modeStruct[0]); i++) {
        if(name.equals(String(modeStruct[i].modeName)))
            return i;
    }
    return -1;
}

int getModeIndexFromID(int id) {
    for(int i=0; i<(int)(sizeof modeStruct / sizeof modeStruct[0]); i++) {
        if(id == modeStruct[i].modeId)
            return i;
    }
    return -1;
}

void resetVariables(int modeIndex) {
    switch (modeIndex) {
        case STANDBY:
	        break;
     	case COLORALL:
	        break;
     	case COLORBREATHE:
	        break;
     	case CHASER:
			break;
		case ZONE:
	        break;
		case ZONECHASER:
	        break;
		case COLORPULSE:
		    break;
		case COLORSTRIPES:
		    break;
		case ACIDDREAM:
		    break;    
		case COLORWIPE:
		    break;    
		case RAINBOW:
		    break;
		case SPECTRUM:
		    break;
		case THEATERCHASE:
		    break;
		case FROZEN:
		    break;
		case LISTENER:
		    break;
		case COLLIDE:
		    break;
		case POLICELIGHTS:
		    break;
		case TWOCOLORWIPE:
		    break;
		case TWOCOLORCHASE:
		    break;
		case COLORFADE:
		    break;
		case RAINBOW_BURST:
            //We must reset RAINBOW BURST mode's startup variables
            idex = 0;
            ihue = 0;
		    break;
     	case FLICKER:
			break;
		case NORMAL:
		default:
			break;
    }    
    
}

void listen() {
    run = TRUE;
    
    // Checks for the presence of a UDP packet, and reports the buffer size
    unsigned long bytes = Udp.parsePacket();
    if(bytes >= EXPECTED_PACKED_SIZE) {
        // Read 198 bytes of data from the buffer
        char data[EXPECTED_PACKED_SIZE];
        Udp.read(data, EXPECTED_PACKED_SIZE);
        
        // Ignore all other chars
        Udp.flush();
        
        /*------------------*/
        /*-- Header check --*/
        /*------------------*/
        // Block Start Byte
        if(data[0] == TPM2NET_HEADER_IDENT) {
            // Block Type: Command Packet
            if((data[1] == TPM2NET_CMD_COMMAND) || stop) {
                strip.setBrightness(brightness);
                if(stop) {demo = FALSE;}
                return; // Don't care
            }

            // Block Type: Frame Data Packet - that's what we want
            if(data[1] == TPM2NET_CMD_DATAFRAME) {
                // Calculate frame size
                char frameSize = data[2];
                frameSize = (frameSize << 8) + data[3];
        
                // Use packetNumber to calculate offset
                char packetNumber = data[4];
                char totalPackets = data[5];
        
                // Calculate offset
                int index = packetNumber * PIXELS_PER_PANEL;
                int voxelIdx = TPM2NET_HEADER_SIZE;

                // Start drawing!!
                for(int z = NR_OF_PANELS-1; z >= 0; z--) {                  // We're only dealing in 2 dimensions (width & height)
                    for(int col = 0; col < NR_OF_PANELS; col++) {           // Linewise, starting from leftmost index
                        for(int row = NR_OF_PANELS - 1; row >= 0; row--) {  // Columnwise, starting from topmost index
                            Color pixelColor = Color(data[voxelIdx], data[voxelIdx + 1], data[voxelIdx + 2]);  // Take 3 color bytes from buffer
                            setPixelColor(z, col, row, pixelColor);
                        }
                        voxelIdx+=3; // Increment buffer index by 3 bytes
                    }
                }
                // Display!!
                if(packetNumber == totalPackets) {
                    if(stop) {demo = FALSE; strip.setBrightness(brightness); return;}
                    if(demo) {if(millis() - lastModeSet > twoMinuteInterval) {strip.setBrightness(brightness); return;}}
                    strip.setBrightness(255);
                    strip.show();
                    Particle.process();    //process Spark events
                }
            }
        }
    }
}

/* ======================= AUDIO SPECTRUM mode functions ====================== */
void FFTJoy() {
    run = TRUE;
    
    for(int i=0; i<pow(2,M); i++) {
        real[i]=analogRead(MICROPHONE)-SAMPLES;
        delayMicroseconds(120);  /* 210 this sets our 'sample rate'.  I went through a bunch of trial and error to 
                                  * find a good sample rate to put a soprano's vocal range in the spectrum of the cube
    							  *
      							  * 120 gets the entire range from bass to soprano within the cube's spectrum
      							  * *From bass to soprano: https://youtu.be/6jqCuE7C3rg */
        imaginary[i]=0;
    }
    
    FFT(1, M, real, imaginary);
    
    /* In this loop we can trim our 'viewing window', in regards to
     * what range of the audio spectrum we want to see at all times.
     * We do that by means of slightly 'shifting' the array index in
     * both imaginary[] and real[] arrays - the greater the index,
     * the more our 'window' will shift towards high frequencies. */
    for(int i=0; i<pow(2,M); i++) {
        imaginary[i]=sqrt(pow(imaginary[i+1],2)+pow(real[i+1],2));
        if(imaginary[i]>maxVal)
            maxVal=imaginary[i];
    }

    for(int i=0; i<pow(2,M)/2; i++) {
        imaginary[i]=SIDE*imaginary[i]/maxVal;
        Color pixelColor;
        int y, pixIdx, pixUppIdx, pixLowIdx;
    	for(y=0; y<=imaginary[i]; y++) {
          	pixelColor=getColorFromInteger(colorMap(y,0,SIDE+2));
          	pixIdx=(((SIDE-1)*SIDE*SIDE) + (i*SIDE) + y);
            strip.setPixelColor(pixIdx, strip.Color(pixelColor.red, pixelColor.green, pixelColor.blue));
    		if(dotMode) {
    		    //Down-top fade/blanking
                pixelColor=black;
                pixLowIdx=pixIdx-(pixIdx%SIDE);
                for(int j=pixLowIdx; j<pixIdx; j++) {
                    if(smooth) {
                        pixelColor=getPixelColor(j);
                        //This causes a nice and smooth 'trailing' effect
                      	//from the base of the cube (y-axis) towards the peak
                        if(pixelColor.red > 0) pixelColor.red-=pixelColor.red*(map(j%SIDE, 0, SIDE-1, SIDE-1, 1)*.05);
                        if(pixelColor.green > 0) pixelColor.green-=pixelColor.green*(map(j%SIDE, 0, SIDE-1, SIDE-1, 1)*.05);
                        if(pixelColor.blue > 0) pixelColor.blue-=pixelColor.blue*(map(j%SIDE, 0, SIDE-1, SIDE-1, 1)*.05);
                    }
        			strip.setPixelColor(j, strip.Color(pixelColor.red, pixelColor.green, pixelColor.blue));
                }
            }
    	}
    	
        //Topdown fade/blanking
        pixelColor=black;
        pixUppIdx=pixIdx+(SIDE-(pixIdx%SIDE));
        for(int j=pixUppIdx-1; j>pixIdx; j--) {
            if(smooth) {
                pixelColor=getPixelColor(j);
              	//This causes a nice and smooth 'trailing' effect
              	//from the top of the cube (y-axis) towards the peak
                if(pixelColor.red > 0) pixelColor.red-=pixelColor.red*(map(j%SIDE, 0, SIDE-1, 1, SIDE-1)*.05);
                if(pixelColor.green > 0) pixelColor.green-=pixelColor.green*(map(j%SIDE, 0, SIDE-1, 1, SIDE-1)*.05);
                if(pixelColor.blue > 0) pixelColor.blue-=pixelColor.blue*(map(j%SIDE, 0, SIDE-1, 1, SIDE-1)*.05);
            }
            strip.setPixelColor(j, strip.Color(pixelColor.red, pixelColor.green, pixelColor.blue));
        }
    }
    
    //Fade the 'trail' to black over the length of the cube's z-axis
    for(int x=0; x<SIDE; x++)
        for(int y=0; y<SIDE; y++)
            for(int z=0; z<SIDE-1; z++) {
                int pixIdx=((z+1)*SIDE*SIDE) + (x*SIDE) + y;
                Color trailColor=getPixelColor(pixIdx);
                if(smooth) {
                  	//This is responsible for the 'meteors' shooting towards the back of 
                  	//the cube; otherwise it would look like they were 'going backwards'
                    if(trailColor.red > 0) trailColor.red-=trailColor.red*(map(z%SIDE, 0, SIDE-1, 1, SIDE-1)*.05); //.125;
                    if(trailColor.green > 0) trailColor.green-=trailColor.green*(map(z%SIDE, 0, SIDE-1, 1, SIDE-1)*.05); //.125;
                    if(trailColor.blue > 0) trailColor.blue-=trailColor.blue*(map(z%SIDE, 0, SIDE-1, 1, SIDE-1)*.05); //.125;
                }
                strip.setPixelColor((z*SIDE*SIDE) + (x*SIDE) + y, strip.Color(trailColor.red, trailColor.green, trailColor.blue));
                if(stop) {demo = FALSE; strip.setBrightness(brightness); return;}
                if(demo) {if(millis() - lastModeSet > twoMinuteInterval) {strip.setBrightness(brightness); return;}}
                delayMicroseconds(speed);  //introducing a little bit of delay to 'smoothen-out' transitions
            }
    
    maxVal= (maxVal>=120) ? maxVal-2 : (maxVal<8) ? 8 : maxVal-.8;
    if(stop) {demo = FALSE; strip.setBrightness(brightness); return;}
    if(demo) {
        if(millis() - lastModeSet > twoMinuteInterval) {strip.setBrightness(brightness); return;}
        strip.setBrightness(127);
        strip.show();
        Particle.process();    //process Spark events
    }
    else {showPixels();}
}

void smoothSwitch() {
    smooth = !digitalRead(SMOOTH_SW);
}

void modeButton() {
    dotMode = !dotMode;
}

void initMicrophone() {
    pinMode(GAIN_CONTROL, OUTPUT);
    analogWrite(GAIN_CONTROL, INPUTLEVEL);  //digitalWrite(GAIN_CONTROL, LOW);
    //Seed the random number generator.  THINGS WILL NEVER BE THE SAME AGAIN
    srand(analogRead(MICROPHONE)); 
}

short FFT(short int dir,int m,float *x,float *y) {
   int n,i,i1,j,k,i2,l,l1,l2;
   float c1,c2,tx,ty,t1,t2,u1,u2,z;

   /* Calculate the number of points */
   n = 1;
   for (i=0;i<m;i++) 
      n *= 2;

   /* Do the bit reversal */
   i2 = n >> 1;
   j = 0;
   for (i=0;i<n-1;i++) {
      if (i < j) {
         tx = x[i];
         ty = y[i];
         x[i] = x[j];
         y[i] = y[j];
         x[j] = tx;
         y[j] = ty;
      }
      k = i2;
      while (k <= j) {
         j -= k;
         k >>= 1;
      }
      j += k;
   }

   /* Compute the FFT */
   c1 = -1.0; 
   c2 = 0.0;
   l2 = 1;
   for (l=0;l<m;l++) {
      l1 = l2;
      l2 <<= 1;
      u1 = 1.0; 
      u2 = 0.0;
      for (j=0;j<l1;j++) {
         for (i=j;i<n;i+=l2) {
            i1 = i + l1;
            t1 = u1 * x[i1] - u2 * y[i1];
            t2 = u1 * y[i1] + u2 * x[i1];
            x[i1] = x[i] - t1; 
            y[i1] = y[i] - t2;
            x[i] += t1;
            y[i] += t2;
         }
         z =  u1 * c1 - u2 * c2;
         u2 = u1 * c2 + u2 * c1;
         u1 = z;
      }
      c2 = sqrt((1.0 - c1) / 2.0);
      if (dir == 1) 
         c2 = -c2;
      c1 = sqrt((1.0 + c1) / 2.0);
   }

   /* Scaling for forward transform */
   if (dir == 1) {
      for (i=0;i<n;i++) {
         x[i] /= n;
         y[i] /= n;
      }
   }

   return(0);
}

/* ============================== Text functions ============================== */
void showChar(char a, Point p, Color col) {
    for(int row=0; row<SIDE; row++)
        for(int bit=0; bit<8; bit++)
            if(((fontTable[((int)a)*8+row]>>(7-bit))&0x01)==1)
                for(int th=0; th<thickness+1; th++)
                    setPixelColor(p.x+bit, p.y+(SIDE-1-row), p.z-th, col);
}

void showChar(char a, Point origin, Point angle, Color col) {
    showChar(a, origin, Point(ceil((SIDE-1)*.5),0,0), angle, col);
}

void showChar(char a, Point origin, Point pivot, Point angle, Color col) {
    for(int row=0; row<SIDE; row++)
        for(int bit=0; bit<8; bit++)
            if(((fontTable[((int)a)*8+(7-row)]>>(7-bit))&0x01)==1)
                for(int th=0; th<thickness+1; th++)
                    setPixelColor(origin.x+((float)bit-pivot.x)*cos(angle.y)-th, 
                    origin.y+((float)row-pivot.y)*cos(angle.x), 
                    origin.z+((float)row-pivot.y)*sin(angle.x)+((float)bit-pivot.y)*sin(angle.y)-th, col);
}

void scrollSpinningText(String text, Point initialPosition, Color col) {
    for(int i=0; i<strlen(text); i++)
        showChar(text.charAt(i), Point(SIDE*i-initialPosition.x, initialPosition.y, initialPosition.z), Point(0,((initialPosition.x-SIDE-1)*.5)*.6,0), col);
}

void scrollText(String text, Point initialPosition, Color col) {
    for(int i=0; i<strlen(text); i++)
        showChar(text.charAt(i), Point(SIDE*i-initialPosition.x, initialPosition.y, initialPosition.z), col);
}

void marquee(String text, float pos, Color col) {
    for(int i=0; i<strlen(text); i++)
        showMarqueeChar(text.charAt(i), (int)pos - SIDE*i, col);
}

void showMarqueeChar(char a, int pos, Color col) {
    for(int row=0; row<SIDE; row++)
        for(int bit=0; bit<8; bit++)
            if(((fontTable[((int)a)*8+row]>>(7-bit))&0x01)==1) {
                for(int th=0; th<thickness+1; th++) {
                    if((pos-bit)<SIDE)
                        setPixelColor(SIDE-1-th, SIDE-1-row, pos-bit, col);
                    if(((pos-bit)>=SIDE)&&((pos-bit)<2*SIDE))
                        setPixelColor((SIDE-1-th)-(pos-bit-SIDE-th), SIDE-1-row, SIDE-1-th, col);
                    if(((pos-bit)>=2*SIDE)&&((pos-bit)<3*SIDE))
                        setPixelColor(th, SIDE-1-row, SIDE-1-(pos-bit-2*SIDE), col);
                    if((pos-bit)>3*SIDE)
                        setPixelColor(pos-bit-3*SIDE, SIDE-1-row, th, col);
                }
            }
}
