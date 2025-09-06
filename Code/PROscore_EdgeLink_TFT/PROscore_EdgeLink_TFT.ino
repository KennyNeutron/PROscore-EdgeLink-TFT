/*******************************************************
 * Project: PROscore EdgeLink TFT
 * Board: ESP32-2432S028R (ESP32-WROOM, 2.8" TFT, ILI9341)
 *
 * Description:
 *   PROscore EdgeLink TFT is a handheld diagnostic and 
 *   monitoring tool for the PROscore scoreboard ecosystem. 
 *   It mirrors scoreboard data, inspects signal packets, 
 *   and provides real-time visualization on a TFT display 
 *   using LVGL.
 *
 * Board Configuration (Arduino IDE / PlatformIO):
 *   - CPU Frequency:        240 MHz (WiFi/BT enabled)
 *   - Events Run On:        Core 1
 *   - Arduino Runs On:      Core 1
 *   - Flash Frequency:      80 MHz
 *   - Flash Mode:           QIO
 *   - Flash Size:           4MB (32Mb)
 *   - Partition Scheme:     Default 4MB with spiffs 
 *                           (1.2MB APP / 1.5MB SPIFFS)
 *   - PSRAM:                Disabled
 *   - Upload Speed:         921600 baud
 *   - JTAG Adapter:         Disabled
 *   - Zigbee Mode:          Disabled
 *   - Core Debug Level:     None
 *
 * Notes:
 *   - TFT: ILI9341, SPI interface
 *   - Touch (optional): XPT2046, SPI interface
 *   - LVGL 9.x recommended for UI
 *   - Radio modules (NRF24L01 / ESP-NOW / BLE) supported
 *   - Power: Li-ion battery with USB-C charging
 *
 * Author: Kenny Neutron
 *******************************************************/
#include "EdgeLink_Variables.h"
#include "DataStructure_Payload.h"
#include <lvgl.h>
#include <TFT_eSPI.h>
#include <XPT2046_Touchscreen.h>
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
#include "DigitalIO.h"

// NRF24L01 pins (different from TFT)
#define NRF_CE 22
#define NRF_CSN 27

// Initialize RF24 with CE and CSN pins
RF24 radio(NRF_CE, NRF_CSN);

#define XPT2046_IRQ 36   // T_IRQ
#define XPT2046_MOSI 32  // T_DIN
#define XPT2046_MISO 39  // T_OUT
#define XPT2046_CLK 25   // T_CLK
#define XPT2046_CS 33    // T_CS

SPIClass touchscreenSPI = SPIClass(VSPI);
XPT2046_Touchscreen touchscreen(XPT2046_CS, XPT2046_IRQ);

#define SCREEN_WIDTH 240
#define SCREEN_HEIGHT 320

// Touchscreen coordinates: (x, y) and pressure (z)
int x, y, z;

#define DRAW_BUF_SIZE (SCREEN_WIDTH * SCREEN_HEIGHT / 6 * (LV_COLOR_DEPTH / 8))
uint32_t draw_buf[DRAW_BUF_SIZE / 4];

lv_obj_t* SCR_CurrentScreen;

// If logging is enabled, it will inform the user about what is happening in the library
void log_print(lv_log_level_t level, const char* buf) {
  LV_UNUSED(level);
  Serial.println(buf);
  Serial.flush();
}

// Get the Touchscreen data
void touchscreen_read(lv_indev_t* indev, lv_indev_data_t* data) {
  // Checks if Touchscreen was touched, and prints X, Y and Pressure (Z)
  if (touchscreen.tirqTouched() && touchscreen.touched()) {
    // Get Touchscreen points
    TS_Point p = touchscreen.getPoint();
    // Calibrate Touchscreen points with map function to the correct width and height
    x = map(p.x, 200, 3700, 1, SCREEN_WIDTH);
    y = map(p.y, 240, 3800, 1, SCREEN_HEIGHT);
    z = p.z;

    data->state = LV_INDEV_STATE_PRESSED;

    // Set the coordinates
    data->point.x = x;
    data->point.y = y;

    // Print Touchscreen info about X, Y and Pressure (Z) on the Serial Monitor
    /* Serial.print("X = ");
    Serial.print(x);
    Serial.print(" | Y = ");
    Serial.print(y);
    Serial.print(" | Pressure = ");
    Serial.print(z);
    Serial.println();*/
  } else {
    data->state = LV_INDEV_STATE_RELEASED;
  }
}

//LV Screens
static lv_obj_t* SCR_MainMenu;
static lv_obj_t* SCR_NRF24L01Tester;
static lv_obj_t* SCR_PROscoreRX;
static lv_obj_t* SCR_PROscoreRX_Settings;
static lv_obj_t* SCR_About;

//LV Variables
lv_obj_t* Icon_WIFI_Label = NULL;
lv_obj_t* Label_ShotClock = NULL;
lv_obj_t* Label_GameTime = NULL;
lv_obj_t* Label_HomeScore = NULL;
lv_obj_t* Label_GuestScore = NULL;
lv_obj_t* Label_HomeFoul = NULL;
lv_obj_t* Label_GuestFoul = NULL;
lv_obj_t* Label_HomeTOut = NULL;
lv_obj_t* Label_GuestTOut = NULL;
lv_obj_t* Label_Period = NULL;
lv_obj_t* Label_BallPoss_Left = NULL;
lv_obj_t* Label_BallPoss_Right = NULL;

//State tracking variables
bool last_NRF24L01_state = false;  // Track last state to detect changes

//ShotClock tracking variables
int last_ShotClock_Second = -1;  // Track last ShotClock value (-1 means uninitialized)
int last_ShotClock_Millis = -1;  // Track last ShotClock millis value (-1 means uninitialized)

//GameTime tracking variables
int last_GameTime_Minute = -1;  // Track last GameTime minute value (-1 means uninitialized)
int last_GameTime_Second = -1;  // Track last GameTime second value (-1 means uninitialized)
int last_GameTime_Millis = -1;  // Track last GameTime millis value (-1 means uninitialized)

//Score tracking variables
int last_HomeScore = -1;   // Track last HomeScore value (-1 means uninitialized)
int last_GuestScore = -1;  // Track last GuestScore value (-1 means uninitialized)

//Foul tracking variables
int last_HomeFoul = -1;   // Track last HomeFoul value (-1 means uninitialized)
int last_GuestFoul = -1;  // Track last GuestFoul value (-1 means uninitialized)

//Timeout tracking variables
int last_HomeTOut = -1;   // Track last HomeTOut value (-1 means uninitialized)
int last_GuestTOut = -1;  // Track last GuestTOut value (-1 means uninitialized)

//Period tracking variable
int last_Period = -1;     // Track last Period value (-1 means uninitialized)

//BallPoss tracking variables
int last_BallPoss = -1;  // Track last BallPoss value (-1 means uninitialized)

static void CloseIcon_Clicked(lv_event_t* e) {
  switch (CurrentScreenID) {
    case 0x1000:  //NRF24L01Tester
      lv_obj_del(SCR_CurrentScreen);
      Display_MainMenu_POST();
      Display_MainMenu();
      break;
    case 0x2000:  //PROscoreRX
      lv_obj_del(SCR_CurrentScreen);
      Display_MainMenu_POST();
      Display_MainMenu();
      break;
    case 0x2100:  //PROscoreRX Settings
      lv_obj_del(SCR_CurrentScreen);
      Display_PROscoreRX_POST();
      Display_PROscoreRX();
      break;
    default:
      CurrentScreenID = 0x0000;
      break;
  }
}


void setup() {
  String LVGL_Arduino = String("LVGL Library Version: ") + lv_version_major() + "." + lv_version_minor() + "." + lv_version_patch();
  Serial.begin(115200);
  Serial.println(LVGL_Arduino);

  // Start LVGL
  lv_init();
  // Register print function for debugging
  lv_log_register_print_cb(log_print);

  // Start the SPI for the touchscreen and init the touchscreen
  touchscreenSPI.begin(XPT2046_CLK, XPT2046_MISO, XPT2046_MOSI, XPT2046_CS);
  touchscreen.begin(touchscreenSPI);
  // Set the Touchscreen rotation in landscape mode
  // Note: in some displays, the touchscreen might be upside down, so you might need to set the rotation to 0: touchscreen.setRotation(0);
  touchscreen.setRotation(2);

  // Create a display object
  lv_display_t* disp;
  // Initialize the TFT display using the TFT_eSPI library
  disp = lv_tft_espi_create(SCREEN_WIDTH, SCREEN_HEIGHT, draw_buf, sizeof(draw_buf));
  lv_display_set_rotation(disp, LV_DISPLAY_ROTATION_270);

  // Initialize an LVGL input device object (Touchscreen)
  lv_indev_t* indev = lv_indev_create();
  lv_indev_set_type(indev, LV_INDEV_TYPE_POINTER);
  // Set the callback function to read Touchscreen input
  lv_indev_set_read_cb(indev, touchscreen_read);

  if (!radio.begin()) {
    Serial.println("NRF24L01 hardware not responding!");
  } else {
    Serial.println("NRF24L01 initialized on custom SPI");
    radio.printDetails();  // Optional: print radio config
  }

  radio.setPALevel(RF24_PA_MAX);
  radio.setDataRate(RF24_250KBPS);
  radio.openReadingPipe(0, address);
  radio.startListening();

  Display_MainMenu();
}

void loop() {
  // Check NRF24L01 status
  if (radio.available()) {
    Serial.println("Data received from NRF24L01");
    NRF24L01_DecodeData();
    NRF24L01_DataReceived = true;
  } else {
    Serial.println("No data received from NRF24L01");
    NRF24L01_DataReceived = false;
  }


  switch (CurrentScreenID) {
    case 0x2000: // PROscoreRX screen
      update_wifi_icon_realtime();
      update_shotclock_realtime();
      update_gametime_realtime();
      update_scores_realtime();
      update_fouls_realtime();
      update_timeouts_realtime();
      update_period_realtime();
      update_ballposs_realtime();
      break;
      
    case 0x1000: // NRF24L01 Tester screen
      update_nrf24l01_tester_realtime();
      break;
      
    default:
      // No specific updates needed for other screens
      break;
  }

  lv_task_handler();
  lv_tick_inc(5);
  delayMicroseconds(3);
}
