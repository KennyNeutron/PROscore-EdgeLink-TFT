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

#include <lvgl.h>
#include <TFT_eSPI.h>
#include <XPT2046_Touchscreen.h>

TFT_eSPI tft = TFT_eSPI(); // Create TFT instance

// Touchscreen pins
#define XPT2046_IRQ 36   // T_IRQ
#define XPT2046_MOSI 32  // T_DIN
#define XPT2046_MISO 39  // T_OUT
#define XPT2046_CLK 25   // T_CLK
#define XPT2046_CS 33    // T_CS

SPIClass touchscreenSPI = SPIClass(VSPI);
XPT2046_Touchscreen touchscreen(XPT2046_CS, XPT2046_IRQ);

#define SCREEN_WIDTH 240
#define SCREEN_HEIGHT 320

// Touchscreen coordinates
int x, y, z;

// Buffer for LVGL
#define DRAW_BUF_SIZE (SCREEN_WIDTH * SCREEN_HEIGHT / 4 * (LV_COLOR_DEPTH / 8))
uint32_t draw_buf[DRAW_BUF_SIZE / 4];

// UI objects
static lv_obj_t* slider_label;
static int button_click_count = 0;

// Display flush callback for LVGL 9.x
void my_disp_flush(lv_display_t* disp, const lv_area_t* area, uint8_t* px_map) {
  uint32_t w = (area->x2 - area->x1 + 1);
  uint32_t h = (area->y2 - area->y1 + 1);

  tft.startWrite();
  tft.setAddrWindow(area->x1, area->y1, w, h);
  tft.pushColors((uint16_t*)px_map, w * h, true);
  tft.endWrite();

  lv_display_flush_ready(disp);
}

// Touchscreen input callback
void touchscreen_read(lv_indev_t* indev, lv_indev_data_t* data) {
  if (touchscreen.tirqTouched() && touchscreen.touched()) {
    TS_Point p = touchscreen.getPoint();
    x = map(p.x, 200, 3700, 1, SCREEN_WIDTH);
    y = map(p.y, 240, 3800, 1, SCREEN_HEIGHT);
    z = p.z;

    data->state = LV_INDEV_STATE_PRESSED;
    data->point.x = x;
    data->point.y = y;
  } else {
    data->state = LV_INDEV_STATE_RELEASED;
  }
}

// Button click callback
static void button_event_cb(lv_event_t* e) {
  if (lv_event_get_code(e) == LV_EVENT_CLICKED) {
    button_click_count++;
    Serial.printf("Button clicked %d times\n", button_click_count);
    
    // Update button text to show click count
    lv_obj_t* btn = (lv_obj_t*)lv_event_get_target(e);
    lv_obj_t* label = lv_obj_get_child(btn, 0);
    lv_label_set_text_fmt(label, "Clicked %d", button_click_count);
  }
}

// Slider value changed callback
static void slider_event_cb(lv_event_t* e) {
  if (lv_event_get_code(e) == LV_EVENT_VALUE_CHANGED) {
    lv_obj_t* slider = (lv_obj_t*)lv_event_get_target(e);
    int32_t value = lv_slider_get_value(slider);
    lv_label_set_text_fmt(slider_label, "%d%%", (int)value);
    Serial.printf("Slider value: %d%%\n", (int)value);
  }
}

void create_hello_world_gui(void) {
  // Get active screen
  lv_obj_t* scr = lv_screen_active();
  
  // Apply dark theme
  lv_theme_t* theme = lv_theme_default_init(lv_display_get_default(), 
                                           lv_palette_main(LV_PALETTE_BLUE), 
                                           lv_palette_main(LV_PALETTE_RED), 
                                           true,  // Dark mode
                                           LV_FONT_DEFAULT);
  lv_display_set_theme(lv_display_get_default(), theme);

  // Create Hello World label
  lv_obj_t* hello_label = lv_label_create(scr);
  lv_label_set_text(hello_label, "Hello World!");
  lv_obj_set_style_text_font(hello_label, &lv_font_montserrat_14, 0);
  lv_obj_align(hello_label, LV_ALIGN_TOP_MID, 0, 30);

  // Create button
  lv_obj_t* btn = lv_button_create(scr);
  lv_obj_set_size(btn, 120, 50);
  lv_obj_align(btn, LV_ALIGN_CENTER, 0, -20);
  lv_obj_add_event_cb(btn, button_event_cb, LV_EVENT_CLICKED, NULL);

  // Button label
  lv_obj_t* btn_label = lv_label_create(btn);
  lv_label_set_text(btn_label, "Click Me!");
  lv_obj_center(btn_label);

  // Create slider
  lv_obj_t* slider = lv_slider_create(scr);
  lv_obj_set_width(slider, 180);
  lv_obj_align(slider, LV_ALIGN_CENTER, 0, 40);
  lv_slider_set_value(slider, 50, LV_ANIM_OFF);  // Initial value 50%
  lv_obj_add_event_cb(slider, slider_event_cb, LV_EVENT_VALUE_CHANGED, NULL);

  // Slider value label
  slider_label = lv_label_create(scr);
  lv_label_set_text(slider_label, "50%");
  lv_obj_align_to(slider_label, slider, LV_ALIGN_OUT_BOTTOM_MID, 0, 10);

  Serial.println("Hello World GUI created successfully!");
}

void setup() {
  Serial.begin(115200);
  Serial.printf("LVGL Version: %d.%d.%d\n", lv_version_major(), lv_version_minor(), lv_version_patch());

  // Initialize TFT
  tft.init();
  tft.setRotation(1); // Landscape mode

  // Initialize LVGL
  lv_init();

  // Initialize touchscreen
  touchscreenSPI.begin(XPT2046_CLK, XPT2046_MISO, XPT2046_MOSI, XPT2046_CS);
  touchscreen.begin(touchscreenSPI);
  touchscreen.setRotation(2);

  // Create display
  lv_display_t* disp = lv_display_create(SCREEN_WIDTH, SCREEN_HEIGHT);
  lv_display_set_flush_cb(disp, my_disp_flush);
  lv_display_set_buffers(disp, draw_buf, NULL, sizeof(draw_buf), LV_DISPLAY_RENDER_MODE_PARTIAL);
  lv_display_set_rotation(disp, LV_DISPLAY_ROTATION_90);

  // Create input device
  lv_indev_t* indev = lv_indev_create();
  lv_indev_set_type(indev, LV_INDEV_TYPE_POINTER);
  lv_indev_set_read_cb(indev, touchscreen_read);

  // Small delay for stability
  delay(100);

  // Create the GUI
  create_hello_world_gui();

  Serial.println("Setup complete!");
}

void loop() {
  lv_timer_handler();   // v9
  delay(5);
}