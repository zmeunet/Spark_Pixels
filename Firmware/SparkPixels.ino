/**
 ******************************************************************************
 * @extended SparkPixels.ino:
 *		New mode: AUDIO SPECTRUM
 *		Fixed modes: CHASER, POLICE LIGHTS CHASER, POLICE LIGHTS WIPE
 * @author   Werner Moecke
 * @version  V1.0.5
 * @date     08-October-2015 ~ 10-October-2015
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
const int STROBE                      = 9;
const int RAINBOW                     = 10;
const int THEATERCHASE                = 11;
const int FROZEN                      = 12;
const int COLLIDE                     = 13;
const int COLORFADE                   = 14;
const int RAINBOW_BURST               = 15;
const int FLICKER                     = 16;
const int COLORBREATHE                = 17;
const int POLICELIGHTS                = 18;
const int POLICELIGHTSWIPE            = 19;
const int POLICELIGHTSONE             = 20;
const int LISTENER                    = 21;
const int ZONECHASER                  = 22;
const int SPECTRUM                    = 23;

typedef struct modeParams {
   int 	 modeId;
   char  modeName[64];
   int   numOfColors;       //Tell the android app home many colors to expect. Max number is 6
} modeParams;

/**   An RGB color. */
typedef struct Color {
  unsigned char red, green, blue;

  Color(int r, int g, int b) : red(r), green(g), blue(b) {}
  Color() : red(0), green(0), blue(0) {}
} Color;

/** Overloaded != operator. */
bool operator!= (const Color& a, const Color& b);

/** Overloaded == operator. */
bool operator== (const Color& a, const Color& b);

/** Common colors */
const Color black     = Color(0x00, 0x00, 0x00);
const Color grey      = Color(0x92, 0x95, 0x91);
const Color yellow    = Color(0xff, 0xff, 0x14);
const Color magenta   = Color(0xc2, 0x00, 0x78);
const Color orange    = Color(0xf9, 0x73, 0x06);
const Color teal      = Color(0x02, 0x93, 0x86);
const Color red       = Color(0xe5, 0x00, 0x00);
const Color brown     = Color(0x65, 0x37, 0x00);
const Color pink      = Color(0xff, 0x81, 0xc0);
const Color blue      = Color(0x03, 0x43, 0xdf);
const Color green     = Color(0x15, 0xb0, 0x1a);
const Color purple    = Color(0x7e, 0x1e, 0x9c);
const Color white     = Color(0xff, 0xff, 0xff);

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
        {  COLLIDE,                     "COLLIDE",              0   },
        {  COLORBREATHE,                "COLOR BREATHE",        0   },
        {  COLORPULSE,                  "COLOR PULSE",          0   },
        {  COLORSTRIPES,                "COLOR STRIPES",        0   },
        {  COLORFADE,                   "COLOR TRANSITION",     0   },
        {  COLORWIPE,                   "COLOR WIPE",           0   },
        {  CHASER,                      "CHASER",               1   },
        {  COLORALL,                    "COLOR ALL",            1   },
        {  FLICKER,                     "FLICKER",              1   },
        {  STROBE,                      "STROBE",               1   },
        {  FROZEN,                      "FROZEN",               0   },
        {  LISTENER,                    "LISTENER",             0   },
        {  POLICELIGHTS,                "POLICE LIGHTS",        0   },
        {  POLICELIGHTSONE,             "POLICE LIGHTS CHASER", 0   },
        {  POLICELIGHTSWIPE,            "POLICE LIGHTS WIPE",   0   },
        {  RAINBOW,                     "RAINBOW",              0   },
        {  RAINBOW_BURST,               "RAINBOW BURST",        0   },
        {  SPECTRUM,                    "AUDIO SPECTRUM",	    0   },
        {  THEATERCHASE,                "THEATER CHASE",	    0   },
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
unsigned long oneHourInterval =      1*60*60*1000;  //auto off in 1 hr when night time
unsigned long oneDayInterval = 		24*60*60*1000;  //time sync interval - 24 hours

//Program Flags
int currentModeID;	//This is the ID of the current mode selected - used in the case statement
int run;    	//Use this for modes that don't need to loop. Set the color, then stop sending commands to the pixels
int stop;   	//Use this to break out of sequence loops when changing to a new mode

//Misc variables
int speed;	//not to be confused with speedIndex below, this is the local speed (delay) value
unsigned long previousMillis = 0;
unsigned long lastCommandReceived = 0;
unsigned long lastSync = 0;
uint32_t defaultColor;	//The NORMAL mode color
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
int wifi = 0;   					//used for general info and setup
int tHour=0;    					//used for general info and setup
int speedIndex;     				//Let the cloud know what speed preset we are using
int brightness;                     //How bright do we want these things anyway
//double measuredTemperature = 0.0;   //Let's see how hot our project box is getting since is has the power supply in there
char modeList[622] = "None";		//Holds all mode info comma delimited. Use this to populate the android app
char currentModeName[64] = "None";  //Holds current selected mode
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
int zone4End   = PIXEL_CNT;             //512

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
#define PIXELS_PER_PANEL        (strip.numPixels() / NR_OF_PANELS)
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
#define SMOOTH                  ON  // Switch for 'smoothening-out' the LEDs fade-to-black on the spectrum
#define DOTMODE                 ON  // Switch to control how the peaks will be displayed over each strip (bar/dot)
bool smooth, dotMode;
float real[ARRAY_SIZE];             //[(int)pow(2,M)]
float imaginary[ARRAY_SIZE];        //[(int)pow(2,M)]
float maxVal=8;

/* ======================= Required Prototypes =============================== */
int showPixels(void);
int antipodal_index(int i);
int setNewMode(int newMode);
int getModeIndexFromID(int id);
int isValidMode(String newMode);
int getModeIndexFromName(String name);
//int getTemperature(void);
void initMicrophone(void);
void fadeInToColor(uint32_t index, Color col);
void fadeOutFromColor(uint32_t index, Color col);
short FFT(short int dir,int m,float *x,float *y);
uint32_t Wheel(byte WheelPos);
Color getPixelColor(uint32_t index);
Color getColorFromInteger(uint32_t col);
uint32_t getHighestValFromRGB(Color col);
uint32_t colorMap(float val, float minVal, float maxVal);
uint32_t lerpColor(uint32_t c1, uint32_t c2, uint32_t val, uint32_t minVal, uint32_t maxVal);

/* ======================= Spark Pixel Prototypes =============================== */
int colorAll(uint32_t c);
int colorWipe(uint32_t c);
int colorZone(uint32_t c1, uint32_t c2, uint32_t c3, uint32_t c4);
void FFTJoy(void);
void frozen(void);
void listen(void);
void collide(void);
void rainbow(void);
void cycleLerp(void);
void cycleWipe(void);
void color_fade(void);
void colorPulse(void);
void modeButton(void);
void smoothSwitch(void);
void colorStripes(void);
void random_burst(void);
void rainbowCycle(void);
void flicker(uint32_t c);
void police_lightsALL(void);
void police_lightsONE(void);
void strobe(uint32_t color);
void colorChaser(uint32_t c);
void pulse_oneColorAll(void);
void police_light_strobo(void);
void theaterChaseRainbow(void);
void findRandomSnowFlakesPositions(int numFlakes);
void colorZoneChaser(uint32_t c1, uint32_t c2, uint32_t c3, uint32_t c4);

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
    brightness = 64; //250;
    run = FALSE;
    stop = FALSE;
	setNewMode(getModeIndexFromID(NORMAL));
	defaultColor = strip.Color((255*.25),(255*.25),(60*.25)); //This seems close to incandescent color
	snowFlakeColor = getColorFromInteger(0xFFFFFF);
	lastSync = lastCommandReceived = previousMillis = millis();     //Take a time stamp
    
    // seed the random number generator.  THINGS WILL NEVER BE THE SAME AGAIN
    uint32_t seed = millis(); 
    srand(seed); 
    c1 = Wheel(random(2, 256));
    c2 = Wheel(random(2, 256));

	strip.begin();     			//Start up the Neopixels     	
    colorAll(defaultColor);     //Turn on the NORMAL Mode
    //We are already connected: SYSTEM_MODE is no longer set to SEMI_AUTOMATIC
    //Particle.connect();			//Now connect to the cloud
    Time.zone(TIME_ZONE_OFFSET);//Set time zone
    
    //Clear the mode list variable
	sprintf(modeList,"");
    //Assemble Spark Cloud available modes variable
    for(uint16_t i=0;i<sizeof modeStruct / sizeof modeStruct[0];i++) {
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

    if(run)
    {
		stop = FALSE;
        switch (currentModeID) {
            case STANDBY:
    		    colorAll(strip.Color(0,0,0));
    		    break;
         	case COLORALL:
    	        colorAll(color1);
    	        break;
         	case COLORBREATHE:
    	        pulse_oneColorAll();
    	        break;
         	case CHASER:
    		    colorChaser(color1);
    			break;
    		case ZONE:
    	        colorZone(color1, color2, color3, color4); 
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
    		case STROBE:
    		    strobe(color1);
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
    		case POLICELIGHTSWIPE:
    		    police_lightsALL();
    		    break;
    		case POLICELIGHTSONE:
    		    police_lightsONE();
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
		        colorAll(defaultColor);
    			break;
        }
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
            //If it's Monday through Friday between 8am and 4pm or between 10pm and 5am any day, turn Off the lights
            if(((Time.weekday() >= 2 && Time.weekday() <=6) && (Time.hour() >= 8 && Time.hour() <= 16)) || (Time.hour() >= 22) || (Time.hour() <= 5)) {
                //No one is home or everyone is sleeping. So shut it down
				//sprintf(debug,"Last auto Off time = %i,", (int)(currentMillis - lastCommandReceived));
				lastCommandReceived = currentMillis;
				wifi = WiFi.RSSI();
				setNewMode(getModeIndexFromID(STANDBY));
                run = TRUE;
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

//Used in all modes to set the brightness, show the lights, process Spark events and delay
int showPixels(void) {
	strip.setBrightness(brightness);
    strip.show();
    Particle.process();    //process Spark events
	return 1;
}

// Set all pixels in the strip to a solid color
int colorAll(uint32_t c) {
    run = FALSE;
    
    if(c > 0) {
        Color c2, c1 = getColorFromInteger(c);
        uint32_t maxColorPixel = getHighestValFromRGB(c1);
        
        for(int i=0; i<=maxColorPixel; i+=(maxColorPixel*.05)) {
            if(i <= c1.red) c2.red = i;
            if(i <= c1.green) c2.green = i;
            if(i <= c1.blue) c2.blue = i;
            for(int j=0; j<strip.numPixels(); j++)
                strip.setPixelColor(j, strip.Color(c2.red, c2.green, c2.blue));
            showPixels();
            if(stop == TRUE) {return 0;}
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
                showPixels();
                if(stop == TRUE) {return 0;}
            }
            //Ensure every pixel has been fully blanked
            for(int i=0; i<strip.numPixels(); i++)
                strip.setPixelColor(i, 0);
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
        showPixels();
        if(stop == TRUE) {return;}
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

int colorZone(uint32_t c1, uint32_t c2, uint32_t c3, uint32_t c4) {
    uint16_t i, j;
    uint32_t maxColorPixel, increment;
    Color col1 = getColorFromInteger(c1);
    Color col2 = getColorFromInteger(c2);
    Color col3 = getColorFromInteger(c3);
    Color col4 = getColorFromInteger(c4);
    Color c;
    run = FALSE;
    
    maxColorPixel = getHighestValFromRGB(col1);
    increment = map(speed, 1, 120, maxColorPixel*.25, 5);
    for(j=0; j<=maxColorPixel; j+=increment) {
        if(j <= col1.red) c.red = j;
        if(j <= col1.green) c.green = j;
        if(j <= col1.blue) c.blue = j;
        for(i=zone1Start; i<=zone1End; i++)
            strip.setPixelColor(i, strip.Color(c.red, c.green, c.blue));
        showPixels();
        if(stop == TRUE) {return 0;}
        delay(speed);
    }
    maxColorPixel = getHighestValFromRGB(col2);
    increment = map(speed, 1, 120, maxColorPixel*.25, 5);
    for(j=0; j<=maxColorPixel; j+=increment) {
        if(j <= col2.red) c.red = j;
        if(j <= col2.green) c.green = j;
        if(j <= col2.blue) c.blue = j;
        for(i=zone2Start; i<=zone2End; i++)
            strip.setPixelColor(i, strip.Color(c.red, c.green, c.blue));
        showPixels();
        if(stop == TRUE) {return 0;}
        delay(speed);
    }
    maxColorPixel = getHighestValFromRGB(col3);
    increment = map(speed, 1, 120, maxColorPixel*.25, 5);
    for(j=0; j<=maxColorPixel; j+=increment) {
        if(j <= col3.red) c.red = j;
        if(j <= col3.green) c.green = j;
        if(j <= col3.blue) c.blue = j;
        for(i=zone3Start; i<=zone3End; i++)
            strip.setPixelColor(i, strip.Color(c.red, c.green, c.blue));
        showPixels();
        if(stop == TRUE) {return 0;}
        delay(speed);
    }
    maxColorPixel = getHighestValFromRGB(col4);
    increment = map(speed, 1, 120, maxColorPixel*.25, 5);
    for(j=0; j<=maxColorPixel; j+=increment) {
        if(j <= col4.red) c.red = j;
        if(j <= col4.green) c.green = j;
        if(j <= col4.blue) c.blue = j;
        for(i=zone4Start; i<=zone4End; i++)
            strip.setPixelColor(i, strip.Color(c.red, c.green, c.blue));
        showPixels();
        if(stop == TRUE) {return 0;}
        delay(speed);
    }
    return 1;
}

//Creates 4 color zones, with each one having its own "chaser pixel"
void colorZoneChaser(uint32_t c1, uint32_t c2, uint32_t c3, uint32_t c4) {
    static int idexZone1 = random(zone1Start, zone1End+1);
    static int idexZone2 = random(zone2Start, zone2End+1);
    static int idexZone3 = random(zone3Start, zone3End+1);
    static int idexZone4 = random(zone4Start, zone4End);
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
        showPixels();
        if(stop == TRUE) {return;}
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
   if (ihue >= 255) {
      ihue = 0;
   }
   for(int idex = 0 ; idex < PIXEL_CNT; idex++ ) {
      strip.setPixelColor(idex, Wheel(ihue));
   }
   showPixels();
   if(stop == TRUE) {return;}
   delay(speed);
}

//Random burst - Randowm colors on each LED
void random_burst() { //-RANDOM INDEX/COLOR
    static int idex = 0;
    static int ihue = 0;
	run = TRUE;

    idex = random(0,PIXEL_CNT-1);
    ihue = random(0,255);
    
    Color c1 = getColorFromInteger(Wheel(ihue));
    uint32_t c2 = strip.getPixelColor(idex);
    if(c2 > 0)
        fadeOutFromColor(idex, getColorFromInteger(c2));
    fadeInToColor(idex, c1); //strip.setPixelColor(idex, Wheel(ihue));
    if(stop == TRUE) {return;}
    //showPixels();
    delay(speed);
}

//Flicker effect - random flashing of all LEDs
void flicker(uint32_t c) {
    Color c2, c1 = getColorFromInteger(c);
    int ibright = random((brightness*.25), brightness+1);
    int random_delay = random(1, map(speed, 1, 120, 100, 10)+1);
    uint32_t maxColorPixel = floor(max(ibright, getHighestValFromRGB(c1)));
    uint32_t increment = map(speed, 1, 120, maxColorPixel*.25, 5);
	run = TRUE;
    
    for(int j=random(maxColorPixel*.25, (maxColorPixel*.5)+1); j<=maxColorPixel; j+=increment) {
        for(int i = 0; i < PIXEL_CNT; i++ ) {
            if(j <= c1.red) c2.red = j;
            if(j <= c1.green) c2.green = j;
            if(j <= c1.blue) c2.blue = j;
            strip.setPixelColor(i, strip.Color(c2.red, c2.green, c2.blue));
        }
        strip.show();
        if(stop == TRUE) {return;}
        Particle.process();    //process Spark events
        delay(random_delay);
    }
}

//Fade in/out a color using brightness/saturation
void pulse_oneColorAll() { //-PULSE BRIGHTNESS ON ALL LEDS TO ONE COLOR 
    static int ival = 0;
    static int xhue = Wheel(random(0, 256));
    static int bouncedirection = 0;
    int isteps = constrain(brightness*.03, .6, brightness*.25);
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
            xhue = Wheel(random(0, 256));
        }
    }
    for(int i = 0; i < PIXEL_CNT; i++) {
        strip.setPixelColor(i, xhue);
        strip.setBrightness(ival);
    }
  
    strip.show();
    if(stop == TRUE) {return;}
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
        if(stop == TRUE) {return;}
    }
    
    showPixels();
    if(stop == TRUE) {return;}
    delay(speed);
    left_right++;
}

//Police Light all LEDs 
void police_lightsALL() { //-POLICE LIGHTS (TWO COLOR SOLID)
    static int idex = 0;
    int idexR = idex;
    int idexB = antipodal_index(idexR);
    uint32_t increment = map(speed, 1, 120, 0xFF*.25, 5);
    Color colR, red = {0xFF, 0, 0};
    Color colB, blue = {0, 0, 0xFF};
	run = TRUE;
    
    for(int i=0; i<=0xFF; i+=increment) {
        if(i <= red.red) colR.red = i;
        if(i <= red.green) colR.green = i;
        if(i <= red.blue) colR.blue = i;
        if(i <= blue.red) colB.red = i;
        if(i <= blue.green) colB.green = i;
        if(i <= blue.blue) colB.blue = i;
        strip.setPixelColor(idexR, strip.Color(colR.red, colR.green, colR.blue));
        strip.setPixelColor(idexB, strip.Color(colB.red, colB.green, colB.blue));
        showPixels();
        if(stop == TRUE) {return;}
        delay(speed);
    }

    idex++;
    if (idex >= PIXEL_CNT)
        idex = 0;
}

//Police Light one LED blue and red
void police_lightsONE() { //-POLICE LIGHTS (TWO COLOR SINGLE LED)
    static int idexR = random(zone1Start, zone2End+1);  //idex;
    static int idexB = random(zone3Start, zone4End);    //antipodal_index(idexR);
    static bool bounceR = false;
    static bool bounceB = false;
    uint32_t increment = map(speed, 1, 120, 0xFF*.25, 5);
    Color colR, red = {0xFF, 0, 0};
    Color colB, blue = {0, 0, 0xFF};
	run = TRUE;
    
    for(int i=0; i<=0xFF; i+=increment) {
        //Slowly fade in the currently indexed pixels to blue and red
        if(i <= red.red) colR.red = i;
        if(i <= red.green) colR.green = i;
        if(i <= red.blue) colR.blue = i;
        if(i <= blue.red) colB.red = i;
        if(i <= blue.green) colB.green = i;
        if(i <= blue.blue) colB.blue = i;
        strip.setPixelColor(idexR, strip.Color(colR.red, colR.green, colR.blue));
        strip.setPixelColor(idexB, strip.Color(colB.red, colB.green, colB.blue));
        
        //Then slowly fade out previously-lit pixels to black, leaving a nice "trailing" effect
        for(int j=0; j<PIXEL_CNT; j++) {
            if ((j != idexR) && (j != idexB)) {
                Color pixelColor = getColorFromInteger(strip.getPixelColor(j));
                if(pixelColor.red > 0) pixelColor.red-=pixelColor.red*.125;
                if(pixelColor.green > 0) pixelColor.green-=pixelColor.green*.125;
                if(pixelColor.blue > 0) pixelColor.blue-=pixelColor.blue*.125;
                strip.setPixelColor(j, strip.Color(pixelColor.red, pixelColor.green, pixelColor.blue));
            }
        }
        showPixels();
        if(stop == TRUE) {return;}
        delay(speed);
    }
    
    //Check direction
    if(bounceR) {idexR--;} else {idexR++;}
    if(bounceB) {idexB--;} else {idexB++;}
    
    //Check beginning-of-trail
    if (idexR <= zone1Start) {
        idexR = zone1Start;
        bounceR = false;
    }
    if (idexB <= zone3Start) {
        idexB = zone3Start;
        bounceB = false;
    }
    
    //Check end-of-trail
    if (idexR >= zone2End) {
        idexR = zone2End;
        bounceR = true;
    }
    if (idexB >= zone4End-1) {
        idexB = zone4End-1;
        bounceB = true;
    }
}

//Pulse all dots with pseudo-random colors
void colorPulse() {
    uint16_t i, j;
    uint32_t pulseRate;
	run = TRUE;

    for(j=0; j<=c2; j++) {
        for(i=0; i<strip.numPixels(); i++) {
            pulseRate = abs(255-(speed+j));
            strip.setPixelColor(i, Wheel(lerpColor(j, speed, pulseRate, 0, 255)));
        }
        showPixels();
        if(stop == TRUE || pulseRate > 3840) {return;}  //Too fast - bail out.
		delay(speed);
    }
}

//Fill the dots with patterns from colors interpolated between 2 random colors
void cycleLerp() {
    uint16_t i, j;
    uint32_t rate;
	run = TRUE;

    for(j=0; j<=c2; j++) {
        for(i=0; i<strip.numPixels(); i++) {
            rate = abs(i-(speed+j));
            strip.setPixelColor(i, Wheel(lerpColor(j, c1, rate, 0, 255)));
        }
        showPixels();
        if(stop == TRUE || rate > 7680) {return;}   //Gettin' weird - bail out.
		delay(speed);
    }
}

//Randomly fill the dots with colors interpolated between the 2 chosen colors
void colorStripes() {
    uint16_t i, j;
	run = TRUE;

    for(j=0; j<=c2; j++) {
        for(i=0; i<strip.numPixels(); i++)
            strip.setPixelColor(i, lerpColor(j, c1, abs(i-j), 0, 255));
        showPixels();
        if(stop == TRUE) {return;}
        delay(speed);
    }
}

//Fill the dots one after the other with a color, wait (ms) after each one
int colorWipe(uint32_t c) {
    uint16_t i;
    Color c2 = getColorFromInteger(c);
    run = FALSE;
  
    for(i=0; i<strip.numPixels(); i++) {
        uint32_t c3 = strip.getPixelColor(i);
        if(c3 > 0)
            fadeOutFromColor(i, getColorFromInteger(c3));
        fadeInToColor(i, c2);   //strip.setPixelColor(i, c);
        showPixels();
        if(stop == TRUE) {return 0;}
		delay(speed);
    }
    return 1;
}

//use colorWipe to cycle through all the colors 
void cycleWipe(void) {
    uint16_t j;
    int retVal=1;

    for(j=0; j<256; j++) {
        if(j%2==0)
            retVal = colorWipe(Wheel(j & 0xFF));
        else
            retVal = colorWipe(Wheel((j+80) & 0xFF));
        if(stop == TRUE || retVal == 0) {return;}
		delay(speed);
    }
    //colorWipe turns off the run loop, so let's turn it back on
    run = TRUE; 
}

void strobe(uint32_t color) {
    int retVal=1;
    
    retVal = colorAll(color);               //Turn On all Pixels
    if(stop == TRUE || retVal == 0) {return;}
	delay(speed);
    
    retVal = colorAll(strip.Color(0,0,0));  //Turn Off all Pixels
    if(stop == TRUE || retVal == 0) {return;}
	delay(speed);
   
    //colorAll turns off the run loop, so let's turn it back on
    run = TRUE;
}

void rainbow(void) {
    uint16_t i, j;
	run = TRUE;
    
    for(j=0; j<256; j++) {
        for(i=0; i<strip.numPixels(); i++)
            strip.setPixelColor(i, Wheel((i+j) & 255));
        showPixels();
        if(stop == TRUE) {return;}
        delay(speed);
    }
}

// Slightly different, this makes the rainbow equally distributed throughout, then wait (ms)
void rainbowCycle(void) {
  uint16_t i, j;
  run = TRUE;

    for(j=0; j<256; j++) { // 1 cycle of all colors on wheel
        for(i=0; i< strip.numPixels(); i++)
            strip.setPixelColor(i, Wheel(((i * 256 / strip.numPixels()) + j) & 255));
        showPixels();
        if(stop == TRUE) {return;}
        delay(speed);
    }
}

//Theatre-style crawling lights with rainbow effect
void theaterChaseRainbow(void) {
	run = TRUE;

    for (uint16_t j=0; j < 256; j++) {                          //cycle all 256 colors in the wheel
        for (uint16_t q=0; q < 3; q++) {
            for (int i=0; i < strip.numPixels(); i=i+3)
                strip.setPixelColor(i+q, Wheel( (i+j) % 255));  //turn every third pixel on
            showPixels();
            if(stop == TRUE) {return;}
        	delay(speed);
            for (uint16_t i=0; i < strip.numPixels(); i=i+3)
                strip.setPixelColor(i+q, 0);                    //turn every third pixel off
        }
    }
}

//Fade from teal to blue to purple then back to blue then teal, repeat
//Random snowflakes twinkle white for random amounts of time 
//The snowflakes will hang around no faster than MIN_DELAY and no slower than MAX_DELAY
void frozen(void) {
    uint16_t i, j, k;
    uint16_t startColor = 140;  //Hue for teal
    uint16_t stopColor  = 210;  //Hue for purple
    const int MIN_DELAY = 400;  //in ms
    const int MAX_DELAY = 700;  //in ms
    const int MIN_FLAKES = strip.numPixels()*0.01;	//  1% of total number of pixels
    const int MAX_FLAKES = strip.numPixels()*0.1;	// 10% of total number of pixels
    int numSnowFlakes = random(MIN_FLAKES, MAX_FLAKES);	//How many flakes should we have at a time
    unsigned long previousMillis = millis();
    unsigned long flakeLifeSpan = random(MIN_DELAY, MAX_DELAY); //How long will a snowflake last
    int increment = constrain(map(speed, 1, 120, 4, 8), 4, 8);  //How fast/slow a snowflake will dim
	run = TRUE;
    
    //forwards
    for(j=startColor; j<stopColor; j++) { 	//cycle through the colors
        for(i=0; i<strip.numPixels(); i++) {
            strip.setPixelColor(i, Wheel(j));	//first set all pixels to the same color
            if(millis() - previousMillis > flakeLifeSpan) {
        		previousMillis = millis();						//reset time interval
        		flakeLifeSpan = random(MIN_DELAY, MAX_DELAY);	//set new lifespan	
        		numSnowFlakes = random(MIN_FLAKES, MAX_FLAKES); //set new number of flakes
                findRandomSnowFlakesPositions(numSnowFlakes);	//get the list of flake positions
                //avoid our snowflakes fading out by brightening them up every now and then
                if(strip.Color(snowFlakeColor.red, snowFlakeColor.green, snowFlakeColor.blue) < 0xFFFFFF) {
                    if(snowFlakeColor.red < 0xFF) snowFlakeColor.red+=(0xFF-(snowFlakeColor.red))*.5;
                    if(snowFlakeColor.green < 0xFF) snowFlakeColor.green+=(0xFF-(snowFlakeColor.green))*.5;
                    if(snowFlakeColor.blue < 0xFF) snowFlakeColor.blue+=(0xFF-(snowFlakeColor.blue))*.5;
                    for(k=0; k<numSnowFlakes; k++)
                        strip.setPixelColor(randomFlakes[k], strip.Color(snowFlakeColor.red, snowFlakeColor.green, snowFlakeColor.blue)); //Set snowflake
                    showPixels();
                    if(stop == TRUE) {return;}
                }
                increment = constrain(map(speed, 1, 120, 4, 8), 4, 8);  //refresh dim rate
            }
        }
        if(snowFlakeColor.red > 0) snowFlakeColor.red-=increment;
        if(snowFlakeColor.green > 0) snowFlakeColor.green-=increment;
        if(snowFlakeColor.blue > 0) snowFlakeColor.blue-=increment;
        for(k=0; k<numSnowFlakes; k++)
            strip.setPixelColor(randomFlakes[k], strip.Color(snowFlakeColor.red, snowFlakeColor.green, snowFlakeColor.blue)); //Set snowflake
        showPixels();
        if(stop == TRUE) {return;}
        delay(speed);
        increment = constrain(map(speed, 1, 120, 4, 8), 4, 8);  //refresh dim rate
    }
    
    //backwards
    for(j=stopColor; j>startColor; j--) { 	//cycle through the colors
        for(i=0; i< strip.numPixels(); i++) {
            strip.setPixelColor(i, Wheel(j));	//first set all pixels to the same color
            if(millis() - previousMillis > flakeLifeSpan) {
        		previousMillis = millis();						//reset time interval
        		flakeLifeSpan = random(MIN_DELAY, MAX_DELAY);	//set new lifespan	
        		numSnowFlakes = random(MIN_FLAKES, MAX_FLAKES); //set new number of flakes
                findRandomSnowFlakesPositions(numSnowFlakes);	//get the list of flake positions
                //avoid our snowflakes fading out by brightening them up every now and then
                if(strip.Color(snowFlakeColor.red, snowFlakeColor.green, snowFlakeColor.blue) < 0xFFFFFF) {
                    if(snowFlakeColor.red < 0xFF) snowFlakeColor.red+=(0xFF-(snowFlakeColor.red))*.5;
                    if(snowFlakeColor.green < 0xFF) snowFlakeColor.green+=(0xFF-(snowFlakeColor.green))*.5;
                    if(snowFlakeColor.blue < 0xFF) snowFlakeColor.blue+=(0xFF-(snowFlakeColor.blue))*.5;
                    for(k=0; k<numSnowFlakes; k++)
                        strip.setPixelColor(randomFlakes[k], strip.Color(snowFlakeColor.red, snowFlakeColor.green, snowFlakeColor.blue)); //Set snowflake
                    showPixels();
                    if(stop == TRUE) {return;}
                }
                increment = constrain(map(speed, 1, 120, 4, 8), 4, 8);  //refresh dim rate
                if(strip.Color(snowFlakeColor.red, snowFlakeColor.green, snowFlakeColor.blue) <= 0) snowFlakeColor = getColorFromInteger(0xFFFFFF);
            }
        }
        if(snowFlakeColor.red > 0) snowFlakeColor.red-=increment;
        if(snowFlakeColor.green > 0) snowFlakeColor.green-=increment;
        if(snowFlakeColor.blue > 0) snowFlakeColor.blue-=increment;
        for(k=0; k<numSnowFlakes; k++)
            strip.setPixelColor(randomFlakes[k], strip.Color(snowFlakeColor.red, snowFlakeColor.green, snowFlakeColor.blue)); //Set snowflake
        showPixels();
        if(stop == TRUE) {return;}
        delay(speed);
        increment = constrain(map(speed, 1, 120, 4, 8), 4, 8);  //refresh dim rate
    }
}

void findRandomSnowFlakesPositions(int numFlakes) {
    for(int i=0;i<numFlakes;i++) {
        randomFlakes[i] = random(strip.numPixels());
        if(i > 0) {
            for(int j=0;j<i;j++) {
				//make sure we didn't already set that position
                if(randomFlakes[i] == randomFlakes[j]){
                    do {
                        randomFlakes[i] = random(strip.numPixels());
                    }while(randomFlakes[i] != randomFlakes[j]);
                }
            }
        }
    }
}

//Color Blender
//Red starts at 1 end and green at the other. They increment towards each other. 
//When they meet, they eplode into the combined color
void collide(void) {
	uint16_t i, j, color1, color2;
	//uint16_t startColor = 0;
	run = TRUE;
	
	for(color1=0; color1<=255; color1+=85) {
	//for(color1=0; color1<=255; color1+=170) {
	    for(color2=color1+42; color2<=color1+85; color2+=43) {
	    //for(color2=color1+85; color2<=color1+170; color2+=85) {
    		for(i=0; i<=strip.numPixels()/2; i++) { 
    			if(i*2 >= strip.numPixels()) {
    				//Explode the colors
    				for(j=0; j<strip.numPixels(); j++) {
    					strip.setPixelColor(j, Wheel((((color2-color1)/2)+color1)&0xFF));
    					if(j%5==0)
    					    showPixels();
    				}
    				showPixels();
    				if(stop == TRUE) {return;}
					delay(speed);
    				break;
    			}
    			else {
    				//Grow the two colors from either end of the strip
    				strip.setPixelColor(i, Wheel(color1));
    				strip.setPixelColor(strip.numPixels()-i, Wheel((color2)&0xFF));
    				showPixels();
    				if(stop == TRUE) {return;}
					delay(speed);
    			}
    		}
	    }
	}
}

// Input a value 0 to 255 to get a color value.
// The colours are a transition r - g - b - back to r.
uint32_t Wheel(byte WheelPos) {
  if(WheelPos < 85) {
   return strip.Color(WheelPos * 3, 255 - WheelPos * 3, 0);
  } else if(WheelPos < 170) {
   WheelPos -= 85;
   return strip.Color(255 - WheelPos * 3, 0, WheelPos * 3);
  } else {
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
Color getPixelColor(uint32_t index) {
    uint32_t col = strip.getPixelColor(index);
    return getColorFromInteger(col);
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
    uint32_t increment = map(speed, 1, 120, maxColorPixel*.25, 5);
    Color col2;
    
    for(int i=0; i<=maxColorPixel; i+=increment) {
        if(i <= col.red) col2.red = i;
        if(i <= col.green) col2.green = i;
        if(i <= col.blue) col2.blue = i;
        strip.setPixelColor(index, strip.Color(col2.red, col2.green, col2.blue));
        showPixels();
        if(stop == TRUE) {return;}
    }
}

/** Fade out a pixel color from the given Color to 0
  @param index The strip index of the pixel.
  @param col The Color to fade out from.*/
void fadeOutFromColor(uint32_t index, Color col) {
    uint32_t maxColorPixel = getHighestValFromRGB(col);
    uint32_t increment = map(speed, 1, 120, maxColorPixel*.25, 5);
    Color col2;
    
    for(int i=maxColorPixel; i>=0; i-=increment) {
        if(i <= col.red) col2.red = i;
        if(i <= col.green) col2.green = i;
        if(i <= col.blue) col2.blue = i;
        strip.setPixelColor(index, strip.Color(col2.red, col2.green, col2.blue));
        showPixels();
        if(stop == TRUE) {return;}
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
    bool isNewMode = false;
	bool isNewSpeed = false;
	bool isNewBrightness = false;
    
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
			isNewMode = true;
		}
		else if(command.charAt(beginIdx) == 'S') {
		    int receivedSpeedValue = command.substring(beginIdx+2, idx).toInt();
		    if(receivedSpeedValue > (int)(sizeof(speedPresets)/sizeof(int)))
		        receivedSpeedValue = sizeof(speedPresets)/sizeof(int) - 1;
		    if (speedIndex != receivedSpeedValue) {
		        //we don't update the speed when currently in LISTENER mode
				if(currentModeID != LISTENER) isNewSpeed = true;
			}
			//we don't update the speed when currently in LISTENER mode
			if(currentModeID != LISTENER) {
    			speedIndex = receivedSpeedValue;
    			speed = speedPresets[speedIndex];
			}
		}
		else if(command.charAt(beginIdx) == 'B') {
		    int newBrightness = command.substring(beginIdx+2, idx).toInt() * 255 / 100;	//Scale 0-100 to 0-255
			if(brightness != newBrightness) {
			    //we don't update the brightness when currently in LISTENER mode
				if(currentModeID != LISTENER) isNewBrightness = true;
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
    if(isNewMode==true) {
        run = TRUE;
	    stop = TRUE;    
    }
    else {
		//I guess we're updating only the speed or brightness, so let's update the Pixels
		//we don't update the speed or brightness when currently in LISTENER mode
		if(currentModeID != LISTENER) showPixels();
		if(isNewBrightness==true) {
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
	return newModeIndex;
}

int getModeIndexFromName(String name)
{
    for(int i=0;i<(int)(sizeof modeStruct / sizeof modeStruct[0]);i++) {
        if(name.equals(String(modeStruct[i].modeName))) {
            return i;
        }
    }
    return -1;
}

int getModeIndexFromID(int id)
{
    for(int i=0;i<(int)(sizeof modeStruct / sizeof modeStruct[0]);i++) {
        if(id == modeStruct[i].modeId) {
            return i;
        }
    }
    return -1;
}

void listen() {
    run = TRUE;
    
    // Checks for the presence of a UDP packet, and reports the buffer size
    int32_t bytes = Udp.parsePacket();
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
            if((data[1] == TPM2NET_CMD_COMMAND) || (stop == TRUE)) {
                strip.setBrightness(brightness);
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
                uint16_t index = packetNumber * PIXELS_PER_PANEL;
                uint16_t voxelIdx = TPM2NET_HEADER_SIZE;

                // Start drawing!!
                for(int z = NR_OF_PANELS-1; z >= 0; z--) {                  // We're only dealing in 2 dimensions (width & height)
                    for(int col = 0; col < NR_OF_PANELS; col++) {           // Linewise, starting from leftmost index
                        for(int row = NR_OF_PANELS - 1; row >= 0; row--) {  // Columnwise, starting from topmost index
                            Color pixelColor = Color(data[voxelIdx], data[voxelIdx + 1], data[voxelIdx + 2]);  // Take 3 color bytes from buffer
                            int idx = (row*NR_OF_PANELS*NR_OF_PANELS) + (z*NR_OF_PANELS) + col;
                            strip.setPixelColor(idx, strip.Color(pixelColor.red, pixelColor.green, pixelColor.blue));
                        }
                        voxelIdx+=3; // Increment buffer index by 3 bytes
                    }
                }
                // Display!!
                if(packetNumber == totalPackets) {
                    strip.setBrightness(255);
                    strip.show();
                    Particle.process();    //process Spark events
                    if(stop == TRUE) {
                        strip.setBrightness(brightness);
                        return;
                    }
                }
            }
        }
    }
}

/* ============ AUDIO SPECTRUM mode functions ============ */
void FFTJoy() {
    run = TRUE;
    
    for(int i=0;i<pow(2,M);i++) {
        real[i]=analogRead(MICROPHONE)-SAMPLES;
        delayMicroseconds(120);  /* 210 this sets our 'sample rate'.  I went through a bunch of trial and error to 
                                  * find a good sample rate to put a soprano's vocal range in the spectrum of the cube
    							  *
      							  * 120 gets the entire range from bass to soprano within the cube's spectrum
      							  * *From bass to soprano: https://youtu.be/6jqCuE7C3rg */
        imaginary[i]=0;
        if(stop == TRUE) {return;}
    }
    
    FFT(1, M, real, imaginary);
    
    for(int i=0;i<pow(2,M);i++) {
        imaginary[i]=sqrt(pow(imaginary[i],2)+pow(real[i],2));
        if(imaginary[i]>maxVal)
            maxVal=imaginary[i];
        if(stop == TRUE) {return;}
    }

    for(int i=0;i<pow(2,M)/2;i++) {
        imaginary[i]=SIDE*imaginary[i]/maxVal;
        Color pixelColor;
        int y, pixIdx, pixUppIdx, pixLowIdx;
    	for(y=0;y<=imaginary[i];y++) {
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
                        //This causes a nice and smooth 'trailing' effect from the base of
                      	//the cube towards the peak; gotta fade the pixels faster, so that 
                      	//the human eye can spot the 'trailing' effect in 'dot' mode
                        if(pixelColor.red > 0) pixelColor.red-=pixelColor.red*(map(pixIdx%SIDE, 0, SIDE-1, SIDE-2, 1)*.1);
                        if(pixelColor.green > 0) pixelColor.green-=pixelColor.green*(map(pixIdx%SIDE, 0, SIDE-1, SIDE-2, 1)*.1);
                        if(pixelColor.blue > 0) pixelColor.blue-=pixelColor.blue*(map(pixIdx%SIDE, 0, SIDE-1, SIDE-2, 1)*.1);
                    }
        			strip.setPixelColor(j, strip.Color(pixelColor.red, pixelColor.green, pixelColor.blue));
                    if(stop == TRUE) {return;}
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
                if(pixelColor.red > 0) pixelColor.red-=pixelColor.red*(map(pixIdx%SIDE, 0, SIDE-1, 1, SIDE-2)*.06);
                if(pixelColor.green > 0) pixelColor.green-=pixelColor.green*(map(pixIdx%SIDE, 0, SIDE-1, 1, SIDE-2)*.06);
                if(pixelColor.blue > 0) pixelColor.blue-=pixelColor.blue*(map(pixIdx%SIDE, 0, SIDE-1, 1, SIDE-2)*.06);
            }
            strip.setPixelColor(j, strip.Color(pixelColor.red, pixelColor.green, pixelColor.blue));
            if(stop == TRUE) {return;}
        }
    }
    
    //Fade the 'trail' to black over the length of the cube's z-axis
    for(int z=0;z<SIDE-1;z++)
        for(int x=0;x<SIDE;x++)
            for(int y=0;y<SIDE;y++) {
                int pixIdx=((z+1)*SIDE*SIDE) + (x*SIDE) + y;
                Color trailColor=getPixelColor(pixIdx);
                if(smooth) {
                  	//This is responsible for the 'meteors' shooting towards the back of 
                  	//the cube; otherwise it would look like they were 'going backwards'
                    if(trailColor.red > 0) trailColor.red-=trailColor.red*.125;
                    if(trailColor.green > 0) trailColor.green-=trailColor.green*.125;
                    if(trailColor.blue > 0) trailColor.blue-=trailColor.blue*.125;
                }
                strip.setPixelColor((z*SIDE*SIDE) + (x*SIDE) + y, strip.Color(trailColor.red, trailColor.green, trailColor.blue));
                if(stop == TRUE) {return;}
                delayMicroseconds(speed);  //introducing a little bit of delay to 'smoothen-out' transitions
            }
    
    maxVal= (maxVal>=120) ? maxVal-2 : (maxVal<8) ? 8 : maxVal-.8;
    showPixels();
    if(stop == TRUE) {return;}
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
