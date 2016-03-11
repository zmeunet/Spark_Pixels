/**
 ******************************************************************************
 * @extended SparkPixels.ino:
 *		New mode: DIGI, CUBE CLASSICS, IFTTT WEATHER
 *		New setting: AUX Switches
 *		New Functions: transitionALl, transitionOne, transitionHelper, getTransitionStep, 
 *                     clamp, makeAuxSwitchList, getAuxSwitchIndexFromID, updateAuxSwitches, 
 *                     iftttWeather
 *      New Cloud Function: SetAuxSwitch
 *      Updated Cloud Function: Renamed the cloud function *Function* to *FnRouter*
 *      New Feature: Added AUX Switchs used to turn things on or off or switch between two 
 *                   options, i.e. switch between using a light sensor or the app to set 
 *                   LED brightness. The Auto Shut Off has migrated to use this function.
 *                   
 *                   IFTTT WEATHER - search for the Spark Pixels recipe on ifttt.com or
 *                   create your own. Just setup your device to call the SetMode 
 *                   function with this input: M:IFTTT WEATHER,C6:0000FF,
 *                   Where 0000FF is the hex value for blue. Color must be in hex formate
 *                   Oh, and Don't forget that last comma,
 * @author   Werner Moecke, Kevin Carlborg
 * @version  BETA V2.8
 * @date     02-February-2016 ~ 10-March-2016
 *
 * @extended SparkPixels.ino:
 * 		Fixed issue with CHEERLIGHTS mode responsiveness to external events;
 *      Keep connection alive in CHEERLIGHTS mode.
 * @author   Werner Moecke
 * @version  V2.7
 * @date     25-January-2016 ~ 01-February-2016
 *
 * @extended SparkPixels.ino:
 *		New mode: FILLER (by Werner Moecke [based on idea by Alex Hornstein])
 *		New Functions: filler()
 *
 * 		Fixed CHEERLIGHTS mode not initializing after changing modes
 * @author   Werner Moecke
 * @version  V2.6
 * @date     16-January-2016 ~ 17-January-2016
 *
 * @extended SparkPixels.ino:
 *		New mode: CHEERLIGHTS (by Alex Hornstein, Werner Moecke [stability fixes, extra effects])
 *		New Functions: cheerlights(), fillX(), fillY(), fillZ()
 *
 * 		Fixed UDP listener stability issues and improved support for multiple cube streaming
 * @author   Werner Moecke
 * @version  V2.5
 * @date     05-January-2016 ~ 16-January-2016
 *
 * @extended SparkPixels.ino:
 *		New mode: CUBES (by Alex Hornstein, C++ port by Werner Moecke)
 *		New Functions: cubes(), drawCube(), cubeInc()
 *
 * 		Fixed demo getting stuck at OFF mode: Re-numbered modeId constant declares
 * @author   Werner Moecke
 * @version  V2.4
 * @date     23-December-2015 ~ 25-December-2015
 *
 * @extended SparkPixels.ino:
 *		New mode: CHRISTMASTREE (by Kevin Carlborg)
 *		New mode: WHIRLWIND (by Bill Marrs)
 *		New setting: Auto Shut Off enable/disable (through cloud function)
 *		New Functions: christmasTree(), whirlWind(), isWhiteColor(),
 *		drawSolidHorizontalCircle(), drawHollowHorizontalCircle(),
 * 		setASO(), randomColor(), randomDecimal(), radius()
 * @author   Werner Moecke
 * @version  V2.3
 * @date     18-November-2015 ~ 21-December-2015
 *
 * @extended SparkPixels.ino:
 *		Function: void publishCloudVariables()
 * @author   Werner Moecke
 * @version  V2.2
 * @date     07-November-2015 ~ 08-November-2015
 *
 * @extended SparkPixels.ino:
 *		New mode: SQUARRAL (ported from original L3D demo)
 *		New mode: PLASMA (ported from original L3D demo)
 *		New Functions: zplasma(), squarral()
 * @added: New instruction, SYSTEM_THREAD(ENABLED) - provided by Particle as of v0.4.6
 * @author   Werner Moecke
 * @version  V2.1
 * @date     28-October-2015 ~ 07-November-2015
 *
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
SYSTEM_THREAD(ENABLED);

//#include "neopixel.h"
#include "neopixel/neopixel.h"
#include <math.h>
#define ON                      1
#define OFF                     0

//Global Defines
#define MAX_PUBLISHED_STRING_SIZE   622     //defined by Particle Industries
#define TIME_ZONE_OFFSET            -3		//The offset to obtain your region's time correctly
#define CLAMP_255(v) (v > 255 ? 255 : (v < 0 ? 0 : v))

//NEOPIXEL Defines
#define PIXEL_CNT               512
#define PIXEL_PIN               D0
#define PIXEL_TYPE              WS2812B

Adafruit_NeoPixel strip = Adafruit_NeoPixel(PIXEL_CNT, PIXEL_PIN, PIXEL_TYPE);

/* ======================= ADD NEW MODE ID HERE. ======================= */
// Mode ID Defines
const int STANDBY                     = 0; //credit: Kevin Carlborg
const int NORMAL                      = 1; //credit: Kevin Carlborg
const int COLORALL                    = 2; //credit: Kevin Carlborg
const int CHASER                      = 3; //credit: Kevin Carlborg
const int ZONE                        = 4; //credit: Kevin Carlborg
const int COLORPULSE                  = 5; //credit: Werner Moecke
const int COLORSTRIPES                = 6; //credit: Werner Moecke
const int ACIDDREAM                   = 7; //credit: Werner Moecke
const int RAINBOW                     = 8; //credit: Kevin Carlborg
const int THEATERCHASE                = 9; //credit: Kevin Carlborg
const int FROZEN                      = 10; //credit: Kevin Carlborg
const int COLLIDE                     = 11; //credit: Kevin Carlborg
const int COLORFADE                   = 12; //credit: Werner Moecke
const int RAINBOW_BURST               = 13; //credit: Werner Moecke
const int FLICKER                     = 14; //credit: Werner Moecke
const int COLORBREATHE                = 15; //credit: Werner Moecke
const int POLICELIGHTS                = 16; //credit: Werner Moecke
const int TWOCOLORCHASE               = 17; //credit: Werner Moecke
const int LISTENER                    = 18; //credit: Werner Moecke
const int ZONECHASER                  = 19; //credit: Werner Moecke
const int SPECTRUM                    = 20; //credit: Alex Hornstein, Werner Moecke (extra settings)
const int SQUARRAL                    = 21; //credit: Alex Hornstein
const int PLASMA                      = 22; //credit: Alex Hornstein, Werner Moecke (speed settings)
const int WARMFADE                    = 23; //credit: Kevin Carlborg
const int CHRISTMASTREE               = 24; //credit: Kevin's friggin' xmas tree - there, have it!
const int CHRISTMASLIGHTS             = 25; //credit: Kevin Carlborg, Werner Moecke (L3D Cube port)
const int CHRISTMASWREATH             = 26; //credit: Kevin Carlborg, Werner Moecke (L3D Cube port, extra colors)
const int TEXTSCROLL                  = 27; //credit: Alex Hornstein, Hans-Peter "Hape", Werner Moecke (C++ port, extra settings)
const int TEXTSPIN                    = 28; //credit: Alex Hornstein, Hans-Peter "Hape", Werner Moecke (C++ port, extra settings)
const int TEXTMARQUEE                 = 29; //credit: Alex Hornstein, Hans-Peter "Hape", Werner Moecke (C++ port, extra settings)
const int WHIRLWIND                   = 30; //credit: Bill Marrs
const int CUBES                       = 31; //credit: Alex Hornstein, Werner Moecke (C++ port, extra settings)
const int RAIN                        = 32; //credit: Kevin Carlborg, Werner Moecke (Matrix Mode)
const int CHEERLIGHTS                 = 33; //credit: Alex Hornstein, Werner Moecke (stability fixes, extra transition effects)
const int FILLER                      = 34; //credit: Werner Moecke (based on idea by Alex Hornstein)
const int CUBE_PAINTER                = 35; //credit: Werner Moecke (based on idea by Alex Hornstein)
const int CUBE_CLASSICS               = 36; //credit: http://www.instructables.com/id/Led-Cube-8x8x8/, Kevin Carlborg (L3D Cube port)
const int IFTTTWEATHER                = 37; //credit: Kevin Carlborg
const int DIGI                        = 38; //credit: Kevin Carlborg

/* ======================= ADD NEW AUX SWITCH ID HERE. ======================= */
// AUX SWITCH ID Defines
const int ASO         = 0;

const int MAX_NUM_COLORS = 6;
const int MAX_NUM_SWITCHES = 4;

typedef struct modeParams {
   int	modeId;
   char	modeName[20];
   int	numOfColors;       //Tell the android app home many colors to expect. Max number is 6
   int  numOfSwitches;
   bool textInput;   
} modeParams;

typedef struct switchParams {
   int  modeId;
   char switch1Title[20];
   char switch2Title[20];
   char switch3Title[20];
   char switch4Title[20];
} switchParams;

typedef struct auxSwitchParams {
    int      auxSwitchId;
    bool     auxSwitchState;
    char     auxSwitchTitle[20];
    char     auxSwitchOnName[20];
    char     auxSwitchOffName[20];
} auxSwitchParams;

/** An RGB color. */
typedef struct Color {
  unsigned char red, green, blue;

  Color(int r, int g, int b) : red(r), green(g), blue(b) {}
  Color() : red(0), green(0), blue(0) {}
} Color;

/** A point in 3D space. */
typedef struct Point {
  float x;
  float y;
  float z;
  Point() : x(0), y(0), z(0) {}
  Point(float _x, float _y, float _z) : x(_x), y(_y), z(_z) {}
} Point;

/** A 3D RGB voxel. */
typedef struct Voxel {
    Point coordinates;  // 12 Bytes
    Color color;        // 3 Bytes
    Voxel() : coordinates(), color() {}
} Voxel;

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
const Color incandescent    = Color(0xfd, 0xf5, 0xe6);  //This seems closer to incandescent color
const Color magenta         = Color(0xc2, 0x00, 0x78);  //#FF00FF
const Color orange          = Color(0xf9, 0x73, 0x06);  //#FFA500
const Color teal            = Color(0x02, 0x93, 0x86);
const Color cyan            = Color(0x02, 0xff, 0xff);
const Color red             = Color(0xe5, 0x00, 0x00);
const Color brown           = Color(0x65, 0x37, 0x00);
const Color pink            = Color(0xff, 0x81, 0xc0);
const Color lightpink       = Color(0xff, 0xc0, 0xcb);
const Color blue            = Color(0x03, 0x43, 0xdf);
const Color green           = Color(0x15, 0xb0, 0x1a);
const Color purple          = Color(0x7e, 0x1e, 0x9c);  //#800080
const Color white           = Color(0xff, 0xff, 0xff);

/* ======================= ADD NEW MODE STRUCT HERE. ======================= */
//modeId and modeName should be the same name to prevent confusion
//Use this struct array to neatly organize and correlate Mode name with number of colors needed
//The Android app uses numOfColors to help populate the view 
//and to know how many colors to ask to update
modeParams modeStruct[] =
{
    /*     modeId                       modeName                #Colors     #Switches   textInput
     *     --------------- 	            ---------------	        ---------   ---------   --------- */
        {  STANDBY,                     "OFF",                  0,          0,      FALSE   },  //credit: Kevin Carlborg
        {  NORMAL,                      "LIGHT",                0,          0,      FALSE   },  //credit: Kevin Carlborg
        {  ACIDDREAM,                   "ACID DREAM",           0,          0,      FALSE   },  //credit: Werner Moecke
        {  COLORBREATHE,                "BREATHE",              0,          0,      FALSE   },  //credit: Werner Moecke
        {  RAINBOW_BURST,               "BURST",                0,          0,      FALSE   },  //credit: Werner Moecke
        {  CHASER,                      "CHASER",               1,          0,      FALSE   },  //credit: Kevin Carlborg
  		{  CHEERLIGHTS,                 "CHEERLIGHTS",          0,          0,      FALSE   },  //credit: Alex Hornstein, Werner Moecke (stability fixes, extra transition effects)
        {  CHRISTMASLIGHTS,             "CHRISTMAS LIGHTS",     0,          0,      FALSE   },  //credit: Kevin Carlborg, Werner Moecke (L3D Cube port)
        {  CHRISTMASTREE,               "CHRISTMAS TREE",       0,          3,      FALSE   },  //credit: Kevin's friggin' xmas tree - there, have it!
        {  COLLIDE,                     "COLLIDE",              0,          0,      FALSE   },  //credit: Kevin Carlborg
        {  COLORALL,                    "COLOR ALL",            1,          0,      FALSE   },  //credit: Kevin Carlborg
        {  CHRISTMASWREATH,             "COLOR WREATH",         2,          0,      FALSE   },  //credit: Kevin Carlborg, Werner Moecke (L3D Cube port, extra colors)
        {  CUBES,                       "CUBES",                4,          4,      FALSE   },  //credit: Alex Hornstein, Werner Moecke (C++ port, extra settings)
        {  CUBE_CLASSICS,               "CUBE CLASSICS",        1,          1,      FALSE   },  //credit: http://www.instructables.com/id/Led-Cube-8x8x8/, Kevin Carlborg (L3D Cube port)
        {  CUBE_PAINTER,                "CUBE PAINTER",         0,          0,      FALSE   },  //credit: Werner Moecke (based on idea by Alex Hornstein)
        {  DIGI,                        "DIGI",                 1,          2,      FALSE   },  //credit: Kevin Carlborg
        {  TWOCOLORCHASE,               "DUAL CHASE",           2,          0,      FALSE   },  //credit: Werner Moecke
        {  FILLER,                      "FILLER",               3,          1,      FALSE   },  //credit: Werner Moecke (based on idea by Alex Hornstein)
        {  FLICKER,                     "FLICKER",              1,          0,      FALSE   },  //credit: Werner Moecke
        {  FROZEN,                      "FROZEN",               0,          0,      FALSE   },  //credit: Kevin Carlborg, Werner Moecke (flake fading)
        {  IFTTTWEATHER ,               "IFTTT WEATHER",        0,          0,      FALSE   },  //credit: Kevin Carlborg
        {  LISTENER,                    "LISTENER",             0,          0,      FALSE   },  //credit: Werner Moecke
        {  PLASMA,                      "PLASMA",               0,          0,      FALSE   },  //credit: Alex Hornstein, Werner Moecke (speed settings)
        {  POLICELIGHTS,                "POLICE",               0,          0,      FALSE   },  //credit: Werner Moecke
        {  COLORPULSE,                  "PULSE",                0,          0,      FALSE   },  //credit: Werner Moecke
        {  RAIN,                        "RAIN",                 1,          3,      FALSE   },  //credit: Kevin Carlborg, Werner Moecke (Matrix Mode)
        {  RAINBOW,                     "RAINBOW",              0,          0,      FALSE   },  //credit: Kevin Carlborg
        {  SQUARRAL,                    "SQUARRAL",             0,          0,      FALSE   },  //credit: Alex Hornstein
        {  SPECTRUM,                    "SPECTRUM",	            0,          2,      FALSE   },  //credit: Alex Hornstein, Werner Moecke (extra settings)
        {  COLORSTRIPES,                "STRIPES",              0,          0,      FALSE   },  //credit: Werner Moecke
        {  TEXTMARQUEE,                 "TEXT MARQUEE",	        2,          3,      TRUE    },  //credit: Alex Hornstein, Hans-Peter "Hape", Werner Moecke (C++ port, extra settings)
        {  TEXTSCROLL,                  "TEXT SCROLL",	        2,          3,      TRUE    },  //credit: Alex Hornstein, Hans-Peter "Hape", Werner Moecke (C++ port, extra settings)
        {  TEXTSPIN,                    "TEXT SPIN",            2,          3,      TRUE    },  //credit: Alex Hornstein, Hans-Peter "Hape", Werner Moecke (C++ port, extra settings)
        {  THEATERCHASE,                "THEATER CHASE",        0,          0,      FALSE   },  //credit: Kevin Carlborg
        {  COLORFADE,                   "TRANSITION",           0,          0,      FALSE   },  //credit: Werner Moecke
        {  WARMFADE,                    "WARM FADE",            0,          0,      FALSE   },  //credit: Kevin Carlborg
        {  WHIRLWIND,                   "WHIRLWIND",            0,          0,      FALSE   },  //credit: Bill Marrs
        {  ZONE,                        "ZONE",				    4,          3,      FALSE   },  //credit: Kevin Carlborg
        {  ZONECHASER,                  "ZONE CHASE",			4,          0,      FALSE   }   //credit: Werner Moecke
};

switchParams switchTitleStruct[] = 
{ 
    /*  modeId           S1Title                S2Title                S3Title                S4Title 
     *  ---------------  ---------------------- ---------------------- ---------------------- ----------------------  */
	   {  SPECTRUM,      "Smooth",              "Peak Mode",           "",                    ""                     },
	   {  TEXTMARQUEE,   "Bold Text",           "No Background",       "Black Text",          ""                     },
	   {  TEXTSPIN,      "Bold Text",           "No Background",       "Black Text",          ""                     },
	   {  TEXTSCROLL,    "Bold Text",           "No Background",       "Black Text",          ""                     },
	   {  CHRISTMASTREE, "Make it Snow",        "Pulse the Star",      "Lights On",           ""                     },
	   {  CUBES,         "Fill Cubes",          "Random Colors",       "Bleed Edge Color",    "Bleed Main Color"     },
	   {  RAIN,          "Random Colors",       "Matrix Mode",         "Fade Bottom",         ""                     },
	   {  FILLER,        "Random Colors",       "",                    "",                    ""                     },
	   {  ZONE,          "Loop",                "Random Colors",       "Coordinated Colors",  ""                     },
	   {  DIGI,          "Random Color Fill",   "Fade In",             "",                    ""                     },
	   {  CUBE_CLASSICS, "Color Sweep",         "",         "",                    ""                     }
};

/* ======================= ADD NEW AUX SWITCH STRUCT HERE. ======================= 
 * Use these switches to turn things on and off or toggle bewteen two options
 * @Param auxSwitchId        The ID of the switch     
 * @Param auxSwitchState     set the default switch state here
 * @Param auxSwitchTitle     Title/Description of the switch read in by the app
 * @Param auxSwitchOnTitle   Title of the ON/TRUE state of the switch read in by the app
 * @Param auxSwitchOffTitle  Title of the OFF/FALSE state of the switch read in by the app
 */
auxSwitchParams auxSwitchStruct[] = 
{ 
     /*    auxSwitchId      auxSwitchState  auxSwitchTitle         auxSwitchOnTitle       auxSwitchOffTitle    
     *     ---------------  --------------  ---------------------- ---------------------- ----------------------*/
	    {  ASO,             TRUE,          "Auto Shut Off",       "ON",                  "OFF"                }, 
	    //{  LIGHTSENSOR,     TRUE,           "Brightness Control",  "Light Sensor",        "App Controlled"     }, //Shown here as an example
};

/* ========================= Local Aux Switch variables =========================== */
bool autoShutOff;   //Should we shut off the lights at certain times? This is toggled from the app
                    //Configure the Auto Shut Off times in the loop() function 

//Preset speed constants
const int speedPresets[] = {120, 100, 80, 70, 50, 30, 20, 10, 1};  //in mSec, slow to fast       

//Time Interval constants            hh*mm*ss*ms    
unsigned long oneMinuteInterval =     1*60*1000;	//Read temp every minute
unsigned long twoMinuteInterval =     2*60*1000;	//Change mode every 2 minutes in demo
unsigned long oneHourInterval =       1*60*60*1000;  //auto off in 1 hr when night time
unsigned long oneDayInterval = 	     24*60*60*1000;  //time sync interval - 24 hours
unsigned long iftttWeatherInterval = 10*60*1000;  //revert back to last mode for IFTTT Weather
unsigned long start;

//Program Flags
int currentModeID;  //This is the ID of the current mode selected - used in the case statement
int lastModeID;
bool run;           //Use this for modes that don't need to loop. Set the color, then stop sending commands to the pixels
bool stop;          //Use this to break out of sequence loops when changing to a new mode
bool demo;          //Use this to enable/disable the demo sequence playback 
bool isFirstLap;

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
uint8_t colorWheel;
//Variables to hold the switch settings to each mode requiring them
bool switch1, switch2, switch3, switch4;
int lastRand = 0;
int lastLastRand = 0;
int timeZone = TIME_ZONE_OFFSET;
Color lastCol;

//Particle Cloud Variables
//int wifi = 0;           //used for general info and setup
int tHour = 0;          //used for general info and setup
int speedIndex;         //Let the cloud know what speed preset we are using
int brightness;         //How bright do we want these things anyway
char modeNameList[MAX_PUBLISHED_STRING_SIZE] = "None";  //Holds all mode info comma delimited. Use this to populate the android app
char modeParamList[MAX_PUBLISHED_STRING_SIZE] = "None";
char auxSwitchList[MAX_PUBLISHED_STRING_SIZE] = "None";
char currentModeName[64] = "None";  //Holds current selected mode
char textInputString[64];           //Holds the Text for any mode needing a test input - only useful for a Neopixel Matrix
char debug[200];                    //We might want some debug text for development
bool varsRegistered = FALSE;        //Flags if we have all of our cloud variables registered successfully

/* ======================= Mode-specific Defines ======================= */
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

// Some effects can render on different axis
// for example send pixels along an axis
// for better readability, we use the following predefined constants
#define AXIS_X 0x78
#define AXIS_Y 0x79
#define AXIS_Z 0x7a

/* ========================== LISTENER mode defines ========================== */
UDP Udp;        //an UDP instance to let us receive packets over UDP
int countdown;  // Keep a watchdog count to 100 max failed UDP buffer read attempts
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
// Package size we expect. The footer byte is included here!
#define EXPECTED_PACKET_SIZE    (PIXELS_PER_PANEL * BPP + TPM2NET_HEADER_SIZE) + 1  // 198 Data bytes + footer byte

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
char message[64]=" ";
int thickness=0;
float pos=0;
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
int idex, ihue; //We define these here as they serve to flag if we need
                //to blank the cube every time the mode is called

/* ============================ SQUARRAL mode Defines =========================== */
#define TRAIL_LENGTH 50
int frame;
int bound;
int boundInc;
int squarral_zInc;
bool rainbowColor;
unsigned char axis;
Point position, increment, pixel;
Point trailPoints[TRAIL_LENGTH];

/* ============================= PLASMA mode Defines ============================ */
float phase, colorStretch;

/* ========== AUTO SHUT OFF (ASO) Defines for Cloud Function: Function ========== */
/*#define ASO_CMD_ON              "ASO_ON"
#define ASO_CMD_OFF             "ASO_OFF"
#define ASO_CMD_STATUS          "ASO_STATUS"
#define ASO_STATUS_OFF          2000
#define ASO_STATUS_ON           2001*/

/* ========================= Transition function defines ======================== */
#define LINEAR  0
#define POLAR   1

/* ========================== SetMode return  Defines  ========================== */
#define NO_CHANGE               1000
#define BRIGHTNESS_SET          1001
#define SPEED_SET               1002

/* ========================= christmasTree mode Defines  ======================== */
#define PI 3.1415926535

/* =========================== whirlWind mode Defines =========================== */
#define CYCLE_INTERVAL          60000 // milliseconds between restart
#define MAX_DOTS                19
#define                         MIN_RADI 1
#define                         MAX_RADI 5
Color clr[MAX_DOTS];
float angle[MAX_DOTS];
float radi[MAX_DOTS];
float y[MAX_DOTS];
int arcs = 180; 
Point center = { 4.5, 4.5, 4.5 };
unsigned long lastSwap;

/* ============================= Cubes mode Defines ============================= */
int side, inc, mode;
bool flipColor;
Color cubeCol;

/* ========================= Cheerlights mode defines ======================== */
#define POLLING_INTERVAL 3000   // how often the photon polls the cheerlights API
#define RESPONSE_TIMEOUT 500	// the timeout (in ms) to wait for a response from the cheerlights API
TCPClient client;       // a TCP instance to let us query the cheerlights API over TCP
String hostname, path;  // the URL and path to cheerlights' thingspeak directory
String response;        // the response read from querying cheerlights' thingspeak directory
bool connected;         // flag if we have a solid TCP connection
bool cheerLightsEnabled;
Color cheerLightsColor;
int requestTime, pollTime;
Thread* cheerlightsThread;  //https://community.particle.io/t/particle-photon-multi-blink-sample-using-threads/16214/3
//https://github.com/pipprojects/WM/blob/master/water-meter-2.ino

/* ============================ Required Prototypes ============================= */
int showPixels(void);
int hexToInt(char val);
int antipodal_index(int i);
int SetASO(String command);
int setNewMode(int newMode);
int updateAuxSwitches(int id);
int collapsingSides(Color col);
int getModeIndexFromID(int id);
int randomPixelFill(uint32_t c);
int isValidMode(String newMode);
int getAuxSwitchIndexFromID(int id);
int stackingRope(int mode, Color col);
int getModeIndexFromName(String name);
int ripples (int iterations, Color col);
int getSwitchTitleStructIndex(int modeId);
int spheremove (int iterations, Color col);
int fireworks (int iterations, int n, Color col);
int linespin (int iterations, char axis, Color col);
int sinelines (int iterations, char axis, Color col);
void fillX(Color col);
void fillY(Color col);
void fillZ(Color col);
void makeModeList(void);    //Added new function to make mode and parameter lists
void initMicrophone(void);
void getCheerlights(void);
//void initCheerLights(void);
void background(Color col);
bool isWhiteColor(Color col);
void makeAuxSwitchList(void);
void add(Point& a, Point& b);
void publishCloudVariables(void);
void resetVariables(int modeIndex);
void randomColor(struct Color *clr);
void setPixelColor(Point p, Color col);
void transition(Color bgcolor, bool loop);
void drawLine(Point p1, Point p2, Color col);
void mixVoxel(Point currentPoint, Color col);
void fadeInToColor(uint32_t index, Color col);
void random_seed_from_cloud(unsigned int seed);     //Disable random seed from the cloud 
void fadeOutFromColor(uint32_t index, Color col);
void drawCube(Point topLeft, int side, Color col);
void setPixelColor(int x, int y, int z, Color col);
void transitionAll(Color endColor, uint16_t method);
void arrayShuffle(int arrayToShuffle[], int arraySize);
void transitionOne(Color endColor, uint16_t index, uint16_t method);
void drawSolidHorizontalCircle(int xOrigin, int yOrigin, int z, int radius, Color col);
void drawHollowHorizontalCircle(int xOrigin, int yOrigin, int z, int radius, Color col, bool rndColor);
void transitionHelper(Color startColor, Color endColor, uint16_t index, uint16_t method, int16_t numSteps, int16_t step);
bool isThereEnoughRoomInModeParamList(int textSize);
float squareRoot(float x);
float randomDecimal(void);
float radius(float x, float y, float z);
float distance2d (float x1, float y1, float x2, float y2);
float distance3d (float x1, float y1, float z1, float x2, float y2, float z2);
Color getPixelColor(Point p);
Color getPixelColor(int index);
Color complement(Color original);
Color getColorFromInteger(uint32_t col);
Color getPixelColor(int x, int y, int z);
Color fadeColor(Color col, float scaleFactor);
uint8_t fadeSqRt(float value);
uint8_t fadeSquare(float value);
uint8_t fadeLinear(float value);
uint8_t clamp(unsigned value, unsigned lowClamp, unsigned highClamp);
int16_t getTransitionStep(Color startColor, Color endColor, uint16_t method, int16_t numSteps, int16_t step, Color whichColor);
uint32_t getHighestValFromRGB(Color col);
uint32_t Wheel(byte WheelPos, float opacity=1.0);
uint32_t colorMap(float val, float minVal, float maxVal);
uint32_t lerpColor(uint32_t c1, uint32_t c2, uint32_t val, uint32_t minVal, uint32_t maxVal);

/* ======================= Spark Pixel Prototypes =============================== */
int CubePainter(String command);
int shift (char axis, int direction);
int effect_loadbar(int axis, Color col);
int effect_planboing (int plane, Color col);
int effect_z_updown (int iterations, Color col);
int effect_telcstairs_do(int x, int val, Color col);
int effect_telcstairs (int invert, int val, Color col);
int colorZone(uint32_t c1, uint32_t c2, uint32_t c3, uint32_t c4, bool loop);
int effect_axis_updown_randsuspend (char axis, int sleep, int invert, Color col);
int effect_boxside_randsend_parallel (char axis, int origin, int mode, Color col);
int effect_wormsqueeze (int size, int axis, int direction, int iterations, Color col);
int draw_positions_axis (char axis, unsigned char positions[64], int invert, Color col);
int boingboing(uint16_t iterations, unsigned char mode, unsigned char drawmode, Color col);
int effect_z_updown_move (unsigned char positions[64], unsigned char destinations[64], char axis, Color col);
void FFTJoy(void);
void frozen(void);
void listen(void);
void collide(void);
void cubeInc(void);
void rainbow(void);
void runMode(void);
void runDemo(void);
void zPlasma(void);
void squarral(void);
void warmFade(void);
void whirlWind(void);
void cycleLerp(void);
void color_fade(void);
void colorPulse(void);
void modeButton(void);
void cheerlights(void);
void smoothSwitch(void);
void colorStripes(void);
void random_burst(void);
void rainbowCycle(void);
void christmasTree(void);
void christmasLights(void);
void digi(uint32_t color1);
void pulse_oneColorAll(void);
void police_light_strobo(void);
void theaterChaseRainbow(void);
void rain(uint32_t c);
void flicker(uint32_t c);
void colorChaser(uint32_t c);
void iftttWeather(uint32_t c);
void runCubeClassics(uint32_t c);
void setplane_x (int x, Color col);
void setplane_y (int y, Color col);
void setplane_z (int z, Color col);
void textSpin(uint32_t color1, uint32_t color2);
void textScroll(uint32_t color1, uint32_t color2);
void findRandomSnowFlakesPositions(int numFlakes);
void flipVoxel(int x, int y, int z, Color newCol);
void filler(uint32_t c1, uint32_t c2, uint32_t c3);
void textMarquee(uint32_t color1, uint32_t color2);
void setplane (char axis, unsigned char i, Color col);
void twoColorChaser(uint32_t color1, uint32_t color2);
void christmasWreath(uint32_t color1, uint32_t color2);
void cubeGreeting(int textMode, int frameCount, float pos);
void cubes(uint32_t c1, uint32_t c2, uint32_t c3, uint32_t c4);
void colorZoneChaser(uint32_t c1, uint32_t c2, uint32_t c3, uint32_t c4);
short FFT(short int dir,int m,float *x,float *y);

void setup() {
    publishCloudVariables();
    pinMode(PIXEL_PIN, OUTPUT);

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
    switch1 = FALSE;
    switch2 = FALSE;
    switch3 = FALSE;
    switch4 = FALSE;
    autoShutOff = FALSE;    //Initialize auto shut off mode variable
	currentModeID = getModeIndexFromID(NORMAL);
	defaultColor = strip.Color(incandescent.red, incandescent.green, incandescent.blue);
	snowFlakeColor = getColorFromInteger(0xFFFFFF);
	lastSync = lastCommandReceived = previousMillis = millis(); //Take a time stamp
    
    c1 = Wheel(random(random(2, 256), random(2, 256)));
    c2 = Wheel(random(random(2, 256), random(2, 256)));

	//Start up the Neopixels
	strip.begin();
    
    //Set time zone
    if(Particle.connected()) {Time.zone(TIME_ZONE_OFFSET);}
    
    //Clear the mode list and mode param list variables
	sprintf(modeNameList,"");
	sprintf(modeParamList,"");
	sprintf(auxSwitchList,"");
    
    //Assemble Spark Cloud available modes variable
    makeModeList();
    makeAuxSwitchList();

    //Get Threads ready
    //cheerlightsThread = new Thread("Get CheerLights Color", LoopgetCheerLightsColor);
    
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

void publishCloudVariables() {
    varsRegistered = waitFor(Particle.connected, 5000);
    if(varsRegistered) {
        varsRegistered &= Particle.function("Function",     FnRouter);
        varsRegistered &= Particle.function("SetMode",      SetMode);
        varsRegistered &= Particle.function("SetText",      SetText);
        varsRegistered &= Particle.function("CubePainter",  CubePainter);
        varsRegistered &= Particle.variable("debug",        debug);
        //varsRegistered &= Particle.variable("wifi",         wifi);
        //varsRegistered &= Particle.variable("tHour",        tHour);
        varsRegistered &= Particle.variable("speed",        speedIndex);
        varsRegistered &= Particle.variable("brightness",   brightness);
    	varsRegistered &= Particle.variable("modeList",		modeNameList);
        varsRegistered &= Particle.variable("modeParmList",	modeParamList);
    	varsRegistered &= Particle.variable("auxSwtchList",  auxSwitchList);
    	varsRegistered &= Particle.variable("mode",         currentModeName);
    }
}

void makeModeList(void) {
    for(int i=0; i<sizeof modeStruct / sizeof modeStruct[0]; i++) {
        char cNameBuff[20];
		char cParamBuff[60];
		if(strlen(modeNameList)+strlen(modeStruct[i].modeName)+1 <= MAX_PUBLISHED_STRING_SIZE) {
            sprintf(cNameBuff,"%s;",modeStruct[i].modeName );
		    strcat(modeNameList,cNameBuff);
		}
		else {
		    sprintf(debug,"Error: modeNameList has reached max size limit");
		}
		
		if(modeStruct[i].numOfColors==0 && modeStruct[i].numOfSwitches==0 && modeStruct[i].textInput == FALSE) {
		    if(isThereEnoughRoomInModeParamList(2)) {
			    strcat(modeParamList,"N;");
		    } else { return; }
		}
		else {
			if(modeStruct[i].numOfColors > 0) {
				if(modeStruct[i].numOfColors > MAX_NUM_COLORS) {
					modeStruct[i].numOfColors = MAX_NUM_COLORS;
				}
				if(isThereEnoughRoomInModeParamList(4)) {
    				sprintf(cParamBuff,"C:%i",modeStruct[i].numOfColors);
    				strcat(modeParamList,cParamBuff);
    				if(modeStruct[i].numOfSwitches == 0 && modeStruct[i].textInput == FALSE) {
    					strcat(modeParamList,";");
    				} else { strcat(modeParamList,","); }
				} else { return; }
			}
			if(modeStruct[i].numOfSwitches > 0) {
			    int switchTitleStructIdx = getSwitchTitleStructIndex(modeStruct[i].modeId);
			    if(switchTitleStructIdx != -1) {
    				if(modeStruct[i].numOfSwitches > MAX_NUM_SWITCHES) {
    					modeStruct[i].numOfSwitches = MAX_NUM_SWITCHES;
    				}
    				if(modeStruct[i].numOfSwitches >= 1) {
    					sprintf(cParamBuff,"S:%i,\"%s\"",modeStruct[i].numOfSwitches,switchTitleStruct[switchTitleStructIdx].switch1Title);
                        //consider this instead: strncat(modeParamList,cParamBuff,MAX_PUBLISHED_STRING_SIZE-strlen(modeParamList)-1);
                        if(isThereEnoughRoomInModeParamList(strlen(cParamBuff)+1)) {
    					    strcat(modeParamList,cParamBuff);
                        } else { return; }
    				}
    				if(modeStruct[i].numOfSwitches >= 2) {
    					sprintf(cParamBuff,"\"%s\"",switchTitleStruct[switchTitleStructIdx].switch2Title);
    					if(isThereEnoughRoomInModeParamList(strlen(cParamBuff)+1)) {
    					    strcat(modeParamList,cParamBuff);
    					} else { return; }
    				}
    				if(modeStruct[i].numOfSwitches >= 3) {
    					sprintf(cParamBuff,"\"%s\"",switchTitleStruct[switchTitleStructIdx].switch3Title);
    					if(isThereEnoughRoomInModeParamList(strlen(cParamBuff)+1)) {
    					    strcat(modeParamList,cParamBuff);
    					} else { return; }
    				}
    				if(modeStruct[i].numOfSwitches >= 4) {
    					sprintf(cParamBuff,"\"%s\"",switchTitleStruct[switchTitleStructIdx].switch4Title);
    					if(isThereEnoughRoomInModeParamList(strlen(cParamBuff)+1)) {
                            strcat(modeParamList,cParamBuff);
    					} else { return; }
    				}
    			    if(modeStruct[i].textInput == FALSE) {
    					strcat(modeParamList,";");
    				}
    				else { strcat(modeParamList,","); }
                } else {
                    sprintf(cParamBuff,"S:E;");
                    if(isThereEnoughRoomInModeParamList(strlen(cParamBuff)+1)) {
                       strcat(modeParamList,cParamBuff);
                    } else { return; }
			    }
			}
			if(modeStruct[i].textInput == TRUE) {
			    if(isThereEnoughRoomInModeParamList(3)) {
				    strcat(modeParamList,"T:;");
			    } else { return; }
			}
		} 
    }
}

bool isThereEnoughRoomInModeParamList(int textSize) {
    if(strlen(modeParamList) + textSize + 1 <= MAX_PUBLISHED_STRING_SIZE) {
        return true;
    }
    int idx = strlen(modeParamList)-1;
	while(modeParamList[idx] != ',' && modeParamList[idx] != ';') {
		idx--;
	}
	modeParamList[idx] = ';';
    sprintf(debug,"Error: modeParamList has reached max size limit");
    return false;
}

int getSwitchTitleStructIndex(int modeId) {
    uint16_t i;
    for(i=0;i<sizeof switchTitleStruct / sizeof switchTitleStruct[0];i++) {
        if(switchTitleStruct[i].modeId == modeId)
            return i;
    }
    sprintf(debug,"Error: Missing Switch Titles for mode %s", modeStruct[getModeIndexFromID(modeId)].modeName);
    return -1;
}

// Uses the auxSwitchStruct to assemble the cloud attainable auxSwtchList variable
// Switch param order: "id,title,onName,offName,switchState;"
void makeAuxSwitchList(void) {
    sprintf(auxSwitchList,"");
    for(uint16_t i=0;i<sizeof auxSwitchStruct / sizeof auxSwitchStruct[0];i++) {
        char cNameBuff[62];
		if(strlen(auxSwitchList)+strlen(auxSwitchStruct[i].auxSwitchTitle)+strlen(auxSwitchStruct[i].auxSwitchOnName)+strlen(auxSwitchStruct[i].auxSwitchOffName)+9 <= MAX_PUBLISHED_STRING_SIZE) {
            sprintf(cNameBuff,"%i,%s,%s,%s,%i;",auxSwitchStruct[i].auxSwitchId,
                                                auxSwitchStruct[i].auxSwitchTitle,
                                                auxSwitchStruct[i].auxSwitchOnName,
                                                auxSwitchStruct[i].auxSwitchOffName,
                                                auxSwitchStruct[i].auxSwitchState ? 1 : 0 );
		    strcat(auxSwitchList,cNameBuff);
		}
		else {
		    sprintf(debug,"Error: auxSwitchList has reached max size limit");
		}
		
		//Update local Aux Switch variables
		if(0 == updateAuxSwitches(auxSwitchStruct[i].auxSwitchId))
		    sprintf(debug,"Error: auxSwitch failed to update local variable");
    }
}

/**Update local Aux Switch variables
 *  @id the Aux Switch ID to update
 *  @return 1 if successful
 *  @return -1 if Switch ID was not found
 */
int updateAuxSwitches(int id) {
    switch(id) {
        case ASO:
            autoShutOff = auxSwitchStruct[getAuxSwitchIndexFromID(id)].auxSwitchState;
            return 1;
    }
    return -1;
}

//delay (or speed) is passed 
void loop() {
    if(run) {
		stop = FALSE;
        if(demo) { runDemo(); }
		else { runMode(); }
    }
    
    unsigned long currentMillis = millis();
 
    if(currentMillis - previousMillis > oneMinuteInterval) {
        previousMillis = currentMillis;
        
        //At every minute, check for connection and register cloud variables
        if(Particle.connected()) {
            Time.zone(TIME_ZONE_OFFSET);    //Set time zone
            tHour = Time.hour();            //used to check for correct time zone
            //wifi = WiFi.RSSI();
        }
        if(!varsRegistered) {publishCloudVariables();}
        
        //Put other timing stuff in here to speed up main loop
        if (currentMillis - lastSync > oneDayInterval) {
            // Request time time synchronization from the Spark Cloud
            //sprintf(debug,"Last sync time = %i,", (int)(currentMillis - lastSync));
			Particle.syncTime();
            lastSync = currentMillis;
        }
        if (autoShutOff && (currentMillis - lastCommandReceived > oneHourInterval)) {
            //Auto Off Criteria
            //If it's Monday through Friday between 8am and 4pm or between 10pm and 5am any day, turn Off the lights
            if(((Time.weekday() >= 2 && Time.weekday() <=6) && (Time.hour() >= 8 && Time.hour() <= 18)) || (Time.hour() >= 23) || (Time.hour() <= 5)) {
                //No one is home or everyone is sleeping. So shut it down
				//sprintf(debug,"Last auto Off time = %i,", (int)(currentMillis - lastCommandReceived));
				lastCommandReceived = currentMillis;
				setNewMode(getModeIndexFromID(STANDBY));
                run = TRUE;
                demo = FALSE;
            }
        }
    }
}

//Disable random seed from the cloud
void random_seed_from_cloud(unsigned int seed) {
   // don't do anything with this. Continue with existing seed.
}

void runDemo() {
    static int textMode=0, frameCount=0, previousModeID=currentModeID;
    static int endOfMessage=0, cycleCount=-1;
    static float posInc=.2;
    static bool isTextDone=true;
    run = TRUE;

    if(stop || !demo) {run = TRUE; demo = FALSE; return;}

    if(isTextDone && (millis() - lastModeSet > twoMinuteInterval)) {
        color1 = Wheel(random(random(2, 256), random(2, 256)));
        color2 = Wheel(random(random(2, 256), random(2, 256)));
        color3 = Wheel(random(random(2, 256), random(2, 256)));
        color4 = Wheel(random(random(2, 256), random(2, 256)));
        
        cycleCount++;
        if(cycleCount >= (sizeof(modeStruct) / sizeof(modeStruct[0]))-4) {
            srand(analogRead(MICROPHONE));
            cycleCount = 0;
            textMode = 0;
        }
        do {
            int i = random(0, (int)(sizeof(modeStruct) / sizeof(modeStruct[0])));
            currentModeID = modeStruct[i].modeId;
        }while((currentModeID == previousModeID)                   || 
               (currentModeID == getModeIndexFromID(NORMAL))       || 
               (currentModeID == getModeIndexFromID(STANDBY))      || 
               (currentModeID == getModeIndexFromID(COLORALL))     ||
               (currentModeID == getModeIndexFromID(TEXTSPIN))     || 
               (currentModeID == getModeIndexFromID(TEXTSCROLL))   || 
               (currentModeID == getModeIndexFromID(TEXTMARQUEE))  || 
               (currentModeID == getModeIndexFromID(CHEERLIGHTS))  || 
               (currentModeID == getModeIndexFromID(CUBE_PAINTER)) ||
               (currentModeID == getModeIndexFromID(IFTTTWEATHER)) ||
               (currentModeID == getModeIndexFromID(LISTENER)));
        //sprintf(debug, "currentModeID: %d", currentModeID);
        
        previousModeID = currentModeID;
        setNewMode(currentModeID);

		frameCount = 0;
        isTextDone = FALSE;
        transition(black, true);
    }
    
    while(!isTextDone) {
        if(stop || !demo) {run = TRUE; demo = FALSE; return;}
        if(textMode <= 2) {endOfMessage = 13;}
        if(textMode == 3) {endOfMessage = strlen(message)*8;}
        if(textMode >= 4) {endOfMessage = SIDE*map(strlen(message), 1, 63, 1, SIDE)+(strlen(message))*8;}
        cubeGreeting(textMode, frameCount, pos);

        frameCount++;
        pos += posInc;
        if (pos >= endOfMessage) {
            if(textMode <= 5) 
                pos = map(strlen(message), 1, 63, -(SIDE*.5), 0);
            else
                pos = map(strlen(message), 1, 63, -SIDE, 0);
            textMode++;
            if(textMode > 6) {
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
		    transitionAll(black,LINEAR);	//transition(black, true);  //colorAll(0, demo);
			run = FALSE;
		    break;
		case ACIDDREAM:
		    cycleLerp();
		    break;    
     	case CHASER:
		    colorChaser(color1);
			break;
     	case CHEERLIGHTS:
		    cheerlights();
			break;
		case CHRISTMASLIGHTS:
		    christmasLights();
		    break;
		case CHRISTMASTREE:
		    christmasTree();
		    break;
		case CHRISTMASWREATH:
		    christmasWreath(color1, color2);
		    break;
		case COLLIDE:
		    collide();
		    break;
     	case COLORALL:
	        transitionAll(getColorFromInteger(color1),POLAR);	//transition(getColorFromInteger(color1), false);  //colorAll(color1, demo);
	        break;
     	case COLORBREATHE:
	        pulse_oneColorAll();
	        break;
		case COLORFADE:
		    color_fade();
		    break;
		case COLORPULSE:
		    colorPulse();
		    break;
		case COLORSTRIPES:
		    colorStripes();
		    break;
		case CUBE_CLASSICS:
		    runCubeClassics(color1);
		    break;
		case CUBE_PAINTER:
		    // Nothing to do; function is called through the Cloud API
		    showPixels();
		    delay(100);
		    break;
		case CUBES:
		    cubes(color1, color2, color3, color4);
		    break;
	    case DIGI:
		    digi(color1);
		    break;
		case FILLER:
	        filler(color1, color2, color3); 
	        break;
     	case FLICKER:
		    flicker(color1);
			break;
		case FROZEN:
		    frozen();
		    break;
		case IFTTTWEATHER:
		    iftttWeather(color6);
    	    break;
		case LISTENER:
		    listen();
		    break;
		case PLASMA:
		    zPlasma();
		    break;
		case POLICELIGHTS:
		    police_light_strobo();
		    break;
		case RAIN:
		    rain(color1);
		    break;
		case RAINBOW:
		    rainbowCycle();
		    break;
		case RAINBOW_BURST:
		    random_burst();
		    break;
		case SPECTRUM:
		    FFTJoy();
		    break;
		case SQUARRAL:
		    squarral();
		    break;
     	case TEXTMARQUEE:
		    textMarquee(color1, color2);
			break;
     	case TEXTSCROLL:
		    textScroll(color1, color2);
			break;
     	case TEXTSPIN:
		    textSpin(color1, color2);
			break;
		case THEATERCHASE:
		    theaterChaseRainbow();
		    break;
		case TWOCOLORCHASE:
		    twoColorChaser(color1, color2);
		    break;
		case WARMFADE:
		    warmFade();
		    break;
		case WHIRLWIND:
		    whirlWind();
		    break;
		case ZONE:
	        colorZone(color1, color2, color3, color4, (demo || switch1)); 
	        break;
		case ZONECHASER:
	        colorZoneChaser(color1, color2, color3, color4); 
	        break;
		case NORMAL:
		default:
		    transitionAll(incandescent,LINEAR); //transition(incandescent, false);    //colorAll(defaultColor, demo);
			break;
    }    
}

void resetVariables(int modeIndex) {
    switch (modeIndex) {
		case CHEERLIGHTS:
		    hostname = "api.thingspeak.com";
		    path = "/channels/1417/field/2/last.txt";
		    response = "";
		    pollTime = millis() + POLLING_INTERVAL;
		    lastCol = black;
        	client.stop();
		    connected = client.connect(hostname, 80);
		    break;
        case FILLER:
		    lastCol = black;
			transitionAll(lastCol,LINEAR);
		    break;
		case LISTENER:
	    {
		    /*** DEBUG ***/
            //IPAddress myIp = WiFi.localIP();
            //sprintf(debug, "%d.%d.%d.%d", myIp[0], myIp[1], myIp[2], myIp[3]);
            
		    countdown = 0;
        	Udp.stop();
            while(!Udp.setBuffer(EXPECTED_PACKET_SIZE)) { /* Start the UDP */ }
            Udp.begin(TPM2NET_LISTENING_PORT);
		    break;
	    }
		case CUBES:
            side=0;
            inc=1;
            mode=0;
            flipColor = TRUE;
		    transitionAll(black,LINEAR);	//fadeToBlack();
		    break;
		case CHRISTMASTREE:
		    isFirstLap = TRUE;
		    transitionAll(black,LINEAR);	//fadeToBlack();
		    break;
		case PLASMA:
            phase = 0.0;
            colorStretch = 0.23;    // Higher numbers will produce tighter color bands 
		    transitionAll(black,LINEAR);	//fadeToBlack();
		    break;
		case RAINBOW_BURST:
            idex = 0;
            ihue = 0;
		    break;
		case TEXTMARQUEE:
		case TEXTSCROLL:
            sprintf(message, textInputString);
            pos = map(strlen(message), 1, 63, -(SIDE*.5), 0); 
            transitionAll(black,LINEAR);	//fadeToBlack();
            if(!switch2)
                transitionAll(getColorFromInteger(color2), LINEAR);
		    break;
		case TEXTSPIN:
            sprintf(message, textInputString);
            pos = map(strlen(message), 1, 63, -SIDE, 0);
            transitionAll(black,LINEAR);	//fadeToBlack();
            if(!switch2)
                transitionAll(getColorFromInteger(color2), LINEAR);
		    break;
		case SQUARRAL:
            frame = 0;
            bound = 0;
            axis = 0;
            boundInc = 1;
            squarral_zInc = 1;
            position = {0,0,0};
            increment = {1,0,0};
		    transitionAll(black,LINEAR);	//fadeToBlack();
		    break;
		case WHIRLWIND:
		    lastSwap = millis();
            for (int i=0; i<MAX_DOTS; i++) {
                y[i] = random(SIDE);
                radi[i] = random(MIN_RADI,MAX_RADI) + randomDecimal();
                angle[i] = randomDecimal() * 2 * PI;
                //clr[i] = Color(rand()%16, rand()%16, rand()%16);
                randomColor(&clr[i]);
            }
            transitionAll(black,LINEAR);	//fadeToBlack();
		    break;
		case DIGI:
		case IFTTTWEATHER:
		case CUBE_PAINTER:
     	case COLORALL:
     	case COLORBREATHE:
     	case CHASER:
		case ZONE:
		case ZONECHASER:
		case COLORPULSE:
		case COLORSTRIPES:
		case ACIDDREAM:
		case RAIN:
		case RAINBOW:
		case SPECTRUM:
		case THEATERCHASE:
		case FROZEN:
		case COLLIDE:
		case POLICELIGHTS:
		case TWOCOLORCHASE:
		case COLORFADE:
		case CHRISTMASLIGHTS:
		case CHRISTMASWREATH:
		case WARMFADE:
     	case FLICKER:
        case STANDBY:
		case NORMAL:
		default:
			transitionAll(black,LINEAR);
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
            if(Particle.connected() && varsRegistered)
                sprintf(message, "Select a mode from the Spark Pixels app");
            else
                sprintf(message, "Not connected to Cloud - modes unavailable");
            scrollText(message, Point(pos - strlen(message), 0, 6), getColorFromInteger(Wheel(frameCount%500)));
            break;  
        case(5):
            thickness = 1;
            sprintf(message, "Now showcasing...");
            scrollText(message, Point(pos - strlen(message), 0, 6), getColorFromInteger(Wheel(frameCount%500)));
            break;  
        case(6):
            thickness = 1;
            sprintf(message, currentModeName);
            scrollSpinningText(message, Point(pos - strlen(message), 0, ceil((SIDE-1)*.5)), getColorFromInteger(Wheel(frameCount%500)));
            break;  
    }
    showPixels();
}

/** 
 * Used to make linear transitions between colors by drawing a linear line from
 * the current color to the end color in the HSV model.
 * Assumes every pixel is at a different color value.
 * If the current color is blue and the next color is yellow, it will pass 
 * through the white spectrum to get there.
 * If the current color is red and the next color is blue, it will pass 
 * through pink/purple to get there.
 */
void transitionAll(Color endColor, uint16_t method) {
    int numSteps = 8;   //This could be an input param to this function
    uint32_t startColor[strip.numPixels()];
    run = FALSE;
    
    //Save the start color for each pixel - yeah, I know this is using a lot of memory, but I'm not smart enough to do it a better way
    for(int index = 0; index < strip.numPixels(); index++) {
        startColor[index] = strip.getPixelColor(index);
    }

    for(int i=1; i<=numSteps; i++) {
        for(int index = 0; index < strip.numPixels(); index++) {
            transitionHelper(getColorFromInteger(startColor[index]), endColor, index, method, numSteps, i); 
            if(stop) {return;}
        }
        showPixels();
        delay(speed);
    }
}

//Same as transitionAll but only for one pixel
void transitionOne(Color endColor, uint16_t index, uint16_t method) {
    int numSteps = 8;   //This could be an input param to this function
    Color startColor = getColorFromInteger(strip.getPixelColor(index));

    for(int i=1; i<=numSteps; i++) {
        transitionHelper(startColor, endColor, index, method, numSteps, i);
        if(stop) {return;}
        showPixels();
        delay(speed);
    }
}

//Used to set the next color step for transitionAll and transitionOne
void transitionHelper(Color startColor, Color endColor, uint16_t index, uint16_t method, int16_t numSteps, int16_t step) {
    Color col2;

    //Find the step
    int16_t redStep   = getTransitionStep(startColor, endColor, method, numSteps, step, red);
    int16_t greenStep = getTransitionStep(startColor, endColor, method, numSteps, step, green);
    int16_t blueStep  = getTransitionStep(startColor, endColor, method, numSteps, step, blue);

    //Add the increment to get the next color segments
    //If new color is a higher value, set the high clamp to the new color
    //If new color is a smaller value, set the low clamp to the new color
    if(endColor.red   > startColor.red)   col2.red   = clamp(startColor.red   + redStep,  0,             endColor.red);
	else                                  col2.red   = clamp(startColor.red   + redStep,  endColor.red,  0xFF);
	if(endColor.green > startColor.green) col2.green = clamp(startColor.green + greenStep,0,             endColor.green);
	else                                  col2.green = clamp(startColor.green + greenStep,endColor.green,0xFF);
	if(endColor.blue  > startColor.blue)  col2.blue  = clamp(startColor.blue  + blueStep, 0,             endColor.blue);
	else                                  col2.blue  = clamp(startColor.blue  + blueStep, endColor.blue, 0xFF);
	
    //Let's make sure we hit the target
    if(step == numSteps) {
        col2.red   = endColor.red;
        col2.green = endColor.green;
        col2.blue  = endColor.blue;
    }
    
    strip.setPixelColor(index, strip.Color(col2.red, col2.green, col2.blue));
}

//Used to get the next color step for transitionHelper()
int16_t getTransitionStep(Color startColor, Color endColor, uint16_t method, int16_t numSteps, int16_t step, Color whichColor) {
	int16_t increment=0;

    if(method == LINEAR) {
        if(whichColor == red)        increment = (step * (endColor.red-startColor.red))     / numSteps;
        else if(whichColor == green) increment = (step * (endColor.green-startColor.green)) / numSteps;
        else if(whichColor == blue)  increment = (step * (endColor.blue-startColor.blue))   / numSteps;
    }
	else { // Not Quite POLAR     
	    if(whichColor == red) {
	        if(endColor.red < startColor.red)
	            increment = sqrt(float(step)/numSteps) * float(endColor.red-startColor.red);
	        else
	            increment = (float(step)/numSteps) * (float(step)/numSteps) * float(endColor.red-startColor.red);
	    }
	    else if(whichColor == green) {
	        if(endColor.green < startColor.green)
	            increment = sqrt(float(step)/numSteps) * float(endColor.green-startColor.green);
	        else
	            increment = (float(step)/numSteps) * (float(step)/numSteps) * float(endColor.green-startColor.green);
	    }
	    else if(whichColor == blue) {
	        if(endColor.blue < startColor.blue)
	            increment = sqrt(float(step)/numSteps) * float(endColor.blue-startColor.blue);
	        else 
	            increment = (float(step)/numSteps) * (float(step)/numSteps) * float(endColor.blue-startColor.blue);
	    }
	}

    return increment;
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

///Fade all pixels to black. Or should it be called fade to off
/*void fadeToBlack(void) {
    uint16_t tryCount = 0;
    bool didWeFindAVoxelStillOn;
    //Fade any voxels still lit
    do {
        didWeFindAVoxelStillOn = FALSE;
        for(int idx = 0; idx < PIXEL_CNT; idx++) {
            if(strip.getPixelColor(idx) > 0) {
                transition(black, true);
                didWeFindAVoxelStillOn = TRUE;
            }
        }
        tryCount++;
    }while(tryCount<6 && didWeFindAVoxelStillOn);
}*/

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

/** Allow ifttt.com to trigger this mode based off of a weather recipe. 
 *  Search for Spark Pixels on ifttt.com for an example.
 *  The IFTTT input must be: M:IFTTT WEATHER,C6:xxxxxx,
 *  Don't forget that last comma
 *  Where 0000FF is the hex value for whatever color you want in r-g-b order.
 *  Selecting this mode from the app, will do nothing.
 *  After ifttt.com triggers this mode, the mode will run for 10 minutes, then revert
 *  back to the last running mode.
 *  10 minute time interval is set by the iftttWeatherInterval variable, change this to what you want.
 *  This method will drive all LEDs to 'Breathe' the selected color, just like the RGB
 *  LED on the Photon.
 *  Breathing LED code credit: http://sean.voisen.org/blog/2011/10/breathing-led-with-arduino/
**/
void iftttWeather(uint32_t c) {
    if((millis() - lastCommandReceived) < iftttWeatherInterval) {
        int oldBrightness = brightness;
        float val = (exp(sin(millis()/2000.0*PI)) - 0.36787944)*108.0;  //set "breathing" brightness level
        brightness = (int)val;
        transitionAll(getColorFromInteger(c), LINEAR);    //colorAll(c, true);
        brightness = oldBrightness;
    }
    else {
        currentModeID = lastModeID;
        setNewMode(getModeIndexFromID(currentModeID));
    }
    run = true;
}

/**
 * Source Credit: http://www.instructables.com/id/Led-Cube-8x8x8/
 * Ported by Kevin Carlborg
 * Some standard cube visuals as seen on you tube
 * stackingRope by Kevin Carlborg
 */
void runCubeClassics(uint32_t c) {
    int i, loop, iterations;
    
    Color col = getColorFromInteger(c);
    colorWheel = random(random(2, 256), random(2, 256));
    int effectOrder[13];
	
	for(i=0;i<13;i++) effectOrder[i]=i;
	arrayShuffle(effectOrder, sizeof effectOrder / sizeof effectOrder[0]);
    
	for(i=0;i<13;i++) {
    	transitionAll(black, LINEAR);
		switch(effectOrder[i]) {
			case 0:
			{
			    iterations = 20;
			    if(0 == effect_z_updown(iterations, col)) {
			        transitionAll(black, LINEAR);
			        return;
			    }
				break;
			}
			case 1:
			{
			    for(loop=0;loop<3;loop++) {
			        transitionAll(black, LINEAR);
			        stackingRope(0, col);
			        transitionAll(black, LINEAR);
			        stackingRope(1, col);
			   // collapsingSides(col);
    			}
				break;
			}
			case 2:
			{
				transitionAll(black, LINEAR);
			    iterations = 100;
			    for(int axis=AXIS_X;axis<=AXIS_Z;axis++) {
			        if(0 == effect_wormsqueeze(2, axis, -1, iterations, col)) {
			            transitionAll(black, LINEAR);
			            return;
			        }
			    }
				break;
			}
			case 3: 
			{
			    transitionAll(black, LINEAR);
			    for(loop=0;loop<5;loop++) {
			        for(int axis=AXIS_X;axis<=AXIS_Z;axis++) {
			           if(0 == effect_planboing(axis, col)) {
			               transitionAll(black, LINEAR);
			               return;
			           }
			        }
			    }
				break;
			}
    //effect_telcstairs(0,0xff, col);
	//effect_telcstairs(0,0x00, col);
	//effect_telcstairs(1,0xff, col);
	//effect_telcstairs(1,0xff, col);
			case 4:
			{
			    transitionAll(black, LINEAR);
			    int suspendTime = 1300;
			    for(loop=0;loop<5;loop++) {
			        for(int axis=AXIS_X;axis<=AXIS_Z;axis++) {
			            for(int j=0;j<4;j++) {
			                if(0 == effect_axis_updown_randsuspend(axis, suspendTime,j%2,col)) {
			                    transitionAll(black, LINEAR);
			                    return;
			                }
			            }
			        }
			    }
				break;
			}
			case 5:
			{
				transitionAll(black, LINEAR);
				for(loop=0;loop<5;loop++) {
			    	for(int axis=AXIS_Z;axis>=AXIS_X;axis--) {
			            for(int j=0;j<4;j++) {
			                if(0 == effect_loadbar(axis, col)) {
			                    transitionAll(black, LINEAR);
			                    return;
			                }
			            }
			    	}
			    }
				break;
			}
			case 6:
			{
				transitionAll(black, LINEAR);
				for(loop=0;loop<3;loop++) {
			    	for(int axis=AXIS_X;axis<=AXIS_Z;axis++) {
			    	    for(int mode=1;mode<=2;mode++) {
			                for(int origin=0;origin<=1;origin++) {
			                    if(0 == effect_boxside_randsend_parallel (axis, origin, mode, col)) {
			                        transitionAll(black, LINEAR);
			                        return;
			                    }
			                    delay(1500);
			                }
			    	    }
			    	}
				}
				break;
			}
			case 7:
			{
				transitionAll(black, LINEAR);
				iterations = 120;
			    for(int mode=0;mode<=1;mode++) {
			        for(int drawmode=1;drawmode<=3;drawmode++) {
			            //boingboing(250, 0x01, 0x02, col);
			            if(0 == boingboing(iterations, mode, drawmode, col)) {
			                transitionAll(black, LINEAR);
			                return;
			            }
			        }
			    }
				break;
			}
			case 8:
			{
			    transitionAll(black, LINEAR);
			    iterations = 2000;
			    if(0 == ripples (iterations, col)) {
			        transitionAll(black, LINEAR);
			        return;
			    }
				break;
			}
			case 9:
			{
			    transitionAll(black, LINEAR);
			    iterations = 1200;
			    for(int axis=AXIS_X;axis<=AXIS_Z;axis++) {
			        if(0 == linespin (iterations, axis, col)) {
			            transitionAll(black, LINEAR);
			            return;
			        }
			    }
				break;
			}
			case 10:
			{
			 	transitionAll(black, LINEAR);
			    iterations = 1200;
			    for(int axis=AXIS_X;axis<=AXIS_Z;axis++) {
			        if(0 == sinelines (iterations, axis, col)) {
			            transitionAll(black, LINEAR);
			            return;
			        }
			    }
				break;
			}
			case 11:
			{
				transitionAll(black, LINEAR);
			    iterations = 1500;
			    if(0 == spheremove (iterations,  col)) {
			        transitionAll(black, LINEAR);
			        return;
			    }
				break;
			}
			case 12:
			default:
			{
			    transitionAll(black, LINEAR);
			    iterations = 20;
			    if(0 == fireworks(iterations, 50, col)) {
			        transitionAll(black, LINEAR);
			        return;
			    }
				break;
			}
		}
	}
    run = TRUE;
}

int effect_z_updown (int iterations, Color col) {
	uint8_t positions[64];
	uint8_t destinations[64];
	int i,y,move;
	int speedFactor = 10;
	
    if(switch1) col = getColorFromInteger(Wheel(colorWheel));
    //else if(switch2) col = cheerLightsColor;
        
	for (i=0; i<64; i++) {
		positions[i] = 4;
		destinations[i] = rand()%8;
	}

	for (move=0; move<4; move++)	{
		if(0 == effect_z_updown_move(positions, destinations, AXIS_Z, col)) return 0 ;
		showPixels();
		if(stop) {demo = FALSE; return 0;}
        if(demo) {if(millis() - lastModeSet > twoMinuteInterval) {return 0;}}
		delay(speed*speedFactor);
	}
	
	for (i=0;i<iterations;i++) {
	    for (y=0;y<32;y++) {
			destinations[rand()%64] = rand()%8;
		}
		for (move=0;move<5;move++) {
			if(0 == effect_z_updown_move(positions, destinations, AXIS_Z, col)) return 0;
			showPixels();
		    if(stop) {demo = FALSE; return 0;}
            if(demo) {if(millis() - lastModeSet > twoMinuteInterval) {return 0;}}
			delay(speed*speedFactor);
		}
        if(stop) {demo = FALSE; return 0;}
        if(demo) {if(millis() - lastModeSet > twoMinuteInterval) {return 0;}}
	//	delay(setDdelay);

		
	}
	return 1;
}

int effect_z_updown_move (unsigned char positions[64], unsigned char destinations[64], char axis, Color col) {
	int px;
	
	for (px=0; px<64; px++)	{
		if (positions[px]<destinations[px])	{
			positions[px]++;
		}
		if (positions[px]>destinations[px])	{
			positions[px]--;
		}
	}
	if(stop) {demo = FALSE; return 0;}
    if(demo) {if(millis() - lastModeSet > twoMinuteInterval) {return 0;}}
    if(switch1) col = getColorFromInteger(Wheel(colorWheel+=4));
    //else if(switch2) col = cheerLightsColor;
	if(0 == draw_positions_axis (AXIS_Z, positions,0,col)) return 0;
	
	return 1;
}

int draw_positions_axis (char axis, unsigned char positions[64], int invert, Color col) {
	int x, y, p;
	
	background(black);
	
	for (x=0; x<SIDE; x++)	{
		for (y=0; y<SIDE; y++)	{
			if (invert)	{
				p = (7-positions[(x*8)+y]);
			} else {
				p = positions[(x*8)+y];
			}
			if (axis == AXIS_Y) {
			   // if(switch1) col = getPixelColor(x,y,p);
                //else if(switch2) col = cheerLightsColor;
			    setPixelColor(x,y,p,col); //Z
			}
			if (axis == AXIS_Z)	{
			    //if(switch1) col = getPixelColor(x,p,y);
                //else if(switch2) col = cheerLightsColor;
                setPixelColor(x,p,y,col); //Y
			}
			if (axis == AXIS_X)	{
			    //if(switch1) col = getPixelColor(p,y,x);
                //else if(switch2) col = cheerLightsColor;
                setPixelColor(p,y,x,col);
			}
			//showPixels();
            if(stop) {demo = FALSE; return 0;}
            if(demo) {if(millis() - lastModeSet > twoMinuteInterval) {return 0;}}
		}
	}
	return 1;
}
 

/**
 * @param mode == 0: gradient color rainbow as the rope stacks
 *        mode == 1: Each side is a random color
 */
int stackingRope(int mode, Color col) {
    int x, y, z;
    uint8_t sideColor1, sideColor2, sideColor3, sideColor4;
    
    if(mode == 1) {
        sideColor1 = random(random(2, 256), random(2, 256));
        sideColor2 = sideColor1 + 60;
        sideColor3 = sideColor2 + 60;
        sideColor4 = sideColor3 + 60;
    }
    
    for(y=0;y<SIDE;y++) {
        for(z=0;z<SIDE;z++) {
            if(stop) {demo = FALSE; return 0;}
            if(demo) {if(millis() - lastModeSet > twoMinuteInterval) {return 0;}}
            if(mode == 1) col = getColorFromInteger(Wheel(sideColor1));
            else if(switch1) col = getColorFromInteger(Wheel(colorWheel+=2));
            //else if(switch2) col = cheerLightsColor;
            setPixelColor(0,y,z,col);
            showPixels();
            delay(speed);
        }
        for(x=1;x<SIDE;x++) {
            if(stop) {demo = FALSE; return 0;}
            if(demo) {if(millis() - lastModeSet > twoMinuteInterval) {return 0;}}
            if(mode == 1) col = getColorFromInteger(Wheel(sideColor2));
            else if(switch1) col = getColorFromInteger(Wheel(colorWheel+=2));
            //else if(switch2) col = cheerLightsColor;
            setPixelColor(x,y,SIDE-1,col);
            showPixels();
            delay(speed);
        }
        for(z=SIDE-2;z>=0;z--) {
            if(stop) {demo = FALSE; return 0;}
            if(demo) {if(millis() - lastModeSet > twoMinuteInterval) {return 0;}}
            if(mode == 1) col = getColorFromInteger(Wheel(sideColor3));
            else if(switch1) col = getColorFromInteger(Wheel(colorWheel+=2));
            //else if(switch2) col = cheerLightsColor;
            setPixelColor(SIDE-1,y,z,col);
            showPixels();
            delay(speed);
        }
         for(x=SIDE-2;x>=1;x--) {
            if(stop) {demo = FALSE; return 0;}
            if(demo) {if(millis() - lastModeSet > twoMinuteInterval) {return 0;}}
            if(mode == 1) col = getColorFromInteger(Wheel(sideColor4));
            else if(switch1) col = getColorFromInteger(Wheel(colorWheel+=2));
            //else if(switch2) col = cheerLightsColor;
            setPixelColor(x,y,0,col);
            showPixels();
            delay(speed);
        }
    }
}

/**
 * Assumes the four side planes are alrady drawn
 */
int collapsingSides(Color col) {
    int x, y, z;
    
    Color currentColor = getPixelColor(SIDE/2,0,0);
    for(z=1;z<SIDE-2;z++) {
        for(y=SIDE-2;y>=0;y--) {
            for(x=1;x<SIDE-2;x++) {
                drawLine(Point(x,y-1,z-1),Point(SIDE-1,0,z),black);
                drawLine(Point(x,y,z),Point(SIDE-1,0,z),currentColor);
                if(stop) {demo = FALSE; return 0;}
                if(demo) {if(millis() - lastModeSet > twoMinuteInterval) {return 0;}}
            }
        }
        showPixels();
        delay(speed);
    }
}

int effect_wormsqueeze (int size, int axis, int direction, int iterations, Color col) {
	int x, y, i,j,k, dx, dy;
	int cube_size;
	int origin = 0;
	uint8_t startColor = random(random(2, 256), random(2, 256));
	
	if (direction == -1)
		origin = 7;
	
	cube_size = 8-(size-1);
	
	x = rand()%cube_size;
	y = rand()%cube_size;
	
	for (i=0; i<iterations; i++) {
		dx = ((rand()%3)-1);
		dy = ((rand()%3)-1);
	
		if ((x+dx) > 0 && (x+dx) < cube_size)
			x += dx;
			
		if ((y+dy) > 0 && (y+dy) < cube_size)
			y += dy;
	
		if(0 == shift(axis, direction)) return 0;
		
        if(switch1) col = getColorFromInteger(Wheel(startColor+=2));
        //else if(switch2) col = cheerLightsColor;
		for (j=0; j<size;j++) {
			for (k=0; k<size;k++) {
				if (axis == AXIS_Y) //z
					setPixelColor(x+j,y+k,origin,col);
					
				if (axis == AXIS_Z) //Y
					setPixelColor(x+j,origin,y+k,col);
					
				if (axis == AXIS_X)
					setPixelColor(origin,y+j,x+k,col);
			
				showPixels();
                if(stop) {demo = FALSE; return 0;}
                if(demo) {if(millis() - lastModeSet > twoMinuteInterval) {return 0;}}
			}
		}
		delay(speed*10);
		if(i==iterations-1)
		    transitionAll(black, POLAR);
	}
	return 1;
}

// Shift the entire contents of the cube along an axis
// This is great for effects where you want to draw something
// on one side of the cube and have it flow towards the other
// side. Like rain flowing down the Z axiz.
int shift (char axis, int direction) {
	int i, x ,y;
	int ii, iii;
	Color state;

	for (i = 0; i < SIDE; i++)	{
		if (direction == -1) {
			ii = i;
		} 
		else {
			ii = (7-i);
		}	
	
	    for (x = 0; x < SIDE; x++)	{
			for (y = 0; y < SIDE; y++)	{
				if (direction == -1) {
					iii = ii+1;
				} 
				else {
					iii = ii-1;
				}
				
				if (axis == AXIS_Y)	{ //Z
					state = getPixelColor(x,y,iii);
					setPixelColor(x,y,ii,state);
				}
				
				if (axis == AXIS_Z)	{  //Y
					state = getPixelColor(x,iii,y);
					setPixelColor(x,ii,y,state);
				}
				
				if (axis == AXIS_X)	{
					state = getPixelColor(iii,y,x);
					setPixelColor(ii,y,x,state);
				}
				if(stop) {demo = FALSE; return 0;}
                if(demo) {if(millis() - lastModeSet > twoMinuteInterval) {return 0;}}
			}
		}
	}
	
	if (direction == -1) {
		i = 7;
	} 
	else {
		i = 0;
	}	
	
	for (x = 0; x < SIDE; x++) {
	    for (y = 0; y < SIDE; y++)	{
			if (axis == AXIS_Y) //Z
				setPixelColor(x,y,i,black);
			if (axis == AXIS_Z) //Y
				setPixelColor(x,i,y,black);
			if (axis == AXIS_X)
				setPixelColor(i,y,x,black);
			if(stop) {demo = FALSE; return 0;}
            if(demo) {if(millis() - lastModeSet > twoMinuteInterval) {return 0;}}
		}
	}
	return 1;
}

// Draw a plane on one axis and send it back and forth once.
int effect_planboing (int plane, Color col) {
	int speedFactor = 4;
	int i;
	uint8_t startColor = random(random(2, 256), random(2, 256));
	for (i=0;i<SIDE;i++) {
	    background(black);
	    if(switch1) col = getColorFromInteger(Wheel(startColor+=4));
        //else if(switch2) col = cheerLightsColor;
		setplane(plane, i, col);
        showPixels();
        if(stop) {demo = FALSE; return 0;}
        if(demo) {if(millis() - lastModeSet > twoMinuteInterval) {return 0;}}
		delay(speed*speedFactor);
	}
	
	for (i=7;i>=0;i--) {
		background(black);
		if(switch1) col = getColorFromInteger(Wheel(startColor+=4));
        //else if(switch2) col = cheerLightsColor;
        setplane(plane,i, col);
        showPixels();
        if(stop) {demo = FALSE; return 0;}
        if(demo) {if(millis() - lastModeSet > twoMinuteInterval) {return 0;}}
		delay(speed*speedFactor);
	}
	transitionAll(black, POLAR);
	return 1;
}

void setplane (char axis, unsigned char i, Color col) {
    switch (axis)
    {
        case AXIS_X:
            setplane_x(i, col);
            break;
        case AXIS_Y:
            setplane_y(i, col);
            break;
        case AXIS_Z:
            setplane_z(i, col);
            break;
    }
}

// Sets all voxels along a X/Y plane at a given point
// on axis Z
void setplane_z (int z, Color col) {
	int x,y;
	if (z>=0 && z<SIDE) {
	for (x=0;x<SIDE;x++) 
	    for (y=0;y<SIDE;y++)
	        setPixelColor(x,y,z,col);
	}
}

void setplane_x (int x, Color col) {
	int y,z;
	if (x>=0 && x<SIDE) {
		for (z=0;z<SIDE;z++)
			for (y=0;y<SIDE;y++)
                 setPixelColor(x,y,z,col);
	}
}

void setplane_y (int y, Color col) {
	int x,z;
	if (y>=0 && y<SIDE) {
	    for (x=0;x<SIDE;x++)
		    for (z=0;z<SIDE;z++)
                setPixelColor(x,y,z,col);
	} 
}

int effect_telcstairs (int invert, int val, Color col) {
	int x;
	uint8_t startColor = random(random(2, 256), random(2, 256));
	//background(black);
	if(switch1) col = getColorFromInteger(Wheel(startColor+=2));
    //else if(switch2) col = cheerLightsColor;
	if(invert) {
		for(x = SIDE*2; x >= 0; x--) {
			x = effect_telcstairs_do(x,val,col);
			if(-2 == x) return 0;
		}
	} 
	else {
		for(x = 0; x < SIDE*2; x++) {
			x = effect_telcstairs_do(x,val,col);
			if(-2 == x) return 0;
		}
	}
	return 1;
}

int effect_telcstairs_do(int x, int val, Color col) {
	int speedFactor = 2;
	int y,z;
	
	for(y = 0, z = x; y <= z; y++, x--)	{
		if(x < SIDE && y < SIDE) {
			setPixelColor(x,z,y,col);
		}
	}
	showPixels();
	if(stop) {demo = FALSE; return -2;}
    if(demo) {if(millis() - lastModeSet > twoMinuteInterval) {return -2;}}
	delay(speed*speedFactor);
	return z;
}

int effect_axis_updown_randsuspend (char axis, int sleep, int invert, Color col) {
	int speedFactor = 2;
	int i,px;
	uint8_t startColor = random(random(2, 256), random(2, 256));
	uint8_t positions[PIXELS_PER_PANEL];
	uint8_t destinations[PIXELS_PER_PANEL];
	
    // Set 64 random positions
	for (i=0; i<PIXELS_PER_PANEL; i++) {
		positions[i] = 0; // Set all starting positions to 0
		destinations[i] = rand()%SIDE;
	}

    // Loop 8 times to allow destination 7 to reach all the way
	for (i=0; i<SIDE; i++)	{
        // For every iteration, move all position one step closer to their destination
		for (px=0; px<PIXELS_PER_PANEL; px++)	{
			if (positions[px]<destinations[px])	{
				positions[px]++;
			}
		}
		if(switch1) col = getColorFromInteger(Wheel(startColor+=8));
        //else if(switch2) col = cheerLightsColor;
        // Draw the positions and take a nap
		if(0 == draw_positions_axis (axis, positions,invert,col)) {
		    transitionAll(black, POLAR);
		    return 0;
		}
		showPixels();
	    if(stop) {
		    transitionAll(black, POLAR);
	        demo = FALSE; 
	        return 0;
	    }
        if(demo) {
            if(millis() - lastModeSet > twoMinuteInterval) {
    		    transitionAll(black, POLAR);
                return 0;
            }
        }
        delay(speed*speedFactor);
	}
	
    // Set all destinations to 7 (opposite from the side they started out)
	for (i=0; i<PIXELS_PER_PANEL; i++) {
		destinations[i] = 7;
	}
	
    // Suspend the positions in mid-air for a while
	delay(sleep);
	
	if(switch1) col = getColorFromInteger(Wheel(startColor+=8));
    //else if(switch2) col = cheerLightsColor;
    // Then do the same thing one more time
	for (i=0; i<SIDE; i++)	{
		for (px=0; px<PIXELS_PER_PANEL; px++)	{
			if (positions[px]<destinations[px])	{
				positions[px]++;
			}
			if (positions[px]>destinations[px])	{
				positions[px]--;
			}
		}
		if(0 == draw_positions_axis (axis, positions,invert,col)) {
		    transitionAll(black, POLAR);
		    return 0;
		}
		showPixels();
	    if(stop) {
		    transitionAll(black, POLAR);
	        demo = FALSE; 
	        return 0;
	    }
        if(demo) {
            if(millis() - lastModeSet > twoMinuteInterval) {
    		    transitionAll(black, POLAR);
                return 0;
            }
        }
	    delay(speed*speedFactor);
	}
    //transitionAll(black, POLAR);
	return 1;
}

// Light all leds layer by layer,
// then unset layer by layer
int effect_loadbar(int axis, Color col) {
    int i;
    int speedFactor = 3;
    uint8_t startColor = random(random(2, 256), random(2, 256));
    
	//background(black);

	for (i=0;i<SIDE;i++) {
	    if(switch1) col = getColorFromInteger(Wheel(startColor+=4));
        //else if(switch2) col = cheerLightsColor;
    	setplane(axis,i,col);
        showPixels();
        if(stop) {demo = FALSE; return 0;}
        if(demo) {if(millis() - lastModeSet > twoMinuteInterval) {return 0;}}
    	delay(speed*speedFactor);
	}
	
	delay(speed*3);
	
	for (i=0;i<SIDE;i++) {
		if(i==SIDE-1)
		    transitionAll(black, POLAR);
		else
    	    setplane(axis,i,black);
        showPixels();
        if(stop) {demo = FALSE; return 0;}
        if(demo) {if(millis() - lastModeSet > twoMinuteInterval) {return 0;}}
    	delay(speed*speedFactor);
	}
    //transitionAll(black, POLAR);
	return 1;
}

int effect_boxside_randsend_parallel (char axis, int origin, int mode, Color col) {
	int speedFactor = 2;
	int i, done;
	int notdone = 1;
	int notdone2 = 1;
	int sent = 0;
	uint8_t startColor = random(random(2, 256), random(2, 256));
	uint8_t cubepos[PIXELS_PER_PANEL];
	uint8_t pos[PIXELS_PER_PANEL];
	
	for (i=0;i<PIXELS_PER_PANEL;i++) {
	    if(switch1) col = getColorFromInteger(Wheel(startColor+=4));
        //else if(switch2) col = cheerLightsColor;
		pos[i] = 0;
	}
	
	while (notdone)	{
		if (mode == 1) {
			notdone2 = 1;
			while (notdone2 && sent<PIXELS_PER_PANEL)	{
				i = rand()%PIXELS_PER_PANEL;
				if (pos[i] == 0) {
					sent++;
					pos[i] += 1;
					notdone2 = 0;
				}
			}
		} 
		else if (mode == 2)	{
			if (sent<PIXELS_PER_PANEL) {
				pos[sent] += 1;
				sent++;
			}
		}
		
		done = 0;
		for (i=0;i<PIXELS_PER_PANEL;i++) {
			if (pos[i] > 0 && pos[i] <7) {
				pos[i] += 1;
			}
				
			if (pos[i] == 7)
				done++;
		}
		
		if (done == PIXELS_PER_PANEL)
			notdone = 0;
		
		for (i=0;i<PIXELS_PER_PANEL;i++) {
			if (origin == 0) {
				cubepos[i] = pos[i];
			} 
			else {
				cubepos[i] = (7-pos[i]);
			}
		}
		//delay(speed);
		if(0 == draw_positions_axis(axis,cubepos,0,col)) {
		    transitionAll(black, POLAR);
		    return 0;
		}
		showPixels();
	    if(stop) {
		    transitionAll(black, POLAR);
	        demo = FALSE; 
	        return 0;
	    }
        if(demo) {
            if(millis() - lastModeSet > twoMinuteInterval) {
    		    transitionAll(black, POLAR);
                return 0;
            }
        }
		delay(speed*speedFactor);
	}
}

// Big ugly function :p but it looks pretty
int boingboing(uint16_t iterations, unsigned char mode, unsigned char drawmode, Color col) {
    int speedFactor = 4;
	int x, y, z;		// Current coordinates for the point
	int dx, dy, dz;	// Direction of movement
	int lol, i;		// lol?
	uint8_t startColor = random(random(2, 256), random(2, 256));
	uint8_t crash_x, crash_y, crash_z;

	background(black);		// Blank the cube
	
	y = rand()%SIDE;
	x = rand()%SIDE;
	z = rand()%SIDE;

	// Coordinate array for the snake.
	int snake[8][3];
	for (i=0;i<SIDE;i++) {
		snake[i][0] = x;
		snake[i][1] = y;
		snake[i][2] = z;
	}
	
	dx = 1;
	dy = 1;
	dz = 1;
	
	while(iterations) {
		crash_x = 0;
		crash_y = 0;
		crash_z = 0;
	
		// Let's mix things up a little:
		if (rand()%3 == 0) {
			// Pick a random axis, and set the speed to a random number.
			lol = rand()%3;
			if (lol == 0) dx = rand()%3 - 1;
			if (lol == 1) dy = rand()%3 - 1;
			if (lol == 2) dz = rand()%3 - 1;
		}

	    // The point has reached 0 on the x-axis and is trying to go to -1
        // aka a crash
		if (dx == -1 && x == 0)	{
			crash_x = 0x01;
			if (rand()%3 == 1) dx = 1;
			else dx = 0;
		}
		
        // y axis 0 crash
		if (dy == -1 && y == 0) {
			crash_y = 0x01;
			if (rand()%3 == 1) dy = 1;
			else dy = 0;
		}
		
        // z axis 0 crash
		if (dz == -1 && z == 0) {
			crash_z = 0x01;
			if (rand()%3 == 1) dz = 1;
			else dz = 0;
		}
	    
        // x axis 7 crash
		if (dx == 1 && x == 7) {
			crash_x = 0x01;
			if (rand()%3 == 1) dx = -1;
			else dx = 0;
		}
		
        // y axis 7 crash
		if (dy == 1 && y == 7) {
			crash_y = 0x01;
			if (rand()%3 == 1) dy = -1;
			else dy = 0;
		}
		
        // z azis 7 crash
		if (dz == 1 && z == 7) {
			crash_z = 0x01;
			if (rand()%3 == 1) dz = -1;
			else dz = 0;
		}
		
		// mode bit 0 sets crash action enable
		if (mode | 0x01) {
			if (crash_x) {
				if (dy == 0) {
					if (y == 7) dy = -1;
					else if (y == 0) dy = +1;
					else {
						if (rand()%2 == 0) dy = -1;
						else dy = 1;
					}
				}
				if (dz == 0) {
					if (z == 7)	dz = -1;
					else if (z == 0) dz = 1;
					else {
						if (rand()%2 == 0) dz = -1;
				        else dz = 1;
					}	
				}
			}
			
			if (crash_y) {
				if (dx == 0) {
					if (x == 7)	dx = -1;
					else if (x == 0) dx = 1;
					else {
						if (rand()%2 == 0) dx = -1;
						else dx = 1;
					}
				}
				if (dz == 0) {
					if (z == 3)	dz = -1;
					else if (z == 0) dz = 1;
					else {
						if (rand()%2 == 0) dz = -1;
						else dz = 1;
					}	
				}
			}
			
			if (crash_z) {
				if (dy == 0) {
					if (y == 7)	dy = -1;
					else if (y == 0) dy = 1;
					else {
						if (rand()%2 == 0) dy = -1;
						else dy = 1;
					}	
				}
				if (dx == 0) {
					if (x == 7)	dx = -1;
					else if (x == 0) dx = 1;
					else {
						if (rand()%2 == 0) dx = -1;
						else dx = 1;
					}	
				}
			}
		}
		
		// mode bit 1 sets corner avoid enable
		if (mode | 0x02) {
			if (	// We are in one of 8 corner positions
				(x == 0 && y == 0 && z == 0) ||
				(x == 0 && y == 0 && z == 7) ||
				(x == 0 && y == 7 && z == 0) ||
				(x == 0 && y == 7 && z == 7) ||
				(x == 7 && y == 0 && z == 0) ||
				(x == 7 && y == 0 && z == 7) ||
				(x == 7 && y == 7 && z == 0) ||
				(x == 7 && y == 7 && z == 7)
			) {
				// At this point, the voxel would bounce
				// back and forth between this corner,
				// and the exact opposite corner
				// We don't want that!
			
				// So we alter the trajectory a bit,
				// to avoid corner stickyness
				lol = rand()%3;
				if (lol == 0) dx = 0;
				if (lol == 1) dy = 0;
				if (lol == 2) dz = 0;
			}
		}

        // one last sanity check
        if (x == 0 && dx == -1) dx = 1;
	    if (y == 0 && dy == -1) dy = 1;
        if (z == 0 && dz == -1) dz = 1;
        if (x == 7 && dx == 1)  dx = -1;
        if (y == 7 && dy == 1)  dy = -1;
        if (z == 7 && dz == 1)  dz = -1;
	
		// Finally, move the voxel.
		x = x + dx;
		y = y + dy;
		z = z + dz;
		
		if(switch1) col = getColorFromInteger(Wheel(startColor+=2));
        //else if(switch2) col = cheerLightsColor;
		if (drawmode == 0x01) {// show one voxel at time
			setPixelColor(x,y,z,col);
			showPixels();
	        if(stop) {demo = FALSE; return 0;}
            if(demo) {if(millis() - lastModeSet > twoMinuteInterval) {return 0;}}
			delay(speed*speedFactor);
			setPixelColor(x,y,z,black);	
		} 
		else if (drawmode == 0x02) {// flip the voxel in question
			flipVoxel(x,y,z,col);
			showPixels();
	        if(stop) {demo = FALSE; return 0;}
            if(demo) {if(millis() - lastModeSet > twoMinuteInterval) {return 0;}}
			delay(speed*speedFactor);
		} 
		if (drawmode == 0x03) {// draw a snake
			for (i=SIDE-1;i>=0;i--) {
				snake[i][0] = snake[i-1][0];
				snake[i][1] = snake[i-1][1];
				snake[i][2] = snake[i-1][2];
			}
			snake[0][0] = x;
			snake[0][1] = y;
			snake[0][2] = z;
				
			for (i=0;i<SIDE;i++) {
				setPixelColor(snake[i][0],snake[i][1],snake[i][2],col);
				showPixels();
	            if(stop) {demo = FALSE; return 0;}
                if(demo) {if(millis() - lastModeSet > twoMinuteInterval) {return 0;}}
			}
			delay(speed*speedFactor);
			for (i=0;i<SIDE;i++) {
				setPixelColor(snake[i][0],snake[i][1],snake[i][2],black);
				showPixels();
			}
		}
		iterations--;
	}
	transitionAll(black, POLAR);
	return 1;
}

void flipVoxel(int x, int y, int z, Color newCol) {
    Color currentCol = getPixelColor(x,y,z);
    if(currentCol.red==0 && currentCol.green==0 && currentCol.blue==0)
        setPixelColor(x,y,z,newCol);
    else
        setPixelColor(x,y,z,black);
}

// Display a sine wave running out from the center of the cube.
int ripples (int iterations, Color col) {
	float origin_x, origin_y, distance, height, ripple_interval;
	int x,z,i;
    uint8_t startColor = random(random(2, 256), random(2, 256));
    
    //background(black);

	for (i=0;i<iterations;i++) {
	    if(switch1) col = getColorFromInteger(Wheel(startColor+=2));
        //else if(switch2) col = cheerLightsColor;
		for (x=0;x<SIDE;x++) {
			for (z=0;z<SIDE;z++) {
				distance = distance2d(3.5,3.5,x,z)/9.899495*8;
				//distance = distance2d(3.5,3.5,x,y);
				ripple_interval =1.3;
				height = 4+sin(distance/ripple_interval+(float) i/50)*4;

				setPixelColor(x,(int) height, z, col);	
			}
		}
		showPixels();
	    if(stop) {demo = FALSE; return 0;}
        if(demo) {if(millis() - lastModeSet > twoMinuteInterval) {return 0;}}
		//delay(speed);
		if(i==iterations-1)
		    transitionAll(black, POLAR);
		else
		    background(black);
	}
}

/*inline float Hill(float x) {
  const float a0 = 1.0f;
  const float a2 = 2.0f / PI - 12.0f / (PI * PI);
  const float a3 = 16.0f / (PI * PI * PI) - 4.0f / (PI * PI);
  const float xx = x * x;
  const float xxx = xx * x;
 
  return a0 + a2 * xx + a3 * xxx;
}

float sine(float x) {
  // wrap x within [0, (2.0f * PI))
  const float a = x * (1.0f / (2.0f * PI));
  x -= static_cast<int>(a) * (2.0f * PI);
  if (x < 0.0f)
    x += (2.0f * PI);
 
  // 4 pieces of hills
  if (x < (0.5f * PI))
    return Hill((0.5f * PI) - x);
  else if (x < PI)
    return Hill(x - (0.5f * PI));
  else if (x < 3.0f * (0.5f * PI))
    return -Hill(3.0f * (0.5f * PI) - x);
  else
    return -Hill(x - 3.0f * (0.5f * PI));
}

float cosine(float x) {
    return sine(x + (0.5f * PI));
}

float squareRoot(float x) {
  unsigned int i = *(unsigned int*) &x;

  // adjust bias
  i  += 127 << 23;
  // approximation of square root
  i >>= 1;

  return *(float*) &i;
}*/

float distance2d (float x1, float y1, float x2, float y2) {	
	float dist;
	dist = sqrt((x1-x2)*(x1-x2) + (y1-y2)*(y1-y2));

	return dist;
}

int linespin (int iterations, char axis, Color col) {
	float top_x, top_y, top_z, bot_x, bot_y, bot_z, sin_base;
	float center_x, center_y;
	int i, z;
    uint8_t startColor = random(random(2, 256), random(2, 256));
    
    //transitionAll(black, POLAR);
    
	center_x = 4;
	center_y = 4;

	for (i=0;i<iterations;i++) {

		//printf("Sin base %f \n",sin_base);
        if(switch1) col = getColorFromInteger(Wheel(startColor+=2));
        //else if(switch2) col = cheerLightsColor;
        
		for (z = 0; z < SIDE; z++)	{

    		sin_base = (float)i/50 + (float)z/(10+(7*sin((float)i/200)));
    
    		top_x = center_x + sin(sin_base)*5;
    		top_y = center_x + cos(sin_base)*5;
    		//top_z = center_x + cos(sin_base/100)*2.5;
    
    		bot_x = center_x + sin(sin_base+3.14)*10;
    		bot_y = center_x + cos(sin_base+3.14)*10;
    		//bot_z = 7-top_z;
    		
    		bot_z = z;
    		top_z = z;
    
    		//setPixelColor((int) top_x, (int) top_y, 7);
    		//setPixelColor((int) bot_x, (int) bot_y, 0);
    		//sprintf(debug, "P1: %i %i %i P2: %i %i %i \n", (int) top_x, (int) top_y, 7, (int) bot_x, (int) bot_y, 0);
    
    		switch(axis){
    		    case AXIS_X:
                    drawLine(Point((int)top_z, (int)top_x, (int)top_y), Point((int)bot_z, (int)bot_x, (int)bot_y), col); 
                    break;
                case AXIS_Y:
                    drawLine(Point((int)top_x, (int)top_z, (int)top_y), Point((int)bot_x, (int)bot_z, (int)bot_y), col);
                    break;
                case AXIS_Z:
                    drawLine(Point((int)top_x, (int)top_y, (int)top_z), Point((int)bot_x, (int)bot_y, (int)bot_z), col);
                    break;
            }
			
		}
        showPixels();
	    if(stop) {demo = FALSE; return 0;}
        if(demo) {if(millis() - lastModeSet > twoMinuteInterval) {return 0;}}
		delay(speed);
		if(i==iterations-1)
		    transitionAll(black, POLAR);
		else
		    background(black);
	}
}

int sinelines (int iterations, char axis, Color col) {
	float left, right, sine_base, x_dividor,ripple_height;
	int i,x;
    uint8_t startColor = random(random(2, 256), random(2, 256));
    
    //transitionAll(black, POLAR);

	for (i=0; i<iterations; i++) {
	    if(switch1) col = getColorFromInteger(Wheel(startColor+=2));
        //else if(switch2) col = cheerLightsColor;
		for (x=0; x<SIDE ;x++)	{
			x_dividor = 2 + sin((float)i/100)+1;
			ripple_height = 3 + (sin((float)i/200)+1)*6;

			sine_base = (float) i/40 + (float) x/x_dividor;

			left = 4 + sin(sine_base)*ripple_height;
			right = 4 + cos(sine_base)*ripple_height;
			right = 7-left;

			//printf("%i %i \n", (int) left, (int) right);

			//line_3d(0-3, x, (int) left, 7+3, x, (int) right, col);

			switch(axis){
    		    case AXIS_X:
                    drawLine(Point(x, 0-3, (int)left), Point(x, 7+3, (int)right), col);
                    break;
                case AXIS_Y:
                    drawLine(Point(0-3, x, (int)left), Point(7+3, x, (int)right), col);
                    break;
                case AXIS_Z:
                    drawLine(Point(0-3, (int)left, x), Point(7+3, (int)right, x), col);
                    break;
            }
			
		}
	
	    showPixels();
	    if(stop) {demo = FALSE; return 0;}
        if(demo) {if(millis() - lastModeSet > twoMinuteInterval) {return 0;}}
		delay(speed);
		if(i==iterations-1)
		    transitionAll(black, POLAR);
		else
		    background(black);
	}
}

int spheremove (int iterations, Color col) {
	float origin_x, origin_y, origin_z, distance, diameter;
	uint8_t startColor = random(random(2, 256), random(2, 256));

    //background(black);

	origin_x = 0;
	origin_y = 3.5;
	origin_z = 3.5;

	diameter = 3;

	int x, y, z, i;

	for (i=0; i<iterations; i++) {
	    if(switch1) col = getColorFromInteger(Wheel(startColor+=2));
        //else if(switch2) col = cheerLightsColor;
		origin_x = 3.5+sin((float)i/50)*2.5;
		origin_y = 3.5+cos((float)i/50)*2.5;
		origin_z = 3.5+cos((float)i/30)*2;

		diameter = 2+sin((float)i/150);

		for (x=0; x<SIDE; x++)	{
			for (y=0; y<SIDE; y++)	{
				for (z=0; z<SIDE; z++)	{
					distance = distance3d(x,y,z, origin_x, origin_y, origin_z);
					//printf("Distance: %f \n", distance);

					if (distance>diameter && distance<diameter+1) {
						setPixelColor(x,z,y,col);
					}
				}
			}
		}
        showPixels();
	    if(stop) {demo = FALSE; return 0;}
        if(demo) {if(millis() - lastModeSet > twoMinuteInterval) {return 0;}}
		delay(speed);
		if(i==iterations-1)
		    transitionAll(black, POLAR);
		else
		    background(black);
	}
}

float distance3d (float x1, float y1, float z1, float x2, float y2, float z2) {	
	float dist;
	dist = sqrt((x1-x2)*(x1-x2) + (y1-y2)*(y1-y2) + (z1-z2)*(z1-z2));

	return dist;
}

int fireworks (int iterations, int n, Color col) {
    int i,f,e;
    int rand_y, rand_x, rand_z;
    uint8_t startColor = random(random(2, 256), random(2, 256));
    float origin_x = 3;
	float origin_y = 3;
	float origin_z = 3;
	float slowrate, gravity;
	float particles[n][6];  // Particles and their position, x,y,z and their movement, dx, dy, dz
    
	//background(black);

	for (i=0; i<iterations; i++) {

		origin_x = (rand()%4) + 2;
		origin_y = (rand()%4) + 2;
		origin_z = (rand()%2) + 5;

		// shoot a particle up in the air
		if(switch1) col = getColorFromInteger(Wheel(startColor+=2));
        //else if(switch2) col = cheerLightsColor;
        
		for (e=0;e<origin_z;e++) {
			setPixelColor(origin_x,e, origin_y,col);
			showPixels();
	        if(stop) {demo = FALSE; return 0;}
             if(demo) {if(millis() - lastModeSet > twoMinuteInterval) {return 0;}}
			delay(speed*e);
			background(black);
		}

		// Fill particle array
		for (f=0; f<n; f++)	{
			// Position
			particles[f][0] = origin_x;
			particles[f][1] = origin_y;
			particles[f][2] = origin_z;
			
			rand_x = rand()%200;
			rand_y = rand()%200;
			rand_z = rand()%200;

			// Movement
			particles[f][3] = 1-(float)rand_x/100; // dx
			particles[f][4] = 1-(float)rand_y/100; // dy
			particles[f][5] = 1-(float)rand_z/100; // dz
		}

		// explode
		for (e=0; e<25; e++) {
		    if(switch1) col = getColorFromInteger(Wheel(startColor+=2));
            //else if(switch2) col = cheerLightsColor;
			slowrate = 1+tan((e+0.1)/20)*10;
			
			gravity = tan((e+0.1)/20)/2;

			for (f=0; f<n; f++) {
				particles[f][0] += particles[f][3]/slowrate;
				particles[f][1] += particles[f][4]/slowrate;
				particles[f][2] += particles[f][5]/slowrate;
				particles[f][2] -= gravity;

				setPixelColor(particles[f][0],particles[f][2],particles[f][1],col);
			}
            showPixels();
	        if(stop) {demo = FALSE; return 0;}
            if(demo) {if(millis() - lastModeSet > twoMinuteInterval) {return 0;}}
			delay(speed*5);
			background(black);
		}
		if(i==iterations-1)
		    transitionAll(black, POLAR);
		else
		    background(black);
	}
}

/**
 * Pick a random color, then randomly fill the whole strip with that color
 * But make sure the next color has some contrast from the current color
 * @randomPixelFill(): helper function to do the dirty work
 * @switch1 = Random Color Fill: Used in randomPixelFill() to fill the strip with random colors
 * @switch2 = Slow Transition: 
 */
void digi(uint32_t color1) {
    uint16_t i; 
    uint32_t nextCol;
    
    //Lets just assume all of the colors in the strip are the same color
    //This will be true on the second lap and if we aren't doing Random Fill
    uint32_t curCol = nextCol = strip.getPixelColor(0);  
    if((!switch1) && (curCol > 0))
        transitionAll(black, POLAR);
        
    //Next color should have some contrast from the current color
    if(switch1) {
        while(abs(nextCol - curCol) <= 45 ) {
           nextCol = Wheel(random(random(2, 256), random(2, 256)));
        }
    }
    else
        nextCol = color1;
    
    randomPixelFill(nextCol);
    run = TRUE;
}

/**
 * digi() helper function
 * Randomly fills the whole strip with a selected color or a random color
 * @param c: Next Color to populate
 * @switch1 = Random Color Fill: Ignore the passed color and choose random colors for each pixel
 */
int randomPixelFill(uint32_t c) {
    uint16_t i; 
    uint32_t pulseRate;
    int pixelFillOrder[strip.numPixels()];
    
    for(i=0; i<strip.numPixels(); i++) {
        pixelFillOrder[i]=i;
    }
    
    arrayShuffle(pixelFillOrder, sizeof pixelFillOrder / sizeof pixelFillOrder[0]);
    if(switch1) {c = Wheel(random(random(2, 256), random(2, 256)));}
    
    for(i=0; i<strip.numPixels(); i++) {
        if(stop) {demo = FALSE; return 0;}
        if(demo) {if(millis() - lastModeSet > twoMinuteInterval) {return 0;}}
        //if(switch1) {c = Wheel(random(random(2, 256), random(2, 256)));}
        if(switch2) {
            transitionOne(getColorFromInteger(c),pixelFillOrder[i],POLAR);
        }
        else {
            strip.setPixelColor(pixelFillOrder[i], c);
            showPixels();
            delay(speed);
        }
    }
    return 1;
}

int CubePainter(String command) {
	if(currentModeID != CUBE_PAINTER) {return 0;}
	
	int beginIdx = 0;
	//int returnValue = -1;
	int idx = command.indexOf(',');
    int voxelIdx;
    Color voxelCol;
    run = TRUE;
    
    // Trim extra spaces
    command.trim();
    // Convert it to upper-case for easier matching
    command.toUpperCase();
    /** DEBUG **/
    //sprintf(debug,"%s", command.c_str());
    
    while(idx != -1) {
        if(command.charAt(beginIdx) == 'I') {
            voxelIdx = command.substring(beginIdx+1, idx).toInt();
            /** DEBUG **/
            //itoa(voxelIdx, debug, 10);
        }
        else if(command.charAt(beginIdx) == '#') {
    		voxelCol.red=hexToInt(command.charAt(beginIdx+1))*16+hexToInt(command.charAt(beginIdx+2));
    		voxelCol.green=hexToInt(command.charAt(beginIdx+3))*16+hexToInt(command.charAt(beginIdx+4));
    		voxelCol.blue=hexToInt(command.charAt(beginIdx+5))*16+hexToInt(command.charAt(beginIdx+6));
            /** DEBUG **/
            //itoa(strip.Color(voxelCol.red, voxelCol.green, voxelCol.blue), debug, 10);
        }
        else if(command.charAt(beginIdx) == 'C') {
            int startIdx, endIdx;
            startIdx = command.substring(beginIdx+1, command.indexOf(':', beginIdx)).toInt();
            endIdx = command.substring(command.indexOf(':', beginIdx)+1, idx).toInt();
            for(int i=startIdx; i<=endIdx; i++)
                strip.setPixelColor(i, 0);
          	return 0;
        }
		beginIdx = idx + 1;
		idx = command.indexOf(',', beginIdx);    
    }
    strip.setPixelColor(voxelIdx, strip.Color(voxelCol.red, voxelCol.green, voxelCol.blue));
    return 0;
}

// Set all pixels in the strip to a solid color
/* THIS FUNCTION HAS BEEN DEPRECATED - Use 'void transition(Color bgcolor, bool loop)'
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
    }
    return 1;
}*/

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

void cheerlights(void) {
    int red, green, blue;
    bool headers;
    char lastChar;
    run = TRUE;
    
    if((millis()-pollTime)<=POLLING_INTERVAL) {
        if(stop) {demo = FALSE; client.stop(); return;}
        if(demo) {if(millis() - lastModeSet > twoMinuteInterval) {client.stop(); return;}}
	    if(!Particle.connected()) {
	        Particle.connect();
	        connected = waitFor(Particle.connected, 1500);
	        if(connected) {
    		    client.stop();
    		    client.connect(hostname, 80);
    		    connected = waitFor(client.connected, 1500);
	        }
	    }
        else if(!client.connected()) {
		    client.stop();
		    client.connect(hostname, 80);
		    connected = waitFor(client.connected, 1500);
        }
	    else {
            //In order to allow changing the brightness at any moment
            strip.setBrightness(brightness);
            strip.show();
            //process Spark events
            Particle.process();
            delay(100);
	    }
    }
    else {
        connected = client.connected();
        if(connected) {
            pollTime=millis();
            client.print("GET ");
            client.print(path);
            client.println(" HTTP/1.0");
            client.print("Host: ");
            client.println(hostname);
            client.println("Content-Length: 0");
            client.println();
          	// DEBUG
            sprintf(debug, "connected");
        }
        else {
          	// DEBUG
            sprintf(debug, "not connected");
          
            if(stop) {demo = FALSE; client.stop(); return;}
            if(demo) {if(millis() - lastModeSet > twoMinuteInterval) {client.stop(); return;}}
            client.stop();
		    response = "";
		    if(!Particle.connected()) {
		        Particle.connect();
		        connected = waitFor(Particle.connected, 1500);
		        if(connected) {
    		        client.stop();
        		    client.connect(hostname, 80);
        		    connected = waitFor(client.connected, 1500);
		        }
		    }
		    else {
		        client.stop();
    		    client.connect(hostname, 80);
    		    connected = waitFor(client.connected, 1500);
		    }
        }
    
        requestTime=millis();
        while((client.available()==0)&&((millis()-requestTime)<RESPONSE_TIMEOUT)) {
            if(stop) {demo = FALSE; client.stop(); return;}
            if(demo) {if(millis() - lastModeSet > twoMinuteInterval) {client.stop(); return;}}
            Particle.process();    //process Spark events
        };
        
        headers=TRUE;
        lastChar='\n';
        response="";
    	while(client.available()>0) {
            if(stop) {demo = FALSE; client.stop(); return;}
            if(demo) {if(millis() - lastModeSet > twoMinuteInterval) {client.stop(); return;}}
    		char thisChar=client.read();
    		if(!headers)
    		    response.concat(String(thisChar));
    		else {
    			if((thisChar=='\r')&&(lastChar=='\n')) {
        			headers=FALSE;
        			client.read();  //kill that last \n
    			}
    			lastChar=thisChar;  
    		}
          	// DEBUG
            itoa(client.available(), debug, 10);
    	}

        //if there's a valid hex color string from Cheerlights, update the color
        if(response.length()==7) {
            //convert the hex values from the response.body string into byte values
    		red=hexToInt(response.charAt(1))*16+hexToInt(response.charAt(2));
    		green=hexToInt(response.charAt(3))*16+hexToInt(response.charAt(4));
    		blue=hexToInt(response.charAt(5))*16+hexToInt(response.charAt(6));
        	Color col=Color(red, green, blue);

        	//actually update the color on the cube, with a cute animation
    	    if(col != lastCol) {
            	lastCol = col;
	            int c = strip.Color(col.red, col.green, col.blue);
        	    int which = random(0, 6);
                if(stop) {demo = FALSE; client.stop(); return;}
                if(demo) {if(millis() - lastModeSet > twoMinuteInterval) {client.stop(); return;}}
        	    switch(which) {
        	        case 0:
        	            transitionAll(col, POLAR);
        	            break;
        	        case 1:
        	            colorZone(c, c, c, c, run);
        	            break;
        	        case 2:
        	            fillX(col);
        	            break;
        	        case 3:
        	            fillY(col);
        	            break;
        	        case 4:
        	            fillZ(col);
        	            break;
        	        case 5:
        	        default:
        	            switch1 = FALSE;
        	            switch2 = random(2);
        	            randomPixelFill(c);
        	            break;
        	    }
    	    }
          	// DEBUG
            sprintf(debug, response);
        }
        else {
          	// DEBUG
            sprintf(debug, "no reply from host");
          	
            if(stop) {demo = FALSE; client.stop(); return;}
            if(demo) {if(millis() - lastModeSet > twoMinuteInterval) {client.stop(); return;}}
            client.stop();
		    response = "";
		    if(!Particle.connected()) {
		        Particle.connect();
		        connected = waitFor(Particle.connected, 1500);
		        if(connected) {
    		        client.stop();
        		    client.connect(hostname, 80);
        		    connected = waitFor(client.connected, 1500);
		        }
		    }
		    else {
		        client.stop();
    		    client.connect(hostname, 80);
    		    connected = waitFor(client.connected, 1500);
		    }
        }
    }
}

/*void cheerlights(void) {
    if(cheerLightsColor != lastCol) {
        lastCol = cheerLightsColor;
    	int c = strip.Color(cheerLightsColor.red, cheerLightsColor.green, cheerLightsColor.blue);
        int which = random(0, 6);
        if(stop) {demo = FALSE; client.stop(); return;}
        if(demo) {if(millis() - lastModeSet > twoMinuteInterval) {client.stop(); return;}}
        switch(which) {
            case 0:
                transitionAll(cheerLightsColor, POLAR);
            	break;
            case 1:
                colorZone(c, c, c, c, run);
                break;
            case 2:
                fillX(cheerLightsColor);
                break;
            case 3:
                fillY(cheerLightsColor);
                break;
            case 4:
                fillZ(cheerLightsColor);
                break;
            case 5:
            default:
                randomPixelFill(c);
                break;
        }
    }
    sprintf(debug,"lastColor=%i, cheerlightsColor=%i",strip.Color(lastCol.red, lastCol.green, lastCol.blue),strip.Color(cheerLightsColor.red, cheerLightsColor.green, cheerLightsColor.blue));
}*/

/*os_thread_return_t LoopgetCheerLightsColor(void* param){
    bool shouldIGetCheerLights;
    resetVariables(CHEERLIGHTS);   //initCheerLights();
    for(;;) {
        shouldIGetCheerLights = ((currentModeID == getModeIndexFromID(CUBE_CLASSICS)) && switch2);
        if(shouldIGetCheerLights) {getCheerlights();}
    }
}*/

/*void initCheerLights(void) {
    hostname = "api.thingspeak.com";
    path = "/channels/1417/field/2/last.txt";
	response = "";
	pollTime = millis() + POLLING_INTERVAL;
	lastCol = black;
    client.stop();
	connected = client.connect(hostname, 80);
}*/

/*void getCheerlights(void) {
    int red, green, blue;
    bool headers;
    char lastChar;
    run = TRUE;
    
    if((millis()-pollTime)<=POLLING_INTERVAL) {
	    if(!Particle.connected) {
	        Particle.connect();
	        varsRegistered = waitFor(Particle.connected, 1000);
	    }
	    else {
            //In order to allow changing the brightness at any moment
            strip.setBrightness(brightness);
            strip.show();
            //process Spark events
            Particle.process();
            delay(100);
	    }
    }
    else {
        if(connected) {
            pollTime=millis();
            client.print("GET ");
            client.print(path);
            client.println(" HTTP/1.0");
            client.print("Host: ");
            client.println(hostname);
            client.println("Content-Length: 0");
            client.println();
            sprintf(debug, "connected");
        }
        else {
            sprintf(debug, "not connected");
          
            client.stop();
		    response = "";
		    if(!Particle.connected) {
		        Particle.connect();
		        varsRegistered = waitFor(Particle.connected, 1000);
		    }
		    else {connected = client.connect(hostname, 80);}
        }
    
        requestTime=millis();
        while((client.available()==0)&&((millis()-requestTime)<RESPONSE_TIMEOUT)) {
            Particle.process();    //process Spark events
        };
        
        headers=TRUE;
        lastChar='\n';
        response="";
    	while(client.available()>0) {
    		char thisChar=client.read();
    		if(!headers)
    		    response.concat(String(thisChar));
    		else {
    			if((thisChar=='\r')&&(lastChar=='\n')) {
        			headers=FALSE;
        			client.read();  //kill that last \n
    			}
    			lastChar=thisChar;  
    		}
            itoa(client.available(), debug, 10);
    	}

        //if there's a valid hex color string from Cheerlights, update the color
        if(response.length()==7) {
            //convert the hex values from the response.body string into byte values
    		red=hexToInt(response.charAt(1))*16+hexToInt(response.charAt(2));
    		green=hexToInt(response.charAt(3))*16+hexToInt(response.charAt(4));
    		blue=hexToInt(response.charAt(5))*16+hexToInt(response.charAt(6));
        	cheerLightsColor = Color(red, green, blue);
        }
        else {
            sprintf(debug, "no reply from host");
          	
            client.stop();
		    response = "";
		    if(!Particle.connected) {
		        Particle.connect();
		        varsRegistered = waitFor(Particle.connected, 1000);
		    }
		    else {connected = client.connect(hostname, 80);}
        }
    }
}*/

void filler(uint32_t c1, uint32_t c2, uint32_t c3) {
    static uint32_t whichColor = -1, whichFill;
    Color col;
    run = TRUE;

    if(switch1) {
        whichColor = Wheel(random(random(3, 256), random(3, 256)));
        col = getColorFromInteger(whichColor);
    }
    else {
        if(whichColor >= 2) {whichColor = 0;} else {whichColor++;}
	    switch(whichColor) {
	        case 0:
	            col = getColorFromInteger(c1);
	            break;
	        case 1:
	            col = getColorFromInteger(c2);
	            break;
	        case 2:
	            col = getColorFromInteger(c3);
	            break;
	    }
    }
    
    if(col != lastCol) {
    	lastCol = col;
        whichFill = random(0, 3);
        switch(whichFill) {
            case 0:
                fillX(col);
                break;
            case 1:
                fillY(col);
                break;
            case 2:
                fillZ(col);
                break;
        }
    }
    if(stop) {demo = FALSE; return;}
    if(demo) {if(millis() - lastModeSet > twoMinuteInterval) {return;}}
    delay(speed);
}

void fillX(Color col) {
    int whichX = random(0, 10);
    int whichY = random(0, 10);
    int whichZ = random(0, 10);
    int startX = (whichX%2 == 0) ? SIDE-1 : 0;
    int startY = (whichY%2 == 0) ? SIDE-1 : 0;
    int startZ = (whichZ%2 == 0) ? SIDE-1 : 0;

	for(int x=startX;(startX > 0 ? x>=0 : x<SIDE);(startX > 0 ? x-- : x++))
	    for(int y=startY;(startY > 0 ? y>=0 : y<SIDE);(startY > 0 ? y-- : y++))
			for(int z=startZ;(startZ > 0 ? z>=0 : z<SIDE);(startZ > 0 ? z-- : z++)) {
                int index=z*SIDE*SIDE+y*SIDE+x;
                setPixelColor(x,y,z,col);
                if((index%2==0)||(index==(SIDE*SIDE*SIDE)-1)) {
                    if(stop) {demo = FALSE; return;}
                    if(demo) {if(millis() - lastModeSet > twoMinuteInterval) {return;}}
                    showPixels();
                    delay(speed);
                }
            }
}

void fillY(Color col) {
    int whichX = random(0, 10);
    int whichY = random(0, 10);
    int whichZ = random(0, 10);
    int startX = (whichX%2 == 0) ? SIDE-1 : 0;
    int startY = (whichY%2 == 0) ? SIDE-1 : 0;
    int startZ = (whichZ%2 == 0) ? SIDE-1 : 0;

	for(int y=startY;(startY > 0 ? y>=0 : y<SIDE);(startY > 0 ? y-- : y++))
	    for(int z=startZ;(startZ > 0 ? z>=0 : z<SIDE);(startZ > 0 ? z-- : z++))
			for(int x=startX;(startX > 0 ? x>=0 : x<SIDE);(startX > 0 ? x-- : x++)) {
                int index=z*SIDE*SIDE+y*SIDE+x;
                setPixelColor(x,y,z,col);
                if((index%2==0)||(index==(SIDE*SIDE*SIDE)-1)) {
                    if(stop) {demo = FALSE; return;}
                    if(demo) {if(millis() - lastModeSet > twoMinuteInterval) {return;}}
                    showPixels();
                    delay(speed);
                }
            }
}

void fillZ(Color col) {
    int whichX = random(0, 10);
    int whichY = random(0, 10);
    int whichZ = random(0, 10);
    int startX = (whichX%2 == 0) ? SIDE-1 : 0;
    int startY = (whichY%2 == 0) ? SIDE-1 : 0;
    int startZ = (whichZ%2 == 0) ? SIDE-1 : 0;

	for(int z=startZ;(startZ > 0 ? z>=0 : z<SIDE);(startZ > 0 ? z-- : z++))
	    for(int x=startX;(startX > 0 ? x>=0 : x<SIDE);(startX > 0 ? x-- : x++))
			for(int y=startY;(startY > 0 ? y>=0 : y<SIDE);(startY > 0 ? y-- : y++)) {
                int index=z*SIDE*SIDE+y*SIDE+x;
                setPixelColor(x,y,z,col);
                if((index%2==0)||(index==(SIDE*SIDE*SIDE)-1)) {
                    if(stop) {demo = FALSE; return;}
                    if(demo) {if(millis() - lastModeSet > twoMinuteInterval) {return;}}
                    showPixels();
                    delay(speed);
                }
            }
}

void cubes(uint32_t c1, uint32_t c2, uint32_t c3, uint32_t c4) {
//void cubes() {
    //static int frameCount = 0;
    run = TRUE;
  
    background(black);
    Point topLeft=Point{0, 0, 0};
    
    if(switch2) {
        if (flipColor) {
            cubeCol = getColorFromInteger(Wheel(random(random(2,256),random(2,256))));
            flipColor = FALSE;
        }
    }
    
    switch(mode) {
        case(0):
            topLeft=Point{0, 0, 0};
            if(!switch2) {cubeCol=getColorFromInteger(c1);}    //Color{255, 0, 0};
            break;
        case(1):
            topLeft=Point{SIDE-1-side, 0, 0};
            if(!switch2) {cubeCol=getColorFromInteger(c2);}    //Color{255, 255, 0};
            break;
        case(2):
            topLeft=Point{SIDE-1-side, SIDE-1-side, 0};
            if(!switch2) {cubeCol=getColorFromInteger(c3);}    //Color{0, 255, 0};
            break;
        case(3):
            topLeft=Point{0, SIDE-1-side, 0};
            if(!switch2) {cubeCol=getColorFromInteger(c4);}    //Color{0, 0, 255};
            break;
        case(4):
            topLeft=Point{0, 0, SIDE-1-side};
            if(!switch2) {cubeCol=getColorFromInteger(c1);}    //Color{255, 0, 255};
            break;
        case(5):
            topLeft=Point{SIDE-1-side, 0, SIDE-1-side};
            if(!switch2) {cubeCol=getColorFromInteger(c2);}    //Color{0, 255, 255};
            break;
        case(6):
            topLeft=Point{SIDE-1-side, SIDE-1-side, SIDE-1-side};
            if(!switch2) {cubeCol=getColorFromInteger(c3);}    //Color{255, 255, 255};
            break;
        case(7):
            topLeft=Point(0, SIDE-1-side, SIDE-1-side);
            if(!switch2) {cubeCol=getColorFromInteger(c4);}    //Color{0, 180, 130};
            break;
        //frameCount++;
    }
    
    drawCube(topLeft, side, cubeCol);
    //if (frameCount%5==0)
        cubeInc();

    if(stop) {demo = FALSE; return;}
    if(demo) {if(millis() - lastModeSet > twoMinuteInterval) {return;}}
    showPixels();
    delay(speed);
}

/*========== cubes helper functions ==========*/
void cubeInc() {
    side+=inc;
    if ((side==SIDE-1)||(side==0)) {
        delay(speed+(speed*.5));
        inc*=-1;
    }
    if (side==0) {
        mode++;
        flipColor = TRUE;
        //frameCount = 0;
    }
    if (mode>7)
        mode=0;
}

void drawCube(Point topLeft, int side, Color col) {
    Color complementary=complement(col);
    Point topPoints[4];
    Point bottomPoints[4];
    topPoints[0]=topLeft;
    topPoints[1]=Point{topLeft.x+side, topLeft.y, topLeft.z};
    topPoints[2]=Point{topLeft.x+side, topLeft.y+side, topLeft.z};
    topPoints[3]=Point{topLeft.x, topLeft.y+side, topLeft.z};
    Point bottomLeft=Point{topLeft.x, topLeft.y, topLeft.z+side};
    bottomPoints[0]=bottomLeft;
    bottomPoints[1]=Point{bottomLeft.x+side, bottomLeft.y, bottomLeft.z};
    bottomPoints[2]=Point{bottomLeft.x+side, bottomLeft.y+side, bottomLeft.z};
    bottomPoints[3]=Point{bottomLeft.x, bottomLeft.y+side, bottomLeft.z};
    
    //draw bounding lines
    for (int i=0; i<4; i++) {
        drawLine(topPoints[i], bottomPoints[i], (switch3 ? complementary : col));
        drawLine(topPoints[i], topPoints[(i+1)%4], (switch3 ? complementary : col));
        drawLine(bottomPoints[i], bottomPoints[(i+1)%4], (switch3 ? complementary : col));
        if(stop) {demo = FALSE; return;}
        if(demo) {if(millis() - lastModeSet > twoMinuteInterval) {return;}}
    }
    
    for (int x=topLeft.x; x<=topLeft.x+side; x++)
        for (int y=topLeft.y; y<=topLeft.y+side; y++)
            for (int z=topLeft.z; z<=topLeft.z+side; z++) {
                //fill the cube?
                if(switch1) {setPixelColor(x,y,z, col);}
                    if((x==topLeft.x || y==topLeft.y || z==topLeft.z) || 
                       (x==topLeft.x+side || y==topLeft.y+side || z==topLeft.z+side)) {
                            //bleed edge color?
                            if (switch3) {setPixelColor(x,y,z, complementary);}
                            //bleed main color?
                            if (switch4) {setPixelColor(x,y,z, col);}
                       }
            }

    if(!switch4) {
        //paint the 4 edges of the cube with a complementary color
        for (int i=0; i<4; i++) {
            setPixelColor(topPoints[i], complementary);
            setPixelColor(bottomPoints[i], complementary);
            if(stop) {demo = FALSE; return;}
            if(demo) {if(millis() - lastModeSet > twoMinuteInterval) {return;}}
        }
    }
}
/*============================================*/

/** credit: Bill Marrs **/
void whirlWind() {
    run = TRUE;
    
    if (millis() - lastSwap > CYCLE_INTERVAL) {
        lastSwap = millis();
        for (int i=0; i<MAX_DOTS; i++) {
            y[i] = random(SIDE);
            radi[i] = random(MIN_RADI,MAX_RADI) + randomDecimal();
            angle[i] = randomDecimal() * 2 * PI;
            //clr[i] = Color(rand()%16, rand()%16, rand()%16);
            randomColor(&clr[i]);
        }    
    }
    
    background(black);
    
    for (int i=0; i<MAX_DOTS; i++) {
        //draw dots
        for (int i=0; i<MAX_DOTS; i++) {
            setPixelColor(center.x + radi[i] * cos(angle[i]), y[i], center.z + radi[i] * sin(angle[i]), clr[i]);
        }    
        if(stop) {demo = FALSE; return;}
        if(demo) {if(millis() - lastModeSet > twoMinuteInterval) {return;}}
        
        //move dots
        for (int i=0; i<MAX_DOTS; i++) {
            angle[i] += 2 * PI / arcs; 
            if (angle[i] > 2 * PI) {angle[i] -= 2*PI;}
        
            radi[i] += randomDecimal() / 200;
            y[i]    += randomDecimal() / 100;
            
            if (y[i] > SIDE || radi[i] > MAX_RADI) {
                y[i] = 0;
                radi[i] = MIN_RADI;
            }
        }    
        if(stop) {demo = FALSE; return;}
        if(demo) {if(millis() - lastModeSet > twoMinuteInterval) {return;}}
    }    
    showPixels();
    delay(speed * .5);
}

/*========== whirlWind helper functions ==========*/
void randomColor(struct Color *clr) {
  int r;
  do {
    r = random(7);
  } while (r == lastRand || r == lastLastRand);
  
  switch (r) {
    case 0: 
      clr->red   = random(3,128);
      clr->green = random(3,128);
      clr->blue  = random(3,128);
      break;
    case 1: 
      clr->red   = random(3,128);
      clr->green = random(2);
      clr->blue  = random(2);
      break;
    case 2: 
      clr->red   = random(2);
      clr->green = random(3,128);
      clr->blue  = random(2);
      break;
    case 3: 
      clr->red   = random(2);
      clr->green = random(2);
      clr->blue  = random(3,128);
      break;
    case 4: 
      clr->red   = random(2);
      clr->green = random(3,128);
      clr->blue  = random(3,128);
      break;
    case 5: 
      clr->red   = random(3,128);
      clr->green = random(2);
      clr->blue  = random(3,128);
      break;
    case 6: 
      clr->red   = random(3,128);
      clr->green = random(3,128);
      clr->blue  = random(2);
      break;
  }
  lastLastRand = lastRand;
  lastRand = r;
}
/*================================================*/

//A colored Christmas light string that twinkles
void christmasLights(void) {
    uint16_t i, f;
    int speedfactor = 12;    //increase the delay time
    run = TRUE;
    
    for (f=0; f < SIDE; f++) {
        for (i=0; i < strip.numPixels(); i++) {
            if(i%(SIDE-1)==0) strip.setPixelColor(i, 255,0,0 );
            if(i%(SIDE-1)==1) strip.setPixelColor(i, 255,255,0 );
            if(i%(SIDE-1)==2) strip.setPixelColor(i, 0,255,0 );
            if(i%(SIDE-1)==3) strip.setPixelColor(i, 0,255,255 );
            if(i%(SIDE-1)==4) strip.setPixelColor(i, 0,0,255 );
            if(i%(SIDE-1)==5) strip.setPixelColor(i, 255,0,255 );
            if(i%(SIDE-1)==6) strip.setPixelColor(i, 249,115,6 );
            if(i%(SIDE-1)==7) strip.setPixelColor(i, 194,0,120 );
        }
        if(stop) {demo = FALSE; run = TRUE; return;}
        if(demo) {if(millis() - lastModeSet > twoMinuteInterval) {run = TRUE;}}
        showPixels();
        delay(speed*speedfactor);
        for (int i=0; i < strip.numPixels(); i++)
            if(i%(SIDE-1)==f) strip.setPixelColor(i, 0,0,0 );
        if(stop) {demo = FALSE; run = TRUE; return;}
        if(demo) {if(millis() - lastModeSet > twoMinuteInterval) {run = TRUE;}}
        showPixels();
        delay(speed*speedfactor);
    }
}

/*** credit: Kevin's friggin' xmas tree - there, have it! **/
void christmasTree(void) {
    int xOrigin = 3; 
    int yOrigin = 3;
    int speedfactor = 8;    //increase the delay time,
    float radius  = 4;
    static int starColorIdx = 16;
    static bool flipped = FALSE;
    run = TRUE;

    if(isFirstLap) {
        //Then we need to fade any voxels still lit
        for(int idx = 0; idx < PIXEL_CNT; idx++) {
            if(strip.getPixelColor(idx) > 0) {
                transition(black, true);
                return;
            }
        }
        //Paint the tree
        for(int z=0;z<7;z+=2,radius--) {
            int gradient = switch1 ? PIXEL_CNT-(radius)*(z*(radius)) : radius+(z*(radius));
            Color gradedGreen = {fadeSqRt(green.red+gradient),fadeLinear(green.green+gradient),fadeSquare(green.blue+gradient)};
            drawSolidHorizontalCircle(xOrigin, yOrigin, z, radius, gradedGreen);
            drawSolidHorizontalCircle(xOrigin, yOrigin, z+1, radius-2, gradedGreen);
            //drawHollowHorizontalCircle(xOrigin, yOrigin, z-1, radius-1, black, TRUE);
            //drawHollowHorizontalCircle(xOrigin, yOrigin, z, radius, black, TRUE);
        }
        //make the star
        setPixelColor(xOrigin,7,yOrigin,yellow);
        isFirstLap = FALSE;
    }
    
    //draw lights?
    if(switch3) {   //drawing a friggin' inverse conical helix with just a friggin' 8-bit 3D resolution is a friggin' PAIN!!
        Color col;
        radius = 4;
        for(int z=0;z<7;z++) {
            for(int d=0; d<=(2*PI); d++)
                if(z%2>0) {
                    randomColor(&col);
                    setPixelColor(xOrigin+((radius-.75)*cos(d)), z, yOrigin+((radius-.75)*sin(d)), col);  //getColorFromInteger(Wheel(random(random(2,256),random(2,256)))));
                }
                else {
                    randomColor(&col);
                    setPixelColor(xOrigin+(radius*cos(d)), z, yOrigin+(radius*sin(d)), col);    //getColorFromInteger(Wheel(random(random(2,256),random(2,256)))));
                }
            if(z%2>0)   //Yeah, tryin' to slow down the friggin' helix turns from tightening up too fast
                radius-=.75;
            else
                radius-=.25;
        }
    }
    
    //make the star pulse?
    if(switch2) {
        Color starColor;
        if (!flipped)
            if(starColorIdx < 128)
                starColorIdx+=8;
            else {
                starColorIdx = 128;
                flipped = TRUE;
            }
        else
            if(starColorIdx > 8)
                starColorIdx-=8;
            else {
                starColorIdx = 8;
                flipped = FALSE;
            }
        if(switch1)
            starColor = {fadeSqRt(starColorIdx),fadeLinear(starColorIdx),fadeSquare(starColorIdx)};
        else
            starColor = getColorFromInteger(lerpColor(strip.Color(starColor.red, starColor.green, starColor.blue), strip.Color(yellow.red, yellow.green, yellow.blue), starColorIdx, 16, 128));
        setPixelColor(xOrigin,7,yOrigin,starColor);
    }

    //make it snow?
    if(switch1) {
        //So you want some snow, do ya
        int whiteLevel;
        Color flakeColor;
        
        //First lets move any flakes that exist
        for(int x=0;x<8;x++) {
            for(int z=0;z<8;z++) {
                for(int y=0;y<8;y++) {
                    if(isWhiteColor(getPixelColor(x,z,y))) {
                        setPixelColor(x,z,y,black);  // If the voxel is white turn it black
                        if(getPixelColor(x,z-1,y)==black) {
                            //If the next voxel is black then continue the flake down
                            //Otherwise the flake melts, magically disapears, or whatever ya wanna call it
                            whiteLevel = random(80, 255);
                            flakeColor = {whiteLevel, whiteLevel, whiteLevel};
                            setPixelColor(x,z-1,y,flakeColor);  
                        }
                    }
                }
            }
        }
        //Now lets make new flakes - anywhere between 1 and 4 at a time
        int numFlakes = random(1,4);
        for(uint16_t i=0;i<numFlakes;i++) {
            Point flake = {0,7,0};
            do{
                flake.x = random(0,7);
                flake.z = random(0,7);
            } while(getPixelColor(flake)!=black);
            //make a flake
            whiteLevel = random(80, 255);
            flakeColor = {whiteLevel, whiteLevel, whiteLevel};
            setPixelColor(flake,flakeColor);
        }
    }
    if(stop) {demo = FALSE; return;}
    if(demo) {if(millis() - lastModeSet > twoMinuteInterval) {return;}}
    showPixels();
    delay(speed*speedfactor);
}

void christmasWreath(uint32_t color1, uint32_t color2) {
    double i;
    int j, k, l, m;
    int speedFactor = 2;    //increase the delay time
    int currentBrightness = brightness;
    static int originalBrightness = brightness;
    run = TRUE;

    //fade in
    for (i=0; i < currentBrightness; i++) {
        for(j=zone1Start; j<=zone1End; j++)
            strip.setPixelColor(j, color1);
        for(k=zone2Start; k<=zone2End; k++)
            strip.setPixelColor(k, color2);
        for(l=zone3Start; l<=zone3End; l++)
            strip.setPixelColor(l, color1);
        for(m=zone4Start; m<=zone4End; m++)
            strip.setPixelColor(m, color2);
    	brightness = i; // sin(i) * 127.5 + 127.5;
        if(stop) {brightness = originalBrightness; demo = FALSE; return;}
        if(demo) {if(millis() - lastModeSet > twoMinuteInterval) {brightness = originalBrightness; return;}}
    	showPixels();
    	delay(speedFactor*speed);
    }

    //fade out
	for (i=currentBrightness; i > 0; i--) {
        for(j=zone1Start; j<=zone1End; j++)
            strip.setPixelColor(j, color1);
        for(k=zone2Start; k<=zone2End; k++)
            strip.setPixelColor(k, color2);
        for(l=zone3Start; l<=zone3End; l++)
            strip.setPixelColor(l, color1);
        for(m=zone4Start; m<=zone4End; m++)
            strip.setPixelColor(m, color2);
    	brightness = i;
        if(stop) {brightness = originalBrightness; demo = FALSE; return;}
        if(demo) {if(millis() - lastModeSet > twoMinuteInterval) {brightness = originalBrightness; return;}}
    	showPixels();
    	delay(speedFactor*speed);
    }
    
    //alternator colors, fade in
    for (i=0; i < currentBrightness; i++) {
        for(j=zone1Start; j<=zone1End; j++)
            strip.setPixelColor(j, color1);
        for(k=zone2Start; k<=zone2End; k++)
            strip.setPixelColor(k, color2);
        for(l=zone3Start; l<=zone3End; l++)
            strip.setPixelColor(l, color1);
        for(m=zone4Start; m<=zone4End; m++)
            strip.setPixelColor(m, color2);
    	brightness = i; // sin(i) * 127.5 + 127.5;
        if(stop) {brightness = originalBrightness; demo = FALSE; return;}
        if(demo) {if(millis() - lastModeSet > twoMinuteInterval) {brightness = originalBrightness; return;}}
    	showPixels();
    	delay(speedFactor*speed);
    }

    //fade out
	for (i=currentBrightness; i > 0; i--) {
        for(j=zone1Start; j<=zone1End; j++)
            strip.setPixelColor(j, color1);
        for(k=zone2Start; k<=zone2End; k++)
            strip.setPixelColor(k, color2);
        for(l=zone3Start; l<=zone3End; l++)
            strip.setPixelColor(l, color1);
        for(m=zone4Start; m<=zone4End; m++)
            strip.setPixelColor(m, color2);
    	brightness = i;
        if(stop) {brightness = originalBrightness; demo = FALSE; return;}
        if(demo) {if(millis() - lastModeSet > twoMinuteInterval) {brightness = originalBrightness; return;}}
    	showPixels();
    	delay(speedFactor*speed);
    }
    brightness = currentBrightness;
}

/* Trailing Rain Drops
 * @param c1 Selected rain drop color from the app
 *           or use boolean switch1 to toggle random rain drop colors */
void rain(uint32_t c) {
    Color col;
    int speedfactor = 3;    //increase the delay time
    run = TRUE;
    
    //In case we don't have blank playing field yet, make it blank
    if(isFirstLap) {
        transitionAll(black, LINEAR);    //fadeToBlack();
        isFirstLap = FALSE;
    }
	
    //First lets move any drops that exist
    //Start at the bottom and look up each y-axis column
    for(int x=0;x<SIDE;x++) {
        for(int z=0;z<SIDE;z++) {
            for(int y=0;y<SIDE;y++) {
                Color pixelColor = getPixelColor(x,y,z);
                if(pixelColor != black) {
                    //We found a color!
                    int tailEndPosition=y+2;    
                    float firstScaledFactor = switch2 ? 1 : 0.5;    //faded tail scaling
                    float secondScaledFactor = switch2 ? 1 : 0.125;  //faded tail scaling
                    if(!switch2) {
                        if(y==0) {
                            //If we are at the bottom lets diminish the tail properly by finding it's end position
                            for(int d=0;d<SIDE;d++) {
                                if(getPixelColor(x,d,z) == black) {
                                    tailEndPosition = d-1;
                                    break;
                                }
                            }
                        }
                    }
                    if(tailEndPosition>=2) {
                        setPixelColor(x,y-1,z,pixelColor);
                        setPixelColor(x,y  ,z,fadeColor(pixelColor,firstScaledFactor));
                        setPixelColor(x,y+1,z,fadeColor(pixelColor,secondScaledFactor)); 
                    } else if(tailEndPosition==1) {
                        setPixelColor(x,y  ,z,fadeColor(pixelColor,secondScaledFactor/firstScaledFactor));
                    }
                    setPixelColor(x,tailEndPosition,z,((switch2 || switch3) ? pixelColor : black));   //setPixelColor(x,tailEndPosition,z,black);
                    y=tailEndPosition; //Lets look for another rain drop above this one
                }
            }
        }
    }
    
    if(switch2) {
        //Then slowly fade out previously-lit pixels to black, leaving a nice "trailing" effect
        for(int x=0;x<SIDE;x++) {
            for(int z=0;z<SIDE;z++) {
                for(int y=SIDE-1;y>=0;y--) {
                    Color pixelColor = getPixelColor(x,y,z);
                    if(pixelColor.red > 0) pixelColor.red-=pixelColor.red*.125;
                    if(pixelColor.green > 0) pixelColor.green-=pixelColor.green*.125;
                    if(pixelColor.blue > 0) pixelColor.blue-=pixelColor.blue*.125;
                    setPixelColor(x,y,z, pixelColor);
                }
            }
        }
    }
    
    //Let's make some new drops at the top anywher from 5 to 15 at a time
    int numRainDrops = random(5,16);
    for(uint16_t i=0;i<numRainDrops;i++) {
        //We don't want to start a drop unless there are two black pixels at the top of the column
        //So let's look to see if it's possible to start the next drop
        for(int x=0;x<SIDE;x++) {
            for(int z=0;z<SIDE;z++) {
                if(getPixelColor(x,SIDE-2,z)==black && getPixelColor(x,SIDE-1,z)==black) {
                    //It's possible to make a drop in at least one spot, so let's make one
                    Point rainDrop = {0,SIDE-1,0};
                    do{
                        rainDrop.x = random(0,SIDE);
                        rainDrop.z = random(0,SIDE);
                        //let's bail out if we need to
                        if(stop) {demo = FALSE; run = TRUE; return;}
                        if(demo) {if(millis() - lastModeSet > twoMinuteInterval) {return;}}
                        //Lets keep looking for a new rain drop if we found a drop or it's tail
                        //And lets leave an extra space between 1 drop (w/ tail) and the next drop too
                    } while(getPixelColor(rainDrop)!=black || getPixelColor(rainDrop.x,SIDE-2,rainDrop.z)!=black);
                        
                    //Random color drop or predetermined
                    if(switch1) { col = getColorFromInteger(Wheel(random(random(2,256),random(2,256)))); } 
                    else        { col = getColorFromInteger(c); }
                    //make a rain drop
                    setPixelColor(rainDrop,col);
                    x=z=SIDE;
                }
            }
        }
    }
    if(stop) {demo = FALSE; run = TRUE; return;}
    if(demo) {if(millis() - lastModeSet > twoMinuteInterval) {return;}}
    showPixels();
    delay(speed*speedfactor);
}

/* Inspired by Kevin's musically inclined work neighbor
 * This mode simulates halogen stage lights turning On and then Off
 * They start off redish-orange and warm up to bright white
 */
void warmFade(void) {
    float i; 
    Color col;
    run = TRUE;
    
    for(i=0; i<256; i++) {
        col = {fadeSqRt(i),fadeLinear(i),fadeSquare(i)};
        background(col);
        if(stop) {demo = FALSE; return;}
        if(demo) {if(millis() - lastModeSet > twoMinuteInterval) {return;}}
        showPixels();
        delay(speed);
    }
    for(i=255; i>0; i--) {
        col = {fadeSqRt(i),fadeLinear(i),fadeSquare(i)};
        background(col);
        if(stop) {demo = FALSE; return;}
        if(demo) {if(millis() - lastModeSet > twoMinuteInterval) {return;}}
        showPixels();
        delay(speed);
    }
}

// warmFade helper function
uint8_t fadeSquare(float value) {
    return (uint8_t)(255*pow(value/255,2));
}

// warmFade helper function
uint8_t fadeSqRt(float value) {
    return (uint8_t)(255*sqrt(value/255));
}

// warmFade helper function
uint8_t fadeLinear(float value) {
    return (uint8_t) value;
}

int colorZone(uint32_t c1, uint32_t c2, uint32_t c3, uint32_t c4, bool loop) {
    uint32_t maxColorPixel, increment;
    Color col1 = ((switch2 || switch3) ? getColorFromInteger(Wheel(random(random(3, 256), random(3, 256)))) : getColorFromInteger(c1));
    Color col2, col3, col4;
    if(switch2) {
		col2 = getColorFromInteger(Wheel(random(3, strip.Color(col1.red, col1.green, col1.blue)), 
				random(strip.Color(col1.red, col1.green, col1.blue)+1, 256)));
		col3 = getColorFromInteger(Wheel(random(3, strip.Color(col2.red, col2.green, col2.blue)), 
				random(strip.Color(col2.red, col2.green, col2.blue)+1, 256)));
		col4 = getColorFromInteger(Wheel(random(3, strip.Color(col3.red, col3.green, col3.blue)), 
				random(strip.Color(col3.red, col3.green, col3.blue)+1, 256)));
    }
    if(switch3) {
		col2 = getColorFromInteger(random(random(3, strip.Color(col1.red, col1.green, col1.blue)), 
				random(strip.Color(col1.red, col1.green, col1.blue)+1, 256)));
		col3 = getColorFromInteger(random(random(3, strip.Color(col2.red, col2.green, col2.blue)), 
				random(strip.Color(col2.red, col2.green, col2.blue)+1, 256)));
		col4 = getColorFromInteger(random(random(3, strip.Color(col3.red, col3.green, col3.blue)), 
				random(strip.Color(col3.red, col3.green, col3.blue)+1, 256)));
    }
	if ((!switch2) && (!switch3)) {
		col2 = getColorFromInteger(c2);
		col3 = getColorFromInteger(c3);
		col4 = getColorFromInteger(c4);
	}
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
        for(int idx = 0; idx < PIXEL_CNT; idx++) {
            if(strip.getPixelColor(idx) > 0) {
                transition(black, true);
                return;
            }
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
uint32_t Wheel(byte WheelPos, float opacity) {
	if(WheelPos < 85) {
		return strip.Color((WheelPos * 3) * opacity, (255 - WheelPos * 3) * opacity, 0);
	}
	else if(WheelPos < 170) {
		WheelPos -= 85;
		return strip.Color((255 - WheelPos * 3) * opacity, 0, (WheelPos * 3) * opacity);
	}
	else {
		WheelPos -= 170;
		return strip.Color(0, (WheelPos * 3) * opacity, (255 - WheelPos * 3) * opacity);
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

void mixVoxel(Point currentPoint, Color col) {
  Color currentCol=getPixelColor(currentPoint);
  Color newCol=Color{currentCol.red+col.red, currentCol.green+col.green, currentCol.blue+col.blue};
  setPixelColor(currentPoint, newCol);
}

// @param scaleFactor is a percentage of the current color in decimal form, i.e 0.125
Color fadeColor(Color col, float scaleFactor) {
    col.red=col.red*scaleFactor;
    col.green=col.green*scaleFactor;
    col.blue=col.blue*scaleFactor;
    return col;
}

Color complement(Color original) {
  float R = original.red;
  float G = original.green;
  float B = original.blue;
  float minRGB = min(R, min(G, B));
  float maxRGB = max(R, max(G, B));
  float minPlusMax = minRGB + maxRGB;
  return Color{minPlusMax-R, minPlusMax-G, minPlusMax-B};;
}

//Exactly what you think it does
//credit: http://stackoverflow.com/questions/1201200/fast-algorithm-for-drawing-filled-circles
void drawSolidHorizontalCircle(int xOrigin, int yOrigin, int z, int radius, Color col) {
    for(int y=-radius; y<=radius; y++)
        for(int x=-radius; x<=radius; x++)
            if(x*x+y*y <= radius*radius)
                setPixelColor(xOrigin+x, z, yOrigin+y, col);
}

void drawHollowHorizontalCircle(int xOrigin, int yOrigin, int z, int radius, Color col, bool rndColor=FALSE) {
    for(int y=-radius; y<=radius; y++) {
        for(int x=-radius; x<=radius; x++) {
            if(x*x+y*y == radius*radius) {
                if(rndColor)
                    setPixelColor(xOrigin+x, z, yOrigin+y, getColorFromInteger(Wheel(random(2,256))));
                else
                    setPixelColor(xOrigin+x, z, yOrigin+y, col);
            }
        }
    }
}

// draws a line from point p1 to p2 and colors each of the points according
// to the col parameter
// p1 and p2 can be outside of the cube, but it will only draw the parts of
// the line that fall
// inside the cube
void drawLine(Point p1, Point p2, Color col) {
  // thanks to Anthony Thyssen for the original write of Bresenham's line
  // algorithm in 3D
  // http://www.ict.griffith.edu.au/anthony/info/graphics/bresenham.procs

  float dx, dy, dz, l, m, n, dx2, dy2, dz2, i, x_inc, y_inc, z_inc, err_1, err_2;
  Point currentPoint = Point{p1.x, p1.y, p1.z};
  dx = p2.x - p1.x;
  dy = p2.y - p1.y;
  dz = p2.z - p1.z;
  x_inc = (dx < 0) ? -1 : 1;
  l = abs(dx);
  y_inc = (dy < 0) ? -1 : 1;
  m = abs(dy);
  z_inc = (dz < 0) ? -1 : 1;
  n = abs(dz);
  dx2 = l * 2;
  dy2 = m * 2;
  dz2 = n * 2;

  if ((l >= m) && (l >= n)) {
    err_1 = dy2 - l;
    err_2 = dz2 - l;
    for (i = 0; i < l; i++) {
      mixVoxel(currentPoint, col);
      if (err_1 > 0) {
        currentPoint.y += y_inc;
        err_1 -= dx2;
      }
      if (err_2 > 0) {
        currentPoint.z += z_inc;
        err_2 -= dx2;
      }
      err_1 += dy2;
      err_2 += dz2;
      currentPoint.x += x_inc;
    }
  } else if ((m >= l) && (m >= n)) {
    err_1 = dx2 - m;
    err_2 = dz2 - m;
    for (i = 0; i < m; i++) {
      mixVoxel(currentPoint, col);
      if (err_1 > 0) {
        currentPoint.x += x_inc;
        err_1 -= dy2;
      }
      if (err_2 > 0) {
        currentPoint.z += z_inc;
        err_2 -= dy2;
      }
      err_1 += dx2;
      err_2 += dz2;
      currentPoint.y += y_inc;
    }
  } else {
    err_1 = dy2 - n;
    err_2 = dx2 - n;
    for (i = 0; i < n; i++) {
      mixVoxel(currentPoint, col);
      if (err_1 > 0) {
        currentPoint.y += y_inc;
        err_1 -= dz2;
      }
      if (err_2 > 0) {
        currentPoint.x += x_inc;
        err_2 -= dz2;
      }
      err_1 += dy2;
      err_2 += dx2;
      currentPoint.z += z_inc;
    }
  }
  mixVoxel(currentPoint, col);
}

float randomDecimal() {
  return static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
}

float radius(float x, float y, float z) {
  return sqrt(pow(center.x - x, 2) + pow(center.z - z, 2));
}

//Checks to see if it's any gradient of white
bool isWhiteColor(Color col) {
	return (col.red == col.green && col.red == col.blue && col.red > 64);
}

/** Find index of antipodal opposite LED **/
int antipodal_index(int i) {
    int iN = i + zone2End;
    if (i >= zone2End)
        iN = ( i + zone2End ) % PIXEL_CNT; 
    return iN;
}

uint8_t clamp(unsigned value, unsigned lowClamp, unsigned highClamp) {
	return ((value<lowClamp) ? lowClamp : (value>highClamp) ? highClamp : value);
}

void arrayShuffle(int arrayToShuffle[], int arraySize) {
    uint16_t i; 
    char cbuff[20];

    for(i=0;i<arraySize;i++) {
        int r = random(0,arraySize);  // generate a random position
        int temp = arrayToShuffle[i]; 
        arrayToShuffle[i] = arrayToShuffle[r]; 
        arrayToShuffle[r] = temp;
        //sprintf(cbuff,"%i %i %s ", arraySize,r,modeStruct[getModeIndexFromID(arrayToShuffle[i])].modeName);
        //strcat(debug,cbuff);
    }
}

/** Break down an int color into its 3 rgb components for the given pixel 
  @param index The strip index of the pixel.
  @return The rgb Color for the given pixel.*/
Color getPixelColor(int index) {
    uint32_t col = strip.getPixelColor(index);
    return getColorFromInteger(col);
}

/** Get the color of a voxel at a position.
  @param x, y, z Coordinate of the LED to set.
  @return The Color for the given pixel.
  */
Color getPixelColor(int x, int y, int z) {
    int index = (z*64) + (x*8) + y;
    uint32_t col = strip.getPixelColor(index);
    return getColorFromInteger(col);
}

/** Get the color of a voxel at a position
  @param p Coordinate of the LED to get.
  @return The Color for the given pixel.
  */
Color getPixelColor(Point p) {
    return getPixelColor(p.x, p.y, p.z);
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

/** Convert a given hex color value (e.g., 'FF') to integer (e.g., 255)*/
int hexToInt(char val) {
    int v = (val > '9')? (val &~ 0x20) - 'A' + 10: (val - '0');
    return v;
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
			if(isNewSpeed) {
    			speedIndex = receivedSpeedValue;
    			speed = speedPresets[speedIndex];
			}
		}
		else if(command.charAt(beginIdx) == 'B') {
		    int newBrightness = command.substring(beginIdx+2, idx).toInt() * (255 * .01);	//Scale 0-100 to 0-255
			if(brightness != newBrightness) {isNewBrightness = TRUE;}
			if(isNewBrightness) brightness = newBrightness > 0 ? newBrightness : 1;
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

		// T for Toggle switch - expect 0 or 1 for false or true
		// S for Switch would have made more sense, but want to keep this backwards compatible and S is alreay Speed
		else if(command.charAt(beginIdx) == 'T') { 
            switch(command.charAt(beginIdx+1)) {
                case '1':
                    switch1 = command.substring(beginIdx+3, idx).toInt() & 1;
                    break;
                case '2':
                    switch2 = command.substring(beginIdx+3, idx).toInt() & 1;
                    break;
                case '3':
                    switch3 = command.substring(beginIdx+3, idx).toInt() & 1;
                    break;
                case '4':
                    switch4 = command.substring(beginIdx+3, idx).toInt() & 1;
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
			returnValue = BRIGHTNESS_SET;
		}
		else if(isNewSpeed==true) {
			//Let the sender know we got the new speed command
			returnValue = SPEED_SET;
		}
		else {
			//If we got here, it's possible that a command was set to update speed or brightness
			//But neither one was a new value. The new values, equal the current values
			returnValue = NO_CHANGE;
		}
    }

	// still here, so everything must be fine
	return returnValue;
}

/**
 * Depreciated, replaced by FnRouter
 * ASO has merged into the SetAuxSwitch function
 * 
int SetASO(String command) {
    int returnValue = 0;
    if(command == ASO_CMD_ON) {
        autoShutOff = TRUE;
        returnValue = ASO_STATUS_ON;
    }
    else if(command == ASO_CMD_OFF) {
        autoShutOff = FALSE;
        returnValue = ASO_STATUS_OFF;
    }
    else if(command == ASO_CMD_STATUS) {
        returnValue = autoShutOff ? ASO_STATUS_ON : ASO_STATUS_OFF;
    }
    return returnValue;  
}*/

/** 
 * Miscellaneous Cloud Function Router
 * Expect a string with a single function identifier 
 * followed by a colon followed by parameters
 **/
int FnRouter(String command) {
	// Trim extra spaces
    command.trim();
    // Convert it to upper-case for easier matching
    command.toUpperCase();
	
    int beginIdx = 0;
	int colonIdx = command.indexOf(':');
	
    // Set time zone offset
    if(command.substring(beginIdx, colonIdx)=="SETTIMEZONE") {
		//Expect a string like this: SETTIMEZONE:-6
        timeZone = command.substring(colonIdx+1).toInt();
        Time.zone(timeZone);
        return timeZone;
    }
	else if(command.substring(beginIdx, colonIdx)=="SETAUXSWITCH") {
		//Expect a string like this: SETAUXSWITCH:1,0;
		//That breaks down to: SwitchID,state;
		beginIdx = colonIdx+1;
		int commaIdx = command.indexOf(',');
		int semiColonIdx = command.indexOf(';');
		int id = 0;
		while(semiColonIdx != -1) {
			id = (int) command.substring(beginIdx, commaIdx).toInt();
			bool state = command.substring(commaIdx+1,semiColonIdx).equals("1") ? true : false;
			auxSwitchStruct[getAuxSwitchIndexFromID(id)].auxSwitchState = state;
			
			beginIdx = semiColonIdx + 1;
			commaIdx = command.indexOf(',', beginIdx);
			semiColonIdx = command.indexOf(';', commaIdx);
		}
		
		//Update the list
		makeAuxSwitchList();
		
		//Update Switch flags
		return updateAuxSwitches(id);
	}
	
    return -1;  
 }

//Added Particle Function to get Text Input
int SetText(String command) {
    sprintf(textInputString,"%s", command.c_str());
	return 1;
}

//Change Mode based on the modeStruct array index
int setNewMode(int newModeIndex) {
    currentModeID = modeStruct[newModeIndex].modeId;
    sprintf(currentModeName,"%s", modeStruct[newModeIndex].modeName);
	resetVariables(modeStruct[newModeIndex].modeId);
  	//fadeToBlack();
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

int getAuxSwitchIndexFromID(int id) {
    for(uint16_t i=0;i<(int)(sizeof auxSwitchStruct / sizeof auxSwitchStruct[0]);i++) {
        if(id == auxSwitchStruct[i].auxSwitchId) {
            return i;
        }
    }
    return -1;
}

void listen() {
    static char data[EXPECTED_PACKET_SIZE];
    run = TRUE;
    
    // Checks for the presence of a UDP packet, and reports the buffer size
    unsigned long bytes = Udp.parsePacket();
    /*------------------*/
    /*--- Size check ---*/
    /*------------------*/
    // Do we have sufficient data in the buffer yet?
    if(bytes != EXPECTED_PACKET_SIZE) {
        // Nope - let's hold for another 100 attempts
        if(countdown < 100) 
            countdown++;
        else {
            // No luck, then let's refresh the buffer
            Udp.stop();
            resetVariables(LISTENER);
        }
        return;
    }
    else {
        // Read 199 bytes of data from the buffer
        Udp.read(data, EXPECTED_PACKET_SIZE);

        // Ignore all other chars
        Udp.flush();
        
        /*------------------*/
        /*-- Header check --*/
        /*------------------*/
        // Do we have correct tpm2.net data in the buffer?
        if(data[0] != TPM2NET_HEADER_IDENT) {   // Block Type: Block Start Byte
            // Nope - let's hold for another 100 attempts
            if(countdown < 100) 
                countdown++;
            else {
                // No luck, then let's refresh the buffer
                Udp.stop();
                resetVariables(LISTENER);
            }
            return;
        }
        else {
            /*------------------*/
            /*--- Data check ---*/
            /*------------------*/
            // Do we have a tpm2.net dataframe in the buffer? Or has the user cancelled this mode?
            if((data[1] != TPM2NET_CMD_DATAFRAME) || stop) { // Block Type: Frame Data Packet
                if(stop) {  // Was this a user-requested abort?
                    strip.setBrightness(brightness);
                    Udp.stop();
                    //demo = FALSE;
                }
                else {
                    // Nope - let's hold for another 100 attempts
                    if(countdown < 100) 
                        countdown++;
                    else {
                        // No luck, then let's refresh the buffer
                        Udp.stop();
                        resetVariables(LISTENER);
                    }
                }
                return;
            }
            else {
                countdown = 0;
                // Calculate frame size
                char frameSize = data[2];
                frameSize = (frameSize << 8) + data[3];
        
                // Use packetNumber to calculate offset
                /** GLEDiaTor always sends totalPackets > packetNumber and never updates packetNumber,
                  * Jinx always sends both the same value. So there is no point in considering either.
                char packetNumber = data[4];
                char totalPackets = data[5]; **/
        
                // Calculate offset
                //int index = packetNumber * PIXELS_PER_PANEL;
                int voxelIdx = TPM2NET_HEADER_SIZE; // We set the index pointer to the first frame byte

                // Start drawing!!
                for(int z = NR_OF_PANELS-1; z >= 0; z--) {                  // We're only dealing in 2 dimensions (width & height)
                    for(int col = 0; col < NR_OF_PANELS; col++) {           // Linewise, starting from leftmost index
                        for(int row = NR_OF_PANELS - 1; row >= 0; row--) {  // Columnwise, starting from topmost index
                            Color pixelColor = Color(data[voxelIdx], data[voxelIdx + 1], data[voxelIdx + 2]);  // Take 3 color bytes from buffer
                            setPixelColor(z, col, row, pixelColor);
                        }
                        // Do we still have room to increment?
                        if(voxelIdx <= (frameSize+TPM2NET_HEADER_SIZE)-3)
                            voxelIdx+=3;  // Increment buffer index by 3 bytes
                        else {
                            // No - bail out, refresh buffer
                            Udp.stop();
                            resetVariables(LISTENER);
                            return;
                        }
                    }
                }
                // Display!!
                /** GLEDiaTor always sends totalPackets > packetNumber and never updates packetNumber,
                  * Jinx always sends both the same value. So there is no point in considering either.
                if(packetNumber == totalPackets) { **/
                    if(stop) { 
                        //demo = FALSE;
                        strip.setBrightness(brightness);
                        Udp.stop(); 
                        return;
                    }
                    //if(demo) {if(millis() - lastModeSet > twoMinuteInterval) {strip.setBrightness(brightness); return;}}
              		strip.setBrightness(brightness);
                    strip.show();
                    Particle.process();    //process Spark events
                //}
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
		int ii = (i+1) < ARRAY_SIZE ? i+1 : i;
        imaginary[i]=sqrt(pow(imaginary[ii],2)+pow(real[ii],2));
        //imaginary[i]=sqrt(pow(imaginary[i+1],2)+pow(real[i+1],2));
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
    		if(dotMode && switch2) {
    		    //Down-top fade/blanking
                pixelColor=black;
                pixLowIdx=pixIdx-(pixIdx%SIDE);
                for(int j=pixLowIdx; j<pixIdx; j++) {
                    if(smooth && switch1) {
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
            if(smooth && switch1) {
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
                if(smooth && switch1) {
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
void textSpin(uint32_t color1, uint32_t color2) {
    run = TRUE;
    
    thickness = switch1;
    if(switch2) {color2 = 0;}
    if(switch3) {color1 = 0;}
    
    background(getColorFromInteger(color2));
    scrollSpinningText(message, Point(pos - strlen(message), 0, ceil((SIDE-1)*.5)), getColorFromInteger(color1));
    showPixels();
    if(stop) {return;}
    //(largest_item - smallest_item) maps to (max-min)
    float ratio = (.5 - .05)/((120*.05) - .05);
    //(min + ratio*(value-smallest_item))
    float speedFactor = .05 + ratio * ((map(speed, 1, 120, 120, 1) * .05) - .05);
    pos += speedFactor;
    if (pos >= (SIDE*map(strlen(message), 1, 63, 1, SIDE))+(strlen(message))*8)
        pos = map(strlen(message), 1, 63, -SIDE, 0);
}

void textScroll(uint32_t color1, uint32_t color2) {
    run = TRUE;
    
    thickness = switch1;
    if(switch2) {color2 = 0;}
    if(switch3) {color1 = 0;}
    
    background(getColorFromInteger(color2));
    scrollText(message, Point(pos - strlen(message), 0, 6), getColorFromInteger(color1));
    showPixels();
    if(stop) {return;}
    //(largest_item - smallest_item) maps to (max-min)
    float ratio = (.5 - .05)/((120*.05) - .05);
    //(min + ratio*(value-smallest_item))
    float speedFactor = .05 + ratio * ((map(speed, 1, 120, 120, 1) * .05) - .05);
    pos += speedFactor;
    if (pos >= (SIDE*map(strlen(message), 1, 63, 1, SIDE))+(strlen(message))*8)
        pos = map(strlen(message), 1, 63, -(SIDE*.5), 0);
}

void textMarquee(uint32_t color1, uint32_t color2) {
    run = TRUE;
    
    thickness = switch1;
    if(switch2) {color2 = 0;}
    if(switch3) {color1 = 0;}
    
    background(getColorFromInteger(color2));
    marquee(message, pos, getColorFromInteger(color1));
    showPixels();
    if(stop) {return;}
    //(largest_item - smallest_item) maps to (max-min)
    float ratio = (.5 - .05)/((120*.05) - .05);
    //(min + ratio*(value-smallest_item))
    float speedFactor = .05 + ratio * ((map(speed, 1, 120, 120, 1) * .05) - .05);
    pos += speedFactor;
    if (pos >= (SIDE*map(strlen(message), 1, 63, 4, SIDE))+(strlen(message))*8)
        pos = map(strlen(message), 1, 63, -(SIDE*.5), 0);
}


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

/* ============================ Squarral functions ============================ */
void squarral() {
    int posX, posY, posZ;
    int incX, incY, incZ;
    Color voxelColor;
    run = TRUE;
    
    if(frame + bound + axis == 0) {
        //We need to fade any voxels still lit
        for(int idx = 0; idx < PIXEL_CNT; idx++)
            if(strip.getPixelColor(idx) > 0) {
                transition(black, TRUE);
                return;
            }
    }
    
    add(position, increment);   //position += increment;
    if((increment.x==1)&&(position.x==SIDE-1-bound))
        increment={0,1,0};
    if((increment.x==-1)&&(position.x==bound))
        increment={0,-1,0};
    if((increment.y==1)&&(position.y==SIDE-1-bound))
        increment={-1,0,0};
    if((increment.y==-1)&&(position.y==bound)) {
        increment={1,0,0};
        position.z+=squarral_zInc;
        bound+=boundInc;
        if((position.z==3)&&(squarral_zInc>0))
          boundInc=0;
        if((position.z==4)&&(squarral_zInc>0))
          boundInc=-1;
        if((position.z==3)&&(squarral_zInc<0))
          boundInc=-1;
        if((position.z==4)&&(squarral_zInc<0))
          boundInc=0;
        if((position.z==0)||(position.z==SIDE-1))
            boundInc*=-1;
        if((position.z==SIDE-1)||(position.z==0)) {
            squarral_zInc*=-1;
            if(squarral_zInc==1) {
                axis=rand()%6;
                if(rand()%5==0)
                    rainbowColor=TRUE;
                else
                    rainbowColor=FALSE;
            }
        }
    }
    
    posX=position.x;
    posY=position.y;
    posZ=position.z;
    
    incX=increment.x;
    incY=increment.y;
    incZ=increment.z;
    
    for(int i=TRAIL_LENGTH-1;i>0;i--) {
        trailPoints[i].x=trailPoints[i-1].x;
        trailPoints[i].y=trailPoints[i-1].y;
        trailPoints[i].z=trailPoints[i-1].z;
        if(stop) {demo = FALSE; return;}
        if(demo) {if(millis() - lastModeSet > twoMinuteInterval) {return;}}
    }
    trailPoints[0].x=pixel.x;
    trailPoints[0].y=pixel.y;
    trailPoints[0].z=pixel.z;
    switch(axis) {
        case(0):
            pixel.x=position.x;
            pixel.y=position.y;
            pixel.z=position.z;
            break;
        case(1):
            pixel.x=position.z;
            pixel.y=position.x;
            pixel.z=position.y;
            break;
        case(2):
            pixel.x=position.y;
            pixel.y=position.z;
            pixel.z=position.x;
            break;
        case(3):
            pixel.x=position.z;
            pixel.y=SIDE-1-position.x;
            pixel.z=position.y;
            break;
        case(4):
            pixel.x=position.y;
            pixel.y=position.z;
            pixel.z=SIDE-1-position.x;
            break;
        case(5):
            pixel.x=position.x;
            pixel.y=SIDE-1-position.y;
            pixel.z=position.z;
            break;
    }
        
    voxelColor=getColorFromInteger(colorMap(frame%1000,0,1000));
    setPixelColor((int)pixel.x, (int)pixel.y, (int)pixel.z, voxelColor);
    for(int i=0;i<TRAIL_LENGTH;i++) {
        Color trailColor;
        if(rainbowColor) {
            trailColor=getColorFromInteger(colorMap((frame+(i*1000/TRAIL_LENGTH))%1000,0,1000));
            //fade the trail to black over the length of the trail
            trailColor.red=trailColor.red*(TRAIL_LENGTH-i)/TRAIL_LENGTH;
            trailColor.green=trailColor.green*(TRAIL_LENGTH-i)/TRAIL_LENGTH;
            trailColor.blue=trailColor.blue*(TRAIL_LENGTH-i)/TRAIL_LENGTH;
        }
        else {
            trailColor.red=voxelColor.red*(TRAIL_LENGTH-i)/TRAIL_LENGTH;
            trailColor.green=voxelColor.green*(TRAIL_LENGTH-i)/TRAIL_LENGTH;
            trailColor.blue=voxelColor.blue*(TRAIL_LENGTH-i)/TRAIL_LENGTH;
        }
        if(stop) {demo = FALSE; return;}
        if(demo) {if(millis() - lastModeSet > twoMinuteInterval) {return;}}
        setPixelColor((int)trailPoints[i].x, (int)trailPoints[i].y, (int)trailPoints[i].z, trailColor);
    }
    frame++;
    if(stop) {demo = FALSE; return;}
    if(demo) {if(millis() - lastModeSet > twoMinuteInterval) {return;}}
    showPixels();
    delay(speed * .5);
}

void add(Point& a, Point& b) {
    a.x+=b.x;
    a.y+=b.y;
    a.z+=b.z;
}

/* ============================= Plasma functions ============================= */
    void zPlasma() {
    float plasmaBrightness = constrain(map(brightness, 0, 255, 0, 100), 0, 100) * .01;
    Color plasmaColor;
    run = TRUE;
  
    //(largest_item - smallest_item) maps to (max-min)
    float ratio = (.18 - .003)/((120*.003) - .003);
    //(min + ratio*(value-smallest_item))
    phase += .003 + ratio * ((map(speed, 1, 120, 120, 1) * .003) - .003);
    
    // The two points move along Lissajious curves, see: http://en.wikipedia.org/wiki/Lissajous_curve
    // We want values that fit the LED grid: x values between 0..8, y values between 0..8, z values between 0...8
    // The sin() function returns values in the range of -1.0..1.0, so scale these to our desired ranges.
    // The phase value is multiplied by various constants; I chose these semi-randomly, to produce a nice motion.
    Point p1 = { (sin(phase*1.000)+1.0) * 4, (sin(phase*1.310)+1.0) * 4.0,  (sin(phase*1.380)+1.0) * 4.0};
    Point p2 = { (sin(phase*1.770)+1.0) * 4, (sin(phase*2.865)+1.0) * 4.0,  (sin(phase*1.410)+1.0) * 4.0};
    Point p3 = { (sin(phase*0.250)+1.0) * 4, (sin(phase*0.750)+1.0) * 4.0,  (sin(phase*0.380)+1.0) * 4.0};
    
    byte row, col, dep;
    
    // For each row
    for(row=0; row<SIDE; row++) {
        float row_f = float(row); // Optimization: Keep a floating point value of the row number, instead of recasting it repeatedly.
        
        // For each column
        for(col=0; col<SIDE; col++) {
            float col_f = float(col); // Optimization.
            
            // For each depth
            for(dep=0; dep<SIDE; dep++) {
                float dep_f = float(dep); // Optimization.
                
                // Calculate the distance between this LED, and p1.
                Point dist1 = { col_f - p1.x, row_f - p1.y,  dep_f - p1.z }; // The vector from p1 to this LED.
                float distance1 = sqrt( dist1.x*dist1.x + dist1.y*dist1.y + dist1.z*dist1.z);
                
                // Calculate the distance between this LED, and p2.
                Point dist2 = { col_f - p2.x, row_f - p2.y,  dep_f - p2.z}; // The vector from p2 to this LED.
                float distance2 = sqrt( dist2.x*dist2.x + dist2.y*dist2.y + dist2.z*dist2.z);
                
                // Calculate the distance between this LED, and p3.
                Point dist3 = { col_f - p3.x, row_f - p3.y,  dep_f - p3.z}; // The vector from p3 to this LED.
                float distance3 = sqrt( dist3.x*dist3.x + dist3.y*dist3.y + dist3.z*dist3.z);
                
                // Warp the distance with a sin() function. As the distance value increases, the LEDs will get light,dark,light,dark,etc...
                // You can use a cos() for slightly different shading, or experiment with other functions.
                float color_1 = distance1; // range: 0.0...1.0
                float color_2 = distance2;
                float color_3 = distance3;
                float color_4 = (sin( distance1 * distance2 * colorStretch )) + 2.0 * 0.5;
                // Square the color_f value to weight it towards 0. The image will be darker and have higher contrast.
                color_1 *= color_1 * color_4;
                color_2 *= color_2 * color_4;
                color_3 *= color_3 * color_4;
                color_4 *= color_4;
                // Scale the color up to 0..7 . Max brightness is 7.
                //strip.setPixelColor(col + (8 * row), strip.Color(color_4, 0, 0) );
                plasmaColor.red=color_1*plasmaBrightness;
                plasmaColor.green=color_2*plasmaBrightness;
                plasmaColor.blue=color_3*plasmaBrightness;
                
                if(stop) {demo = FALSE; return;}
                if(demo) {if(millis() - lastModeSet > twoMinuteInterval) {return;}}
                setPixelColor(row,col,dep,plasmaColor);       
            }
        }
    }
    if(stop) {demo = FALSE; return;}
    if(demo) {if(millis() - lastModeSet > twoMinuteInterval) {return;}}
    showPixels();
}
