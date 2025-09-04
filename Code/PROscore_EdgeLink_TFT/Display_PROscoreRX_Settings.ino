bool Display_PROscoreRX_Settings_Init = false;

static void settings_checkbox_handler(lv_event_t* e) {
  lv_obj_t* checkbox = (lv_obj_t*)lv_event_get_target(e);
  bool checked = lv_obj_has_state(checkbox, LV_STATE_CHECKED);

  // Get which checkbox was clicked using user data
  const char* setting = (const char*)lv_event_get_user_data(e);

  // Update the corresponding global variable
  if (strcmp(setting, "Millis") == 0) {
    HasMillis = checked;
    Serial.printf("HasMillis is now %s\n", HasMillis ? "ON" : "OFF");
  } else if (strcmp(setting, "TimeOut") == 0) {
    HasTimeOut = checked;
    Serial.printf("HasTimeOut is now %s\n", HasTimeOut ? "ON" : "OFF");
  }

  Serial.printf("Setting '%s' is now %s\n", setting, checked ? "ON" : "OFF");
}

void Display_PROscoreRX_Settings_PRE(void) {
  SCR_PROscoreRX_Settings = lv_obj_create(NULL);
  lv_scr_load(SCR_PROscoreRX_Settings);

  lv_obj_set_style_bg_color(SCR_PROscoreRX_Settings, lv_color_hex(0x000000), 0);
  lv_obj_set_style_bg_opa(SCR_PROscoreRX_Settings, LV_OPA_COVER, 0);

  SCR_CurrentScreen = SCR_PROscoreRX_Settings;

  lv_obj_t* id_label = create_label(SCR_PROscoreRX_Settings, "PROscoreRX Settings", &lv_font_montserrat_12, lv_color_white());
  lv_obj_align(id_label, LV_ALIGN_TOP_LEFT, 0, 0);

  //Close Icon
  lv_obj_t* Icon_Close_Label = create_label(SCR_PROscoreRX_Settings, LV_SYMBOL_CLOSE, &lv_font_montserrat_20, lv_color_white());
  lv_obj_align(Icon_Close_Label, LV_ALIGN_TOP_RIGHT, -5, 5);
  lv_obj_add_flag(Icon_Close_Label, LV_OBJ_FLAG_CLICKABLE);
  lv_obj_add_event_cb(Icon_Close_Label, CloseIcon_Clicked, LV_EVENT_CLICKED, NULL);

  // Millis checkbox - set initial state based on global variable
  lv_obj_t* Millis_cb = lv_checkbox_create(SCR_PROscoreRX_Settings);
  lv_checkbox_set_text(Millis_cb, "Enable Millis");
  lv_obj_align(Millis_cb, LV_ALIGN_TOP_LEFT, 20, 50);
  // Set checkbox state based on global variable
  if (HasMillis) {
    lv_obj_add_state(Millis_cb, LV_STATE_CHECKED);
  } else {
    lv_obj_clear_state(Millis_cb, LV_STATE_CHECKED);
  }
  lv_obj_add_event_cb(Millis_cb, settings_checkbox_handler, LV_EVENT_VALUE_CHANGED, (void*)"Millis");

  // TimeOut checkbox - set initial state based on global variable
  lv_obj_t* TOut_cb = lv_checkbox_create(SCR_PROscoreRX_Settings);
  lv_checkbox_set_text(TOut_cb, "Enable Time Out");
  lv_obj_align(TOut_cb, LV_ALIGN_TOP_LEFT, 20, 80);

  // Set checkbox state based on global variable
  if (HasTimeOut) {
    lv_obj_add_state(TOut_cb, LV_STATE_CHECKED);
  } else {
    lv_obj_clear_state(TOut_cb, LV_STATE_CHECKED);
  }
  lv_obj_add_event_cb(TOut_cb, settings_checkbox_handler, LV_EVENT_VALUE_CHANGED, (void*)"TimeOut");
}
void Display_PROscoreRX_Settings() {
  if (!Display_PROscoreRX_Settings_Init) {
    CurrentScreenID = 0x2100;
    Display_PROscoreRX_Settings_PRE();
    Display_PROscoreRX_Settings_Init = true;
  }
}

void Display_PROscoreRX_Settings_POST() {
  Display_PROscoreRX_Settings_Init = false;
}