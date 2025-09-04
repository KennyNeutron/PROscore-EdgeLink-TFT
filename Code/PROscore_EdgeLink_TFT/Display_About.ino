bool Display_About_Init = false;

// Exit button event handler (same as CloseIcon_Clicked)
static void ExitButton_Clicked(lv_event_t* e) {
  // CurrentScreen = 0x0000;  // Return to main menu
  Display_About_POST();
}

void Display_About_PRE() {
  // ClearScreen();
  lv_obj_t* scr = lv_screen_active();

  // Background (dark theme as in other screens)
  lv_obj_set_style_bg_color(scr, lv_color_hex(0x000000), 0);
  lv_obj_set_style_bg_opa(scr, LV_OPA_COVER, 0);

  // Show current screen ID (top-left), consistent with other screens
  // char hex_buffer[8];
  // snprintf(hex_buffer, sizeof(hex_buffer), "0x%04X", CurrentScreen);
  // lv_obj_t* id_label = create_label(scr, hex_buffer, &lv_font_montserrat_12, lv_color_white());
  // lv_obj_align(id_label, LV_ALIGN_TOP_LEFT, 0, 0);

  // Exit Button (replacing close icon)
  lv_obj_t* exit_button = lv_button_create(scr);
  lv_obj_set_size(exit_button, 60, 30);
  lv_obj_align(exit_button, LV_ALIGN_BOTTOM_MID, 0, 200);

  // Style the exit button
  lv_obj_set_style_bg_color(exit_button, lv_color_hex(0xFF0000), 0);                 // Red background
  lv_obj_set_style_bg_color(exit_button, lv_color_hex(0xCC0000), LV_STATE_PRESSED);  // Darker red when pressed
  lv_obj_set_style_radius(exit_button, 5, 0);
  lv_obj_set_style_border_width(exit_button, 1, 0);
  lv_obj_set_style_border_color(exit_button, lv_color_white(), 0);

  // Exit button label
  lv_obj_t* exit_label = lv_label_create(exit_button);
  lv_label_set_text(exit_label, "EXIT");
  lv_obj_center(exit_label);
  lv_obj_set_style_text_color(exit_label, lv_color_white(), 0);
  lv_obj_set_style_text_font(exit_label, &lv_font_montserrat_12, 0);

  // Add click event
  lv_obj_add_event_cb(exit_button, ExitButton_Clicked, LV_EVENT_CLICKED, NULL);

  // Title
  lv_obj_t* title = create_label(scr, "PROscore EdgeLink TFT", &lv_font_montserrat_24, lv_color_white());
  lv_obj_align(title, LV_ALIGN_TOP_LEFT, 10, 18);

  // Subtitle
  lv_obj_t* subtitle = create_label(scr, "Handheld Monitor / Diagnostics for PROscore\nBy: KennyNeutron", &lv_font_montserrat_12, lv_palette_main(LV_PALETTE_GREY));
  lv_obj_align(subtitle, LV_ALIGN_TOP_LEFT, 10, 46);

  // Body text (fixed characters)
  static const char* about_text =
    "• Board: ESP32-2432S028R (ESP32-WROOM + 2.8\" ILI9341)\n"
    "• Display: 240x320, LVGL v9.3.0\n"
    "• Purpose: Mirrors scoreboard data, inspects packets, and visualizes real-time game state (time, shot clock, scores, fouls, TOs, period, possession).\n"
    "• RX/Tools: PROscore RX/TX, NRF24L01 test, HC-05 test, Settings.\n"
    "• UX: Dark theme, Montserrat fonts, icon controls.\n"
    "• Notes: Designed for low-latency rendering; uses single draw unit and label helpers for consistent styling.\n\n"
    "Author: Kenny Walter Diolola (KennyNeutron)\n"
    "Repo: PROscore EdgeLink TFT\n";

  lv_obj_t* body = lv_label_create(scr);
  lv_label_set_long_mode(body, LV_LABEL_LONG_WRAP);
  lv_obj_set_width(body, 300);  // keep left margin readable on 320-wide landscape
  lv_label_set_text(body, about_text);
  lv_obj_set_style_text_font(body, &lv_font_montserrat_14, 0);
  lv_obj_set_style_text_color(body, lv_color_white(), 0);
  lv_obj_align(body, LV_ALIGN_TOP_LEFT, 10, 80);

  // Footer / version line (bottom-left, with space for exit button)
  char ver[48];
  snprintf(ver, sizeof(ver), "Firmware LVGL: %d.%d.%d", lv_version_major(), lv_version_minor(), lv_version_patch());
  lv_obj_t* version = create_label(scr, ver, &lv_font_montserrat_12, lv_palette_main(LV_PALETTE_GREY));
  lv_obj_align(version, LV_ALIGN_BOTTOM_LEFT, 10, 150);
}

void Display_About() {
  if (!Display_About_Init) {
    Display_About_PRE();
    Display_About_Init = true;
  }
}

void Display_About_POST() {
  Display_About_Init = false;
}