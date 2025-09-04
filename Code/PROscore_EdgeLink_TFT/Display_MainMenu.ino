// Global variables to hold buttons and status label
lv_obj_t* buttons[6];
lv_obj_t* status_label;


bool Display_MainMenu_Init = false;

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
      lv_obj_del(SCR_CurrentScreen);
      Display_NRF24L01Tester_POST();
      Display_NRF24L01Tester();
      break;
    case 1:  // PROscore RX
      // Handle PROscore RX screen
      lv_obj_del(SCR_CurrentScreen);
      Display_PROscoreRX_POST();
      Display_PROscoreRX();
      // Display_MainMenu_POST();
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

void Display_MainMenu_PRE(void) {

  SCR_MainMenu = lv_obj_create(NULL);
  lv_scr_load(SCR_MainMenu);

  lv_obj_set_style_bg_color(SCR_MainMenu, lv_color_hex(0x000000), 0);
  lv_obj_set_style_bg_opa(SCR_MainMenu, LV_OPA_COVER, 0);

  SCR_CurrentScreen = SCR_MainMenu;

  lv_obj_t* id_label = create_label(SCR_MainMenu, "MainMenu", &lv_font_montserrat_12, lv_color_white());
  lv_obj_align(id_label, LV_ALIGN_TOP_LEFT, 0, 0);

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
    "NRF24L01\nTester", "PROscore RX",
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
      buttons[btn_index] = lv_button_create(SCR_MainMenu);
      lv_obj_set_size(buttons[btn_index], BTN_WIDTH, BTN_HEIGHT);
      lv_obj_set_pos(buttons[btn_index], x, y);

      // Style the button
      lv_obj_set_style_bg_color(buttons[btn_index], lv_color_hex(0x0000FF), 0);                 // Normal blue
      lv_obj_set_style_bg_color(buttons[btn_index], lv_color_hex(0x0000CC), LV_STATE_PRESSED);  // Darker blue when pressed
      lv_obj_set_style_bg_opa(buttons[btn_index], LV_OPA_COVER, 0);
      lv_obj_set_style_border_width(buttons[btn_index], 1, 0);
      lv_obj_set_style_border_color(buttons[btn_index], lv_color_white(), 0);
      lv_obj_set_style_radius(buttons[btn_index], 8, 0);

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
  status_label = lv_label_create(SCR_MainMenu);
  lv_label_set_text(status_label, "PROscore EdgeLink TFT - Ready");
  lv_obj_align(status_label, LV_ALIGN_BOTTOM_MID, 0, -5);
  // Set smaller font for status
  lv_obj_set_style_text_font(status_label, &lv_font_montserrat_12, 0);
}

void Display_MainMenu() {
  if (!Display_MainMenu_Init) {
    CurrentScreenID = 0x0000;
    Display_MainMenu_PRE();
    Display_MainMenu_Init = true;
  }
}

void Display_MainMenu_POST() {
  Display_MainMenu_Init = false;
}