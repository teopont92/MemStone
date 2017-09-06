
#include <Wire.h>
#include <LSM303.h>
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

ADC_MODE(ADC_VCC); // necessaru for getting correct values from getVcc();

#define OLED_RESET LED_BUILTIN //4
Adafruit_SSD1306 display(OLED_RESET);

#define NUMFLAKES 10
#define XPOS 0
#define YPOS 1
#define DELTAY 2

#define LOGO16_GLCD_HEIGHT 16 
#define LOGO16_GLCD_WIDTH  16 

#define SECS_PER_MIN  (60UL)
#define SECS_PER_HOUR (3600UL)
#define SECS_PER_DAY  (SECS_PER_HOUR * 24L)

/* Useful Macros for getting elapsed time */
#define numberOfSeconds(_time_) (_time_ % SECS_PER_MIN)  
#define numberOfMinutes(_time_) ((_time_ / SECS_PER_MIN) % SECS_PER_MIN)
#define numberOfHours(_time_) (( _time_% SECS_PER_DAY) / SECS_PER_HOUR)
#define elapsedDays(_time_) ( _time_ / SECS_PER_DAY) 

static const unsigned char PROGMEM logo16_glcd_bmp[] =
{ B00000000, B11000000,
  B00000001, B11000000,
  B00000001, B11000000,
  B00000011, B11100000,
  B11110011, B11100000,
  B11111110, B11111000,
  B01111110, B11111111,
  B00110011, B10011111,
  B00011111, B11111100,
  B00001101, B01110000,
  B00011011, B10100000,
  B00111111, B11100000,
  B00111111, B11110000,
  B01111100, B11110000,
  B01110000, B01110000,
  B00000000, B00110000 };

   unsigned char trash_icon16x16[] =
{
    0b00000011, 0b11000000,
    0b00000111, 0b11100000,
    0b00000110, 0b01100000,
    0b11111100, 0b00111111,
    0b11111111, 0b11111111,
    0b01100000, 0b00000110,
    0b01100110, 0b01100110,
    0b01100110, 0b01100110,
    0b01100110, 0b01100110,
    0b01100110, 0b01100110,
    0b01100110, 0b01100110,
    0b01100110, 0b01100110,
    0b01100110, 0b01100110,
    0b01100000, 0b00000110,
    0b01111111, 0b11111110,
    0b00111111, 0b11111100,
                
};


   unsigned char lock_closed_icon16x16[] =
{
  0b00111111, 0b11111100, //   ############  
  0b00111111, 0b11111100, //   ############  
  0b00111000, 0b00011100, //   ###      ###  
  0b00111000, 0b00011100, //   ###      ###  
  0b00111000, 0b00011100, //   ###      ###  
  0b01111111, 0b11111110, //  ############## 
  0b01111111, 0b11111110, //  ############## 
  0b01111111, 0b11111110, //  ############## 
  0b01111111, 0b11111110, //  ############## 
  0b01111110, 0b01111110, //  ######  ###### 
  0b01111110, 0b01111110, //  ######  ###### 
  0b01111110, 0b01111110, //  ######  ###### 
  0b01111110, 0b01111110, //  ######  ###### 
  0b01111111, 0b11111110, //  ############## 
  0b01111111, 0b11111110, //  ############## 
  0b00000000, 0b00000000, //                 
};

 unsigned char lock_open_icon16x16[] =
{
  0b00111111, 0b11111100, //   ############  
  0b00111111, 0b11111100, //   ############  
  0b00111000, 0b00000000, //   ###        
  0b00111000, 0b00000000, //   ###           
  0b00111000, 0b00000000, //   ###           
  0b01111111, 0b11111110, //  ############## 
  0b01111111, 0b11111110, //  ############## 
  0b01111111, 0b11111110, //  ############## 
  0b01111111, 0b11111110, //  ############## 
  0b01111110, 0b01111110, //  ######  ###### 
  0b01111110, 0b01111110, //  ######  ###### 
  0b01111110, 0b01111110, //  ######  ###### 
  0b01111110, 0b01111110, //  ######  ###### 
  0b01111111, 0b11111110, //  ############## 
  0b01111111, 0b11111110, //  ############## 
  0b00000000, 0b00000000, //                 
};

 unsigned char bat0_icon16x16[] =
{
  0b00000000, 0b00000000, //                 
  0b00000000, 0b00000000, //                 
  0b00000000, 0b00000000, //                 
  0b00111111, 0b11111000, //   ###########   
  0b01111111, 0b11111100, //  #############  
  0b11111111, 0b11111110, // ############### 
  0b11100000, 0b00000110, // ###          ## 
  0b11100000, 0b00000111, // ###          ###
  0b11100000, 0b00000111, // ###          ###
  0b11100000, 0b00000110, // ###          ## 
  0b11111111, 0b11111110, // ############### 
  0b01111111, 0b11111100, //  #############  
  0b00111111, 0b11111000, //   ###########   
  0b00000000, 0b00000000, //                 
  0b00000000, 0b00000000, //                 
  0b00000000, 0b00000000, //                 
};

 unsigned char bat1_icon16x16[] =
{
  0b00000000, 0b00000000, //                 
  0b00000000, 0b00000000, //                 
  0b00000000, 0b00000000, //                 
  0b00111111, 0b11111000, //   ###########   
  0b01111111, 0b11111100, //  #############  
  0b11111111, 0b11111110, // ############### 
  0b11101100, 0b00000110, // ### ##       ## 
  0b11101100, 0b00000111, // ### ##       ###
  0b11101100, 0b00000111, // ### ##       ###
  0b11101100, 0b00000110, // ### ##       ## 
  0b11111111, 0b11111110, // ############### 
  0b01111111, 0b11111100, //  #############  
  0b00111111, 0b11111000, //   ###########   
  0b00000000, 0b00000000, //                 
  0b00000000, 0b00000000, //                 
  0b00000000, 0b00000000, //                 
};

 unsigned char bat2_icon16x16[] =
{
  0b00000000, 0b00000000, //                 
  0b00000000, 0b00000000, //                 
  0b00000000, 0b00000000, //                 
  0b00111111, 0b11111000, //   ###########   
  0b01111111, 0b11111100, //  #############  
  0b11111111, 0b11111110, // ############### 
  0b11101101, 0b10000110, // ### ## ##    ## 
  0b11101101, 0b10000111, // ### ## ##    ###
  0b11101101, 0b10000111, // ### ## ##    ###
  0b11101101, 0b10000110, // ### ## ##    ## 
  0b11111111, 0b11111110, // ############### 
  0b01111111, 0b11111100, //  #############  
  0b00111111, 0b11111000, //   ###########   
  0b00000000, 0b00000000, //                 
  0b00000000, 0b00000000, //                 
  0b00000000, 0b00000000, //                 
};

 unsigned char bat3_icon16x16[] =
{
  0b00000000, 0b00000000, //                 
  0b00000000, 0b00000000, //                 
  0b00000000, 0b00000000, //                 
  0b00111111, 0b11111000, //   ###########   
  0b01111111, 0b11111100, //  #############  
  0b11111111, 0b11111110, // ############### 
  0b11101101, 0b10110110, // ### ## ## ## ## 
  0b11101101, 0b10110111, // ### ## ## ## ###
  0b11101101, 0b10110111, // ### ## ## ## ###
  0b11101101, 0b10110110, // ### ## ## ## ## 
  0b11111111, 0b11111110, // ############### 
  0b01111111, 0b11111100, //  #############  
  0b00111111, 0b11111000, //   ###########   
  0b00000000, 0b00000000, //                 
  0b00000000, 0b00000000, //                 
  0b00000000, 0b00000000, //                 
};

 unsigned char bat4_icon16x16[] =
{
  0b00000000, 0b00000000, //                 
  0b00000000, 0b00000000, //                 
  0b00000000, 0b00000000, //                 
  0b00111111, 0b11111000, //   ###########   
  0b01111111, 0b11111100, //  #############  
  0b11111111, 0b11111110, // ############### 
  0b11100000, 0b00000110, // ###          ## 
  0b11100000, 0b01100111, // ###      ##  ###
  0b11100111, 0b11110111, // ###  ####### ###
  0b11100011, 0b00000110, // ###   ##     ## 
  0b11111111, 0b11111110, // ############### 
  0b01111111, 0b11111100, //  #############  
  0b00111111, 0b11111000, //   ###########   
  0b00000000, 0b00000000, //                 
  0b00000000, 0b00000000, //                 
  0b00000000, 0b00000000, //                 
};

 unsigned char wifi1_icon16x16[] =
{
  0b00000000, 0b00000000, //                 
  0b00000111, 0b11100000, //      ######     
  0b00011111, 0b11111000, //    ##########   
  0b00111111, 0b11111100, //   ############  
  0b01110000, 0b00001110, //  ###        ### 
  0b01100111, 0b11100110, //  ##  ######  ## 
  0b00001111, 0b11110000, //     ########    
  0b00011000, 0b00011000, //    ##      ##   
  0b00000011, 0b11000000, //       ####      
  0b00000111, 0b11100000, //      ######     
  0b00000100, 0b00100000, //      #    #     
  0b00000001, 0b10000000, //        ##       
  0b00000001, 0b10000000, //        ##       
  0b00000000, 0b00000000, //                 
  0b00000000, 0b00000000, //                 
  0b00000000, 0b00000000, //                 
};

 unsigned char nocon_icon16x16[] =
{
  0b00000000, 0b00000000, //                 
  0b00000011, 0b11100000, //       #####     
  0b00001111, 0b11111000, //     #########   
  0b00011111, 0b11111100, //    ###########  
  0b00111110, 0b00111110, //   #####   ##### 
  0b00111000, 0b01111110, //   ###    ###### 
  0b01110000, 0b11111111, //  ###    ########
  0b01110001, 0b11110111, //  ###   ##### ###
  0b01110011, 0b11000111, //  ###  ####   ###
  0b01110111, 0b10000111, //  ### ####    ###
  0b00111111, 0b00001110, //   ######    ### 
  0b00111110, 0b00011110, //   #####    #### 
  0b00011111, 0b11111100, //    ###########  
  0b00001111, 0b11111000, //     #########   
  0b00000011, 0b11100000, //       #####     
  0b00000000, 0b00000000, //                 
};
 unsigned char bullet_icon16x16[] =
{
  0b00000000, 0b00000000, //                 
  0b00000000, 0b00000000, //                 
  0b00000000, 0b00000000, //                 
  0b00000000, 0b00000000, //                 
  0b00000011, 0b10000000, //       ###       
  0b00001111, 0b11100000, //     #######     
  0b00001111, 0b11100000, //     #######     
  0b00011111, 0b11110000, //    #########    
  0b00011111, 0b11110000, //    #########    
  0b00011111, 0b11110000, //    #########    
  0b00001111, 0b11100000, //     #######     
  0b00001111, 0b11100000, //     #######     
  0b00000011, 0b10000000, //       ###       
  0b00000000, 0b00000000, //                 
  0b00000000, 0b00000000, //                 
  0b00000000, 0b00000000, //                 
};
 unsigned char share_icon16x16[] =
{
  0b00000000, 0b01000000, //                 
  0b00000000, 0b11100000, //                 
  0b00000001, 0b11110000, //                 
  0b00000000, 0b11100000, //       ###       
  0b00000111, 0b01000000, //     #######     
  0b00101110, 0b00000000, //     #######     
  0b01110000, 0b00000000, //    #########    
  0b11111000, 0b00000000, //    #########    
  0b01110000, 0b00000000, //    #########    
  0b00101110, 0b00000000, //     #######     
  0b00000111, 0b01000000, //     #######     
  0b00000000, 0b11100000, //       ###       
  0b00000001, 0b11110000, //                 
  0b00000000, 0b11100000, //                 
  0b00000000, 0b01000000, // 
  0b00000000, 0b00000000, //                
};

 unsigned char square_icon16x16[] =
{
  0b00000000, 0b00000000, //                 
  0b00000000, 0b00000000, //                 
  0b00000000, 0b00000000, //                 
  0b00111111, 0b11111000, //    ##########             
  0b00111111, 0b11111000, //    ##########       
  0b00111111, 0b11111000, //    ##########     
  0b00111111, 0b11111000, //    ##########     
  0b00111111, 0b11111000, //    ##########   
  0b00111111, 0b11111000, //    ##########    
  0b00111111, 0b11111000, //    ##########    
  0b00111111, 0b11111000, //    ##########     
  0b00111111, 0b11111000, //    ##########     
  0b00111111, 0b11111000, //    ##########       
  0b00000000, 0b00000000, //                 
  0b00000000, 0b00000000, //                 
  0b00000000, 0b00000000, //                 
};
 unsigned char bluetooth_icon16x16[] =
{
  0b00000000, 0b00000000, //                 
  0b00000001, 0b10000000, //        ##       
  0b00000001, 0b11000000, //        ###      
  0b00000001, 0b01100000, //        # ##     
  0b00001001, 0b00110000, //     #  #  ##    
  0b00001101, 0b00110000, //     ## #  ##    
  0b00000111, 0b01100000, //      ### ##     
  0b00000011, 0b11000000, //       ####      
  0b00000001, 0b10000000, //        ##       
  0b00000011, 0b11000000, //       ####      
  0b00000111, 0b01100000, //      ### ##     
  0b00001101, 0b00110000, //     ## #  ##    
  0b00001001, 0b00110000, //     #  #  ##    
  0b00000001, 0b01100000, //        # ##     
  0b00000001, 0b11000000, //        ###      
  0b00000001, 0b10000000, //        ##       
};
 unsigned char person_icon16x16[] =
{
  0b00000111, 0b11100000, //      ######     
  0b00001111, 0b11110000, //     ########    
  0b00001111, 0b11110000, //     ########    
  0b00011111, 0b11111000, //    ##########   
  0b00011111, 0b11111000, //    ##########   
  0b00011111, 0b11111000, //    ##########   
  0b00011111, 0b11111000, //    ##########   
  0b00001111, 0b11110000, //     ########    
  0b00001111, 0b11110000, //     ########    
  0b00000111, 0b11100000, //      ######     
  0b00000111, 0b11100000, //      ######     
  0b00111111, 0b11111100, //   ############  
  0b01111111, 0b11111110, //  ############## 
  0b11111111, 0b11111111, // ################
  0b11111111, 0b11111111, // ################
  0b11111111, 0b11111111, // ################
};

#if (SSD1306_LCDHEIGHT != 64)
#error("Height incorrect, please fix Adafruit_SSD1306.h!");
#endif

// Display
long display_freq=1000;
long display_prev_millis=0;
int usernameTimeAlternation=0;

// Notification
long notification_time=0;
int notification_duration=1500;
String notification_message="";

// ACC
LSM303 compass;
int total_acc=500;
float x[500];
float y[500];
float z[500];
float diff_x[500];
float diff_y[500];
float diff_z[500];
float sum = 0;
float lastSum = 0;
long acc_freq=1000/20;
long acc_prev_millis=0;

// Magnetometer
int magn_count=0;
int magn_total=5;
float mx[5];
float my[5];
float mz[5];

// Gestures
long gesture_freq=1000/20;
long gesture_prev_millis=0;

// Shake Gesture
int shake_phase=0;
int shake_count=0;
int shake_count_steps=7;
int shake_threshold_x=70000;
int shake_threshold_y=50000;
int shake_threshold_z=20000;

// TAP Gesture
int x_min=300;
int y_min=300;
int z_min=300;
int tap_x[500];
int tap_y[500];
int tap_z[500];

int pre_tap=10;

int tap1_min=1;
int tap1_max=8;
int tap1_count=0;

int between_tap_min=0;
int between_tap_max=10;
int between_tap_count=0;

int tap2_min=1;
int tap2_max=8;
int tap2_count=0;

int post_tap=20;
int post_tap_count=0;

int tap_phase=0;
// END TAP

// BUMP Gesture
int bump_x_threshold=1000;
int bump_y_threshold=1000;
int bump_z_threshold=400;

// Orientation Gesture
int orientation_state=0;
int orientation_temp_state=-1;
int orientation_change_count=0;
int orientation_change_max=30;

// Side Standing Gesture
int side_state=0;

// Recording Timer
long timer_rec_start=0;
long prev_rec_timer=0;
int j = 0;
int timer_seconds = 0;
int timer_minutes = 0;
int timer_hours = 0;

int i = 1;
int ledPin = 14;
int shakePin = 13;
int sharePin = 15;
int vibPin = 12;
bool flag = true;
String operation_status = "";
boolean sharing=false;
boolean locked = false;
boolean recording = false;
boolean configuring_sharing=false;
String username = "Matteo";
int peers = 02;

// battery related stuff
//battery empty - voltage output 1.7v-1.8v
//battery full  - voltage  output 2.63v-2.64v
int MIN_VCC = 1750;
int MAX_VCC = 2630;
int batteryPercent = 0;
int totalVccEntries = 30;
uint32_t batteryVcc[30];
int batteryIndex = 0;
//int greenPin = 10;

// VIBRATION PATTERNS
int ORIENTATION_2_VIB_PATTERN[4] = {150,150,150,150};
int ORIENTATION_2_VIB_PAUSE[4] = {80,80,80,80};

int ORIENTATION_1_VIB_PATTERN[4] = {250,250,100};
int ORIENTATION_1_VIB_PAUSE[4] = {120,250,0};

int ORIENTATION_0_VIB_PATTERN[4] = {100,100,400};
int ORIENTATION_0_VIB_PAUSE[4] = {80,150,0};

int NOTIFICATION_VIB_PATTERN[3] = {500, 500, 500};
int NOTIFICATION_VIB_PAUSE[3] = {100, 100, 0};

// DEBUG, PRINT OUTPUT
boolean PRINT_ORIENTATION=false;
boolean PRINT_TAPPING=false;
boolean PRINT_SHAKING=false;
boolean PRINT_BUMP=false;
boolean PRINT_ACC=false;

//volatile unsigned long next;
//
//void inline printTime(){
////  Serial.println(String(millis()));
//  storeValues(false);
//  next=next+(80000000L*0.05L);
//  timer0_write(next);
//}

void setup() {
  // initialize the serial communications:
  Serial.begin(9600);
  Wire.begin();
  compass.init();
  compass.enableDefault();
  pinMode(ledPin, OUTPUT);
  pinMode(shakePin, OUTPUT);
  pinMode(sharePin, OUTPUT);
  pinMode(vibPin, OUTPUT);
  // by default, we'll generate the high voltage from the 3.3v line internally! (neat!)
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);  // initialize with the I2C addr 0x3D (for the 128x64)
  // init done
  
  // Show image buffer on the display hardware.
  // Since the buffer is intialized with an Adafruit splashscreen
  // internally, this will display the splashscreen.
  display.display();
  delay(1000); 

  // Clear the buffer.
  display.clearDisplay();
  
  // draw mulitple circles
  testdrawcircle();
  display.display();
  delay(100);
  display.clearDisplay();

  // text display tests
  display.setTextSize(2);
  display.setTextColor(WHITE);
  display.setCursor(15,10);
  display.println("Hello by");
  display.setCursor(10,35);
  display.println("MemStone!");
  display.display();
  delay(1000);
  display.clearDisplay();
  vibration(500);
  
  batteryIndex = 0;

  // Trying to read ACC from an interrupt
//  noInterrupts();
//  timer0_isr_init();
//  timer0_attachInterrupt(printTime);
//  next=ESP.getCycleCount()+80000000L;
//  timer0_write(next);
//  interrupts();
}

void loop() {
  
  calculateBattery();
  int vcc = (int) ESP.getVcc();
  if (vcc < 1910){
    lowBatteryDisplay();
  }
  else {
    
    // read ACC data
    compass.read();

    // reset counter
    if (i>total_acc){
      i=0;
    }
    
    storeValues();
    checkGestures();

    // Handle Display Data
    refreshDisplay();
    i++;
    j++;
     
     delay(acc_freq);
 
  }
}

void storeValues(){

  long currentMillis=millis();

//  if (currentMillis-acc_prev_millis>=acc_freq){
  if (true){
    acc_prev_millis=currentMillis;

    if (magn_count > magn_total){
      magn_count=0;
    }
    mx[magn_count] = compass.m.x;
    my[magn_count] = compass.m.y;
    mz[magn_count] = compass.m.z;
    magn_count++;
    
    x[i] = (float) compass.a.x;
    y[i] = (float) compass.a.y;
    z[i] = (float) compass.a.z;
  
    int diff = 0;
    int j = 0;
    
    if (i>0){
      
      diff = x[i] - x[i-1];
      diff_x[i] = abs(diff);
      diff = y[i] - y[i-1];
      diff_y[i] = abs(diff);
      diff = z[i] - z[i-1];
      diff_z[i] = abs(diff);
  
      if (diff_x[i] > x_min){
        tap_x[i]=1;
      } else {
        tap_x[i]=0;
      }
  
      if (diff_y[i] > y_min){
        tap_y[i]=1;
      } else {
        tap_y[i]=0;
      }
  
      if (diff_z[i] > z_min){
        tap_z[i]=1;
      } else {
        tap_z[i]=0;
      }
    }
    
    
    if (PRINT_ACC){
      if (i > 0){
        Serial.print(x[i]);
        Serial.print("\t");
        Serial.print(y[i]);
        Serial.print("\t");
        Serial.print(z[i]);
        Serial.print("\t");
  
        Serial.print(diff_x[i]);
        Serial.print("\t");
        Serial.print(diff_y[i]);
        Serial.print("\t");
        Serial.print(diff_z[i]);
        Serial.print("\t");
        
        Serial.print(tap_x[i]);
        Serial.print("\t");
        Serial.print(tap_y[i]);
        Serial.print("\t");
        Serial.print(tap_z[i]);        
        
        Serial.println();
      }
    }
  }
}

void checkGestures(){
  
  long currentMillis = millis();
//  if (currentMillis-gesture_prev_millis>=gesture_freq){
  if (true){
    gesture_prev_millis=currentMillis;
    checkShake();
    handleOrientation();
    checkTap();
  }
}

// NOT USED
void checkBump(){
  
  if (shake_count==0){
    
    int bump=0;
    
    boolean bump_x=true;
    boolean bump_y=true;
    boolean bump_z=true;
    
    if (diff_x[i] >= bump_x_threshold){
      for (int l=(i-10); l<i; l++){
        if (diff_x[l] >= bump_x_threshold){
          bump_x=false;
          break;
        }
      }

      if (bump_x){
        bump++;
      }
    }
    
    if (diff_y[i] >= bump_y_threshold){
      for (int l=(i-10); l<i; l++){
        if (diff_y[l] >= bump_y_threshold){
          bump_y=false;
          break;
        }
      }

      if (bump_y){
        bump++;
      }
    }

    if (diff_z[i] >= bump_z_threshold){
      for (int l=(i-10); l<i; l++){
        if (diff_z[l] >= bump_z_threshold){
          bump_z=false;
          break;
        }
      }

      if (bump_z){
        bump++;
      }
    }

    if (bump >= 2){
      triggerShare();
    }

    if (PRINT_BUMP){
      Serial.println("Bump Count: " + String(bump));
    }
  }
}

void checkTap(){
  
  if (tap_phase==0){
    tap1_count=0;
    between_tap_count=0;
    tap2_count=0;
    post_tap_count=0;
    notification_message="";
    
    if ((tap_x[i] == 1) || (tap_y[i] == 1) || (tap_z[i] == 1)){
      int temp_sum_x = 0;
      int temp_sum_y = 0;
      int temp_sum_z = 0;
      for (int l=(i-pre_tap-1); l<i; l++){
        temp_sum_x=temp_sum_x + tap_x[l];
        temp_sum_y=temp_sum_y + tap_y[l];
        temp_sum_z=temp_sum_z + tap_z[l];
      }
      if ((temp_sum_x==0) || (temp_sum_y==0) || (temp_sum_z==0)){
        tap_phase=1;
      } else {
        tap_phase=0;
      }
    }
  } else if (tap_phase==1){
    if ((tap_x[i] == 1) || (tap_y[i] == 1) || (tap_z[i] == 1)){
      tap1_count=tap1_count+1;
      if (tap1_count>tap1_max){
        tap_phase=0;
      }
    } else {
      if (tap1_count>=tap1_min && tap1_count<=tap1_max){
        tap_phase=2;
      } else {
        tap_phase=0;
      }
    }
  } else if (tap_phase==2){
    if ((tap_x[i] == 0) || (tap_y[i] == 0) || (tap_z[i] == 0)){
      between_tap_count=between_tap_count+1;
      if (between_tap_count>between_tap_max){
        tap_phase=0;
      }
    } else {
      if (between_tap_count>=between_tap_min && between_tap_count<=between_tap_max){
        tap_phase=3;
      } else {
        tap_phase=0;
      }
    }
  } else if (tap_phase==3){
    if ((tap_x[i] == 1) || (tap_y[i] == 1) || (tap_z[i] == 1)){
      tap2_count=tap2_count+1;
      if (tap2_count>tap2_max){
        tap_phase=0;
      }
    } else {
      if (tap2_count>=tap2_min && tap2_count<=tap2_max){
        tap_phase=4;
      } else {
        tap_phase=0;
      }
    }    
  } else if (tap_phase==4){
    if ((tap_x[i] == 0) || (tap_y[i] == 0) || (tap_z[i] == 0)){
      post_tap_count=post_tap_count+1;
      if (post_tap_count>post_tap){
       
        if (recording && !sharing){
          triggerShare();   
        } else if (recording && sharing){
          triggerLock();
        }
       
        tap_phase=0;
      }
    } else {
      if (post_tap_count>=post_tap){
        
        if (recording && !sharing){
          triggerShare();   
        } else if (recording && sharing){
          triggerLock();
        }
        
        tap_phase=0;
      } else {
        tap_phase=0;
      }
    }    
  }
  if (PRINT_TAPPING){
    Serial.println("Phase: " + String(tap_phase));
  }
}

void triggerLock(){
  if (PRINT_TAPPING){
    Serial.println("Double TAP!!---Double TAP!!---Double TAP!!---Double TAP!!");
  }
  if (locked){
    locked=false;
    notification_message="Session \n Unlocked";
    displayNotification(notification_message, 2000, 8, 10, lock_open_icon16x16);
  } else {
    locked=true;
    notification_message="Session \n Locked";
    displayNotification(notification_message, 2000, 8, 10, lock_closed_icon16x16);
  }
  
}

void triggerShare(){
  if (PRINT_TAPPING){
    Serial.println("DOUBLE TAP FOR SHARING");
  }
  notification_message="Shared\n 3 min.";
  displayNotification(notification_message, 2000, 8, 10, share_icon16x16);
  
}

int checkOrientation(){
  int output;

  float sum_mx=0;
  float sum_my=0;
  float sum_mz=0;
  for (int l=0; l<magn_total; l++){
    sum_mx = sum_mx + (mx[l]/1000);
    sum_my = sum_my + (my[l]/1000);
    sum_mz = sum_mz + (mz[l]/1000);
  }
  sum_mx = sum_mx/magn_total;
  sum_my = sum_my/magn_total;
  sum_mz = sum_mz/magn_total;


  // face down
  if (sum_mz>=0.30){
    output=0;

    // on side
  } else if ((sum_mz>=-0.30) && (sum_mz<=0.30)){
    output=1;

    // face up
  } else if (sum_mz<=-0.30){
    output=2; 

    // unknown
  } else {
    output=-1;
  }

  if (PRINT_ORIENTATION){
    Serial.print(String(sum_mx) + "\t" + String(sum_my) + "\t" + String(sum_mz) + "\t" + String(output));
    Serial.println();
  }

  return output;
  
//  // face down
//  if ((sum_mx>=-0.04) && (sum_mx<=0.01) && (sum_mz >=0.02) && (sum_mz<=0.05)){
//    output=0;
//
//    // on side
//  } else if ((sum_mx>=-0.06) && (sum_mx<=-0.02) && (sum_mz >=-0.03) && (sum_mz<=0.03)){
//    output=1;
//
//    // face up
//  } else if ((sum_mx>=-0.02) && (sum_mx<=0.02) && (sum_mz >= -0.04)){
//    output=2; 
//
//    // unknown
//  } else {
//    output=-1;
//  }
  
//  // standing on side
//  if ((sum_mz>-0.5) && (sum_mz<0.5)){
//    output=1;
//  }
//  // face up
//  else if ((sum_mz>0.5) && (sum_mz<1.40)){
//     output=2;  
//  } 
//  // face down
//  else if ( (sum_mz<-0.5) && (sum_mz>-1.08) ){
//    output = 0;
//    
//  } else {
//    output= -1;
//  }
  
}

void handleOrientation(){

    int orientation = checkOrientation();  
    
    // if state is not changed then just keep LED's on
    if (orientation_state == orientation || orientation == -1 || shake_count != 0){
      if (orientation_state == 2){
        digitalWrite(ledPin,HIGH);
        digitalWrite(sharePin,HIGH);
      } else if (orientation == 1)  {
        digitalWrite(ledPin,HIGH);
        digitalWrite(sharePin,LOW);
      } else {
        digitalWrite(ledPin,LOW);
        digitalWrite(sharePin,LOW);
      }
      
    } else {
      if (orientation==orientation_temp_state){
        orientation_change_count++;
      } else {
        orientation_temp_state = orientation;
        orientation_change_count=0;
      }

      if (orientation_change_count>=orientation_change_max || orientation_temp_state == -1){
        orientation_temp_state = orientation;
        orientation_state = orientation;
        orientation_change_count=0;
        
        if (orientation == 2){
          recording=true;
          sharing=false;
          configuring_sharing=true;
          operation_status = "Looking\n for Peers";
          timer_rec_start=millis();
          Serial.println("RESTARTED");
          digitalWrite(ledPin,HIGH);
          digitalWrite(sharePin,HIGH);
        } else if (orientation == 1){
          recording=true;
          timer_rec_start=millis();
          sharing=false;
          operation_status = " Own Rec.";
          digitalWrite(ledPin,HIGH);
          digitalWrite(sharePin,LOW);
          vibrationPattern(ORIENTATION_1_VIB_PATTERN, ORIENTATION_1_VIB_PAUSE);
        }
        else {
          recording=false;
          sharing=false;
          operation_status = " No Rec.";
          j=0;
          timer_rec_start=0;
          digitalWrite(ledPin,LOW);
          digitalWrite(sharePin,LOW);
          vibrationPattern(ORIENTATION_0_VIB_PATTERN, ORIENTATION_0_VIB_PAUSE);
        }
      }
    }
}

void checkShake(){
  long sumX=0;
  long sumY=0;
  long sumZ=0;
  
  for (int l=(i-5); l<=i; l++){
      sumX=sumX+diff_x[l];
      sumY=sumY+diff_y[l];
      sumZ=sumZ+diff_z[l];  
  }

  if (sumX>=shake_threshold_x && sumY>=shake_threshold_y && sumZ>=shake_threshold_z){
    shake_count++;
  } else {
    shake_count=0;
  }

  if (shake_count >= shake_count_steps){
    shake_count=0;
    triggerShake();
  }
}

void triggerShake(){
  if (PRINT_SHAKING){
    Serial.println("SHAKED");
  }
  timer_rec_start=timer_rec_start+(30000);
  displayNotification("Deleted\n 30 sec.", 3000, 7, 10, trash_icon16x16);
}

void vibration(long duration){
  digitalWrite(vibPin,HIGH);
  delay(duration);
  digitalWrite(vibPin,LOW);
}

void vibrationPattern(int vibrate[], int pause[]){
  for (int l=0; l<sizeof(vibrate); l++){
    digitalWrite(vibPin,HIGH);
    delay(vibrate[l]);
    digitalWrite(vibPin,LOW);
    if (l<(sizeof(vibrate)-1)){
      delay(pause[l]);
    }
  }
}

void feedbackLight(){
  digitalWrite(shakePin,HIGH);
  delay(300);
  digitalWrite(shakePin,LOW);
  delay(300);
  digitalWrite(shakePin,HIGH);
  delay(300);
  digitalWrite(shakePin,LOW);
  delay(300);
}
void testdrawchar(void) {
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0,0);

  for (uint8_t i=0; i < 168; i++) {
    if (i == '\n') continue;
    display.write(i);
    if ((i > 0) && (i % 21 == 0))
      display.println();
  }    
  display.display();
  delay(1);
}

void calculateBattery(){

  batteryVcc[batteryIndex] = ESP.getVcc();
  int vcc = 0;
  for (int i=0; i<totalVccEntries; i++){
    vcc = vcc + batteryVcc[i];
  }
  vcc = vcc / totalVccEntries;

  batteryPercent = map(vcc, MIN_VCC, MAX_VCC, 0, 100);
  if (batteryPercent > 100){
    batteryPercent = 100;
  } else if (batteryPercent < -1){
    batteryPercent = 0;
  }
  
  batteryIndex++;
  if (batteryIndex > totalVccEntries){
    batteryIndex = 0;
  }
}

void lowBatteryDisplay(){
  display.drawBitmap(90, 0, bat1_icon16x16, 16, 16, 1);
  display.setTextSize(1.5);
  display.setCursor(110,5);
  display.println(batteryPercent);
  
  display.setCursor(40,5);
  display.println(String(ESP.getVcc() / 1000.0) + " V");
  
  display.setCursor(45,20);
  display.setTextSize(2);
  display.println("Low"); 
  display.setCursor(23,40);
  display.println("battery!");
  
  
  display.display();
  delay(500);
  display.clearDisplay();
  
}

String getRecElapsedTime(){
  long currentMillis = millis();   
  if (timer_rec_start>currentMillis){
    timer_rec_start=currentMillis;
  }
  timer_seconds=numberOfSeconds((currentMillis-timer_rec_start)/1000);
  timer_minutes=numberOfMinutes((currentMillis-timer_rec_start)/1000);
  timer_hours=numberOfHours((currentMillis-timer_rec_start)/1000);

  String time="";
  if (timer_hours<10){
    time=time+"0"+String(timer_hours);
  } else {
    time=time+String(timer_hours);
  }
  time=time+":";

  if (timer_minutes<10){
    time=time+"0"+String(timer_minutes);
  } else {
    time=time+String(timer_minutes);
  }
  time=time+":";

  if (timer_seconds<10){
    time=time+"0"+String(timer_seconds);
  } else {
    time=time+String(timer_seconds);
  }

  return time;
}

void displayNotification(String message, long duration, int row, int column, unsigned char icon[]){
  display.clearDisplay();
  display.setTextSize(2);
  display.setTextColor(WHITE);
  display.setCursor(row,column);
  display.println(message);
  display.drawBitmap(110, 7, icon, 16, 16, 1);
  display.display();
//  vibrationPattern(NOTIFICATION_VIB_PATTERN, NOTIFICATION_VIB_PAUSE);
  vibration(duration);
  notification_message="";
  notification_time=0;
}

void refreshDisplay(){
    
    long currentMillis=millis();
    
    if (currentMillis-display_prev_millis >= display_freq) {
    display_prev_millis=currentMillis;
    display.clearDisplay();
    
      // print operation status
      display.setTextSize(2);
      display.setTextColor(WHITE);
      display.setCursor(8,20);
      display.println(operation_status);
      
      // alternate between Username and Elapsed Time of Rec.
      if (!recording){
        if (!configuring_sharing){
          display.setCursor(30,40);
          display.println(username);
        }
      }
      else{
        if (recording){
          if (!configuring_sharing){
            if (usernameTimeAlternation<=3){
              display.setCursor(30,40);
              display.println(username);
            } else {
              String time=getRecElapsedTime();
              display.setCursor(20,40);
            display.println(time);
            }
          }
        }
      }
      usernameTimeAlternation++;
      if (usernameTimeAlternation > 13){
        usernameTimeAlternation=0;
      }
      
      // Print Battery Percentage number
      display.setTextSize(1.5);
      display.setCursor(110,5);
      if (batteryPercent == -1){
        display.println("NA");
      } else{
        display.println(batteryPercent);
      }
    
      // Show the right Battery icon
      if (batteryPercent <= 33){
        display.drawBitmap(90, 0, bat1_icon16x16, 16, 16, 1);
      } else if (batteryPercent > 33 && batteryPercent <= 75){
        display.drawBitmap(90, 0, bat2_icon16x16, 16, 16, 1);
      } else {
        display.drawBitmap(90, 0, bat3_icon16x16, 16, 16, 1);
      }
      // Show the Bluetooth icon
      display.drawBitmap(70, 0, bluetooth_icon16x16, 16, 16, 1);
    
      // Show Recording/Not Recording icon
      if (recording == true){
        display.drawBitmap(55, 0, bullet_icon16x16, 16, 16, 1);
      }
      else{
        display.drawBitmap(55, 0, square_icon16x16, 16, 16, 1);
      }
    
      // Show Sharing icon
      if (sharing){
          display.drawBitmap(0, 0, share_icon16x16, 16, 16, 1);
      }

      // Show Locked/Unlocked icon
      if (sharing){
        if (locked){
           display.drawBitmap(18, 0, lock_closed_icon16x16, 16, 16, 1);
        } else {
          display.drawBitmap(18, 0, lock_open_icon16x16, 16, 16, 1);
        }
      }
      
      if (recording && !sharing){
        display.drawBitmap(0, 0, person_icon16x16, 16, 16, 1);
      }
      if (!recording){
        display.drawBitmap(0, 0, nocon_icon16x16, 16, 16, 1);
      }

      display.display();

      if (configuring_sharing){
        for (int l=0; l<20; l++){
          if ((l%2)==0){
            digitalWrite(sharePin,LOW);
          } else {
            digitalWrite(sharePin,HIGH);
          }
          delay(200);
        }
        configuring_sharing=false;
        sharing=true;
        operation_status="Peers: 2";
       
        vibrationPattern(ORIENTATION_2_VIB_PATTERN, ORIENTATION_2_VIB_PAUSE);
        display.clearDisplay();
        display_prev_millis=0;
      }
    }
}

void testdrawcircle(void) {
  for (int16_t i=0; i<display.height(); i+=2) {
    display.drawCircle(display.width()/2, display.height()/2, i, WHITE);
    display.display();
    delay(1);
  }
}

void printMe (String me){
  Serial.println(me);
}

