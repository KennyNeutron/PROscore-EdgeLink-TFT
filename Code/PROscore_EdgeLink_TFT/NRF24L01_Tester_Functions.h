// NRF24L01_Tester_Functions.h
#ifndef NRF24L01_TESTER_FUNCTIONS_H
#define NRF24L01_TESTER_FUNCTIONS_H

#include <RF24.h>

// Function declarations for NRF24L01 Tester
void checkNRF24L01Hardware();
void update_nrf24l01_tester_realtime();

// External variables (defined in main file)
extern RF24 radio;
extern uint16_t CurrentScreenID;

// Screen object declarations
extern lv_obj_t* SCR_NRF24L01Tester;

// Create label helper function declaration
extern lv_obj_t* create_label(lv_obj_t* parent, const char* text, const lv_font_t* font, lv_color_t color);

// Close icon event handler declaration
extern void CloseIcon_Clicked(lv_event_t* e);

#endif  // NRF24L01_TESTER_FUNCTIONS_H