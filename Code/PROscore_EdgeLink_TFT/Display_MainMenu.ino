// Global variables to hold buttons and status label
lv_obj_t* buttons[6];
lv_obj_t* status_label;

void on_button_click(lv_event_t* e) {
  // Get button index from user data
  int btn_index = (int)(intptr_t)lv_event_get_user_data(e);

  const char* button_names[] = {
    "NRF24L01 Tester", "PROscore RX", "HC-05 Tester",
    "PROscore TX", "Settings", "About"
  };

  // Update status label
  char status_text[64];
  snprintf(status_text, sizeof(status_text), "Button '%s' pressed", button_names[btn_index]);
  lv_label_set_text(status_label, status_text);

  // Log the button press
  LV_LOG_USER("Button '%s' (index %d) clicked", button_names[btn_index], btn_index);

  // Add your specific button handling logic here
  switch (btn_index) {
    case 0:  // NRF24L01 Tester
      // Handle NRF24L01 Tester screen
      break;
    case 1:  // PROscore RX
      // Handle PROscore RX screen
      CurrentScreen = 0x2000; // Change to PROscore RX screen
      break;
    case 2:  // HC-05 Tester
      // Handle HC-05 Tester screen
      break;
    case 3:  // PROscore TX
      // Handle PROscore TX screen
      break;
    case 4:  // Settings
      // Handle Settings screen
      break;
    case 5:  // About
      // Handle about screen
      break;
  }
}

bool Display_MainMenu_Init = false;
void Display_MainMenu() {
  if (!Display_MainMenu_Init) {


    lv_obj_t* scr = lv_screen_active();

    // Display dimensions in landscape: 320x240
    const int DISPLAY_WIDTH = 320;
    const int DISPLAY_HEIGHT = 240;

    // Button dimensions and spacing
    const int BTN_WIDTH = 120;  // Button width
    const int BTN_HEIGHT = 60;  // Button height
    const int MARGIN_X = 20;    // Left/right margin
    const int MARGIN_Y = 20;    // Top/bottom margin
    const int SPACING_X = 30;   // Horizontal spacing between buttons
    const int SPACING_Y = 10;   // Vertical spacing between buttons

    // Calculate starting positions
    int start_x = MARGIN_X;
    int start_y = MARGIN_Y;

    // Create 6 buttons in 3 rows x 2 columns
    const char* button_labels[] = {
      "NRF24L01 Tester", "PROscore RX",
      "HC-05 Tester", "PROscore TX",
      "Settings", "About"
    };

    for (int row = 0; row < 3; row++) {
      for (int col = 0; col < 2; col++) {
        int btn_index = row * 2 + col;

        // Calculate button position
        int x = start_x + col * (BTN_WIDTH + SPACING_X);
        int y = start_y + row * (BTN_HEIGHT + SPACING_Y);

        // Create button
        buttons[btn_index] = lv_button_create(scr);
        lv_obj_set_size(buttons[btn_index], BTN_WIDTH, BTN_HEIGHT);
        lv_obj_set_pos(buttons[btn_index], x, y);

        // Add event callback with button index as user data
        lv_obj_add_event_cb(buttons[btn_index], on_button_click,
                            LV_EVENT_CLICKED, (void*)(intptr_t)btn_index);

        // Create label for button
        lv_obj_t* label = lv_label_create(buttons[btn_index]);
        lv_label_set_text(label, button_labels[btn_index]);
        lv_obj_center(label);
      }
    }

    // Status label at the bottom
    status_label = lv_label_create(scr);
    lv_label_set_text(status_label, "PROscore EdgeLink TFT - Ready");
    lv_obj_align(status_label, LV_ALIGN_BOTTOM_MID, 0, -5);
    // Set smaller font for status
    lv_obj_set_style_text_font(status_label, &lv_font_montserrat_12, 0);

    CurrentScreen_Label = lv_label_create(scr);

    //PRINTING SCREEN ID  
    char hex_buffer[8];  
    snprintf(hex_buffer, sizeof(hex_buffer), "0x%04X", CurrentScreen);
    lv_label_set_text(CurrentScreen_Label, hex_buffer);
    lv_obj_align(CurrentScreen_Label, LV_ALIGN_TOP_LEFT, 0, 0);  
    lv_obj_set_style_text_font(CurrentScreen_Label, &lv_font_montserrat_12, 0);
    Display_MainMenu_Init = true;
  }
}