bool Display_PROscoreRX_Settings_Init = false;

void Display_PROscoreRX_Settings_PRE() {
  ClearScreen();
  lv_obj_t* scr = lv_screen_active();

  // Set screen background
  lv_obj_set_style_bg_color(scr, lv_color_hex(0x000000), 0);
  lv_obj_set_style_bg_opa(scr, LV_OPA_COVER, 0);

  char hex_buffer[8];
  snprintf(hex_buffer, sizeof(hex_buffer), "0x%04X", CurrentScreen);
  lv_obj_t* CurrentScreen_Label = create_label(scr, hex_buffer, &lv_font_montserrat_12, lv_color_white());
  lv_obj_align(CurrentScreen_Label, LV_ALIGN_TOP_LEFT, 0, 0);

  //Close Icon
  lv_obj_t* Icon_Close_Label = create_label(scr, LV_SYMBOL_CLOSE, &lv_font_montserrat_20, lv_color_white());
  lv_obj_align(Icon_Close_Label, LV_ALIGN_TOP_RIGHT, -5, 5);
  lv_obj_add_flag(Icon_Close_Label, LV_OBJ_FLAG_CLICKABLE);
  lv_obj_add_event_cb(Icon_Close_Label, CloseIcon_Clicked, LV_EVENT_CLICKED, NULL);


  lv_obj_t* Millis_cb = lv_checkbox_create(scr);
  lv_checkbox_set_text(Millis_cb, "Enable Millis");
  lv_obj_align(Millis_cb, LV_ALIGN_TOP_LEFT, 20, 50);
  lv_obj_add_event_cb(Millis_cb, settings_checkbox_handler, LV_EVENT_VALUE_CHANGED, (void*)"Millis");

  lv_obj_t* TOut_cb = lv_checkbox_create(scr);
  lv_checkbox_set_text(TOut_cb, "Enable Time Out");
  lv_obj_align(TOut_cb, LV_ALIGN_TOP_LEFT, 20, 80);
  lv_obj_add_event_cb(TOut_cb, settings_checkbox_handler, LV_EVENT_VALUE_CHANGED, (void*)"TimeOut");
}

void Display_PROscoreRX_Settings() {
  if (!Display_PROscoreRX_Settings_Init) {
    Display_PROscoreRX_Settings_PRE();

    Display_PROscoreRX_Settings_Init = true;
  }
}

void Display_PROscoreRX_Settings_POST() {
  Display_PROscoreRX_Settings_Init = false;
}

static void settings_checkbox_handler(lv_event_t* e) {
  lv_obj_t* checkbox = (lv_obj_t*)lv_event_get_target(e);
  bool checked = lv_obj_has_state(checkbox, LV_STATE_CHECKED);

  // Get which checkbox was clicked using user data
  const char* setting = (const char*)lv_event_get_user_data(e);

  Serial.printf("Setting '%s' is now %s\n", setting, checked ? "ON" : "OFF");
}