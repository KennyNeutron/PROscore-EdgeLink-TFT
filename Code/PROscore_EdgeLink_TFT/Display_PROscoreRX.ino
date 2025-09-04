bool Display_PROscoreRX_Init = false;


static void SettingsIcon_Clicked(lv_event_t* e) {
  Display_PROscoreRX_Settings_POST();
  lv_obj_del(SCR_PROscoreRX);
  Display_PROscoreRX_Settings();
}

// Helper function to create and style labels - reduces code duplication
static lv_obj_t* create_label(lv_obj_t* parent, const char* text, const lv_font_t* font, lv_color_t color) {
  lv_obj_t* label = lv_label_create(parent);
  lv_label_set_text(label, text);
  lv_obj_set_style_text_font(label, font, 0);
  lv_obj_set_style_text_color(label, color, 0);
  return label;
}

void Display_PROscoreRX_PRE(void) {
  SCR_PROscoreRX = lv_obj_create(NULL);
  lv_scr_load(SCR_PROscoreRX);

  lv_obj_set_style_bg_color(SCR_PROscoreRX, lv_color_hex(0x000000), 0);
  lv_obj_set_style_bg_opa(SCR_PROscoreRX, LV_OPA_COVER, 0);

  SCR_CurrentScreen = SCR_PROscoreRX;

  // Close icon
  lv_obj_t* Icon_Close_Label = create_label(SCR_PROscoreRX, LV_SYMBOL_CLOSE, &lv_font_montserrat_32, lv_color_white());
  lv_obj_align(Icon_Close_Label, LV_ALIGN_TOP_LEFT, 0, 0);
  lv_obj_add_flag(Icon_Close_Label, LV_OBJ_FLAG_CLICKABLE);
  lv_obj_add_event_cb(Icon_Close_Label, CloseIcon_Clicked, LV_EVENT_CLICKED, NULL);

  // WiFi icon
  // WiFi icon - store reference globally for real-time updates
  lv_color_t wifi_color = NRF24L01_DataReceived ? lv_color_hex(0x00FF00) : lv_color_hex(0xFF0000);
  Icon_WIFI_Label = create_label(SCR_PROscoreRX, LV_SYMBOL_WIFI, &lv_font_montserrat_24, wifi_color);
  lv_obj_align(Icon_WIFI_Label, LV_ALIGN_TOP_RIGHT, -6, 6);

  // Initialize the last state
  last_NRF24L01_state = NRF24L01_DataReceived;

  //Settings Icon
  lv_obj_t* Icon_Settings_Label = create_label(SCR_PROscoreRX, LV_SYMBOL_SETTINGS, &lv_font_montserrat_30, lv_palette_main(LV_PALETTE_GREY));
  lv_obj_align(Icon_Settings_Label, LV_ALIGN_BOTTOM_MID, 0, -60);
  lv_obj_add_flag(Icon_Settings_Label, LV_OBJ_FLAG_CLICKABLE);
  lv_obj_add_event_cb(Icon_Settings_Label, SettingsIcon_Clicked, LV_EVENT_CLICKED, NULL);

  // Initialize game values
  GameTime_Minute = 10;
  GameTime_Second = 0;
  GameTime_Millis = 0;
  Period = 1;
  ShotClock_Second = 24;
  ShotClock_Millis = 0;
  HomeScore = 0;
  GuestScore = 0;
  HomeFoul = 0;
  GuestFoul = 0;
  HomeTOut = 0;
  GuestTOut = 0;

  // Game time Section
  lv_obj_t* Label_GameTime_Text = create_label(SCR_PROscoreRX, "GAME TIME", &lv_font_montserrat_18, lv_color_white());
  lv_obj_align(Label_GameTime_Text, LV_ALIGN_TOP_MID, 5, 5);

  char time_str[8];
  if (HasMillis) {
    snprintf(time_str, sizeof(time_str), "%02d:%02d.%d", GameTime_Minute, GameTime_Second, GameTime_Millis);
  } else {
    snprintf(time_str, sizeof(time_str), "%02d:%02d", GameTime_Minute, GameTime_Second);
  }
  
  // Store reference globally for real-time updates
  Label_GameTime = create_label(SCR_PROscoreRX, time_str, &lv_font_montserrat_48, lv_color_hex(0xFF8C00));
  lv_obj_align(Label_GameTime, LV_ALIGN_TOP_MID, 0, 20);

  // Initialize the last GameTime state
  last_GameTime_Minute = GameTime_Minute;
  last_GameTime_Second = GameTime_Second;
  last_GameTime_Millis = GameTime_Millis;


  // ShotClock Section
  lv_obj_t* Label_ShotClock_Text = create_label(SCR_PROscoreRX, "SHOT CLOCK", &lv_font_montserrat_18, lv_color_white());
  lv_obj_align(Label_ShotClock_Text, LV_ALIGN_CENTER, 0, -30);

  char STR_ShotClock[6];
  if (HasMillis) {
    snprintf(STR_ShotClock, sizeof(STR_ShotClock), "%d.%d", ShotClock_Second, ShotClock_Millis);
  } else {
    snprintf(STR_ShotClock, sizeof(STR_ShotClock), "%d", ShotClock_Second);
  }

  // Store reference globally for real-time updates
  Label_ShotClock = create_label(SCR_PROscoreRX, STR_ShotClock, &lv_font_montserrat_48, lv_color_hex(0x00FF00));
  lv_obj_align(Label_ShotClock, LV_ALIGN_CENTER, 0, 0);
  lv_obj_set_style_text_align(Label_ShotClock, LV_TEXT_ALIGN_CENTER, 0);

  // Initialize the last state
  last_ShotClock_Second = ShotClock_Second;
  last_ShotClock_Millis = ShotClock_Millis;

  // Scores Section
  // Home team
  lv_obj_t* Label_HomeTeam_Text = create_label(SCR_PROscoreRX, "HOME", &lv_font_montserrat_18, lv_color_white());
  lv_obj_align(Label_HomeTeam_Text, LV_ALIGN_BOTTOM_LEFT, 21, -44);

  char STR_HomeScore[6];
  snprintf(STR_HomeScore, sizeof(STR_HomeScore), "%d", HomeScore);
  lv_obj_t* Label_HomeScore = create_label(SCR_PROscoreRX, STR_HomeScore, &lv_font_montserrat_48, lv_color_hex(0xFF0000));
  uint Margin_HomeScore = (HomeScore < 10) ? 50 : (HomeScore < 100) ? 25
                                                                    : 30;
  lv_obj_align(Label_HomeScore, LV_ALIGN_BOTTOM_LEFT, Margin_HomeScore, 0);

  // Guest team
  lv_obj_t* Label_GuestTeam_Text = create_label(SCR_PROscoreRX, "GUEST", &lv_font_montserrat_18, lv_color_white());
  lv_obj_align(Label_GuestTeam_Text, LV_ALIGN_BOTTOM_RIGHT, -15, -44);

  char STR_GuestScore[6];
  snprintf(STR_GuestScore, sizeof(STR_GuestScore), "%d", GuestScore);
  lv_obj_t* Label_GuestScore = create_label(SCR_PROscoreRX, STR_GuestScore, &lv_font_montserrat_48, lv_color_hex(0xFF0000));
  lv_obj_align(Label_GuestScore, LV_ALIGN_BOTTOM_RIGHT, -12, 0);


  if (HasTimeOut) {
    //Foul Section
    //Home Foul
    lv_obj_t* Label_HomeFoul_Text = create_label(SCR_PROscoreRX, "FOUL", &lv_font_montserrat_14, lv_color_white());
    lv_obj_align(Label_HomeFoul_Text, LV_ALIGN_LEFT_MID, 12, 0);

    char STR_HomeFoul[3];
    snprintf(STR_HomeFoul, sizeof(STR_HomeFoul), "%d", HomeFoul);
    lv_obj_t* Label_HomeFoul = create_label(SCR_PROscoreRX, STR_HomeFoul, &lv_font_montserrat_36, lv_color_hex(0xFF00FF));
    lv_obj_align(Label_HomeFoul, LV_ALIGN_LEFT_MID, 18, 24);

    //Guest Foul
    lv_obj_t* Label_GuestFoul_Text = create_label(SCR_PROscoreRX, "FOUL", &lv_font_montserrat_14, lv_color_white());
    lv_obj_align(Label_GuestFoul_Text, LV_ALIGN_RIGHT_MID, -12, 0);

    char STR_GuestFoul[3];
    snprintf(STR_GuestFoul, sizeof(STR_GuestFoul), "%d", GuestFoul);
    lv_obj_t* Label_GuestFoul = create_label(SCR_PROscoreRX, STR_GuestFoul, &lv_font_montserrat_36, lv_color_hex(0xFF00FF));
    lv_obj_align(Label_GuestFoul, LV_ALIGN_RIGHT_MID, -18, 24);

    //Time Out Section
    lv_obj_t* Label_TimeOut_Text = create_label(SCR_PROscoreRX, "T.OUT", &lv_font_montserrat_14, lv_color_white());
    lv_obj_align(Label_TimeOut_Text, LV_ALIGN_LEFT_MID, 12, -60);

    char STR_TimeOut[3];
    snprintf(STR_TimeOut, sizeof(STR_TimeOut), "%d", HomeTOut);
    lv_obj_t* Label_TimeOut = create_label(SCR_PROscoreRX, STR_TimeOut, &lv_font_montserrat_36, lv_color_hex(0xFFFF00));
    lv_obj_align(Label_TimeOut, LV_ALIGN_LEFT_MID, 18, -36);

    //Guest Time Out
    lv_obj_t* Label_GuestTimeOut_Text = create_label(SCR_PROscoreRX, "T.OUT", &lv_font_montserrat_14, lv_color_white());
    lv_obj_align(Label_GuestTimeOut_Text, LV_ALIGN_RIGHT_MID, -12, -60);

    char STR_GuestTimeOut[3];
    snprintf(STR_GuestTimeOut, sizeof(STR_GuestTimeOut), "%d", GuestTOut);
    lv_obj_t* Label_GuestTimeOut = create_label(SCR_PROscoreRX, STR_GuestTimeOut, &lv_font_montserrat_36, lv_color_hex(0xFFFF00));
    lv_obj_align(Label_GuestTimeOut, LV_ALIGN_RIGHT_MID, -18, -36);
  } else {
    //Foul Section
    //Home Foul
    lv_obj_t* Label_HomeFoul_Text = create_label(SCR_PROscoreRX, "FOUL", &lv_font_montserrat_14, lv_color_white());
    lv_obj_align(Label_HomeFoul_Text, LV_ALIGN_LEFT_MID, 12, -18);

    char STR_HomeFoul[3];
    snprintf(STR_HomeFoul, sizeof(STR_HomeFoul), "%d", HomeFoul);
    lv_obj_t* Label_HomeFoul = create_label(SCR_PROscoreRX, STR_HomeFoul, &lv_font_montserrat_36, lv_color_hex(0xFF00FF));
    lv_obj_align(Label_HomeFoul, LV_ALIGN_LEFT_MID, 18, 6);

    //Guest Foul
    lv_obj_t* Label_GuestFoul_Text = create_label(SCR_PROscoreRX, "FOUL", &lv_font_montserrat_14, lv_color_white());
    lv_obj_align(Label_GuestFoul_Text, LV_ALIGN_RIGHT_MID, -12, -18);

    char STR_GuestFoul[3];
    snprintf(STR_GuestFoul, sizeof(STR_GuestFoul), "%d", GuestFoul);
    lv_obj_t* Label_GuestFoul = create_label(SCR_PROscoreRX, STR_GuestFoul, &lv_font_montserrat_36, lv_color_hex(0xFF00FF));
    lv_obj_align(Label_GuestFoul, LV_ALIGN_RIGHT_MID, -18, 6);
  }

  // Period Section
  lv_obj_t* Label_Period_Text = create_label(SCR_PROscoreRX, "QTR", &lv_font_montserrat_14, lv_color_white());
  lv_obj_align(Label_Period_Text, LV_ALIGN_BOTTOM_MID, 0, -40);

  char STR_Period[3];
  snprintf(STR_Period, sizeof(STR_Period), "%d", Period);
  lv_obj_t* Label_Period = create_label(SCR_PROscoreRX, STR_Period, &lv_font_montserrat_36, lv_color_hex(0x00FFFF));
  lv_obj_align(Label_Period, LV_ALIGN_BOTTOM_MID, 0, 0);

  BallPoss = 0;
  //Ball Poss Section
  if (BallPoss == 0) {
    lv_obj_t* Icon_Left_Label = create_label(SCR_PROscoreRX, LV_SYMBOL_LEFT, &lv_font_montserrat_32, lv_color_black());
    lv_obj_align(Icon_Left_Label, LV_ALIGN_BOTTOM_MID, -45, -21);

    lv_obj_t* Icon_Right_Label = create_label(SCR_PROscoreRX, LV_SYMBOL_RIGHT, &lv_font_montserrat_32, lv_color_black());
    lv_obj_align(Icon_Right_Label, LV_ALIGN_BOTTOM_MID, 45, -21);
  } else if (BallPoss == 1) {
    lv_obj_t* Icon_Left_Label = create_label(SCR_PROscoreRX, LV_SYMBOL_LEFT, &lv_font_montserrat_32, lv_color_hex(0xFFFF00));
    lv_obj_align(Icon_Left_Label, LV_ALIGN_BOTTOM_MID, -45, -21);

    lv_obj_t* Icon_Right_Label = create_label(SCR_PROscoreRX, LV_SYMBOL_RIGHT, &lv_font_montserrat_32, lv_color_black());
    lv_obj_align(Icon_Right_Label, LV_ALIGN_BOTTOM_MID, 45, -21);
  } else if (BallPoss == 2) {
    lv_obj_t* Icon_Left_Label = create_label(SCR_PROscoreRX, LV_SYMBOL_LEFT, &lv_font_montserrat_32, lv_color_black());
    lv_obj_align(Icon_Left_Label, LV_ALIGN_BOTTOM_MID, -45, -21);

    lv_obj_t* Icon_Right_Label = create_label(SCR_PROscoreRX, LV_SYMBOL_RIGHT, &lv_font_montserrat_32, lv_color_hex(0xFFFF00));
    lv_obj_align(Icon_Right_Label, LV_ALIGN_BOTTOM_MID, 45, -21);
  } else {
    lv_obj_t* Icon_Left_Label = create_label(SCR_PROscoreRX, LV_SYMBOL_LEFT, &lv_font_montserrat_32, lv_color_black());
    lv_obj_align(Icon_Left_Label, LV_ALIGN_BOTTOM_MID, -45, -21);

    lv_obj_t* Icon_Right_Label = create_label(SCR_PROscoreRX, LV_SYMBOL_RIGHT, &lv_font_montserrat_32, lv_color_black());
    lv_obj_align(Icon_Right_Label, LV_ALIGN_BOTTOM_MID, 45, -21);
  }
}

void Display_PROscoreRX() {
  if (!Display_PROscoreRX_Init) {
    CurrentScreenID = 0x2000;
    Display_PROscoreRX_PRE();
    Display_PROscoreRX_Init = true;
  }
}

void Display_PROscoreRX_POST() {
  Display_PROscoreRX_Init = false;
  Icon_WIFI_Label = NULL;       // Clear the reference
  Label_ShotClock = NULL;       // Clear ShotClock reference
  last_NRF24L01_state = false;  // Reset state tracker
  last_ShotClock_Second = -1;   // Reset ShotClock state tracker
  last_ShotClock_Millis = -1;   // Reset ShotClock state tracker
  last_GameTime_Minute = -1;   // Reset GameTime minute tracker
  last_GameTime_Second = -1;   // Reset GameTime second tracker
  last_GameTime_Millis = -1;   // Reset GameTime millis tracker
}

// Real-time WiFi icon color update function
void update_wifi_icon_realtime() {
  // Only update if we're on PROscoreRX screen and icon exists
  if (CurrentScreenID == 0x2000 && Icon_WIFI_Label != NULL) {
    // Check if NRF24L01 status has changed
    if (NRF24L01_DataReceived != last_NRF24L01_state) {
      lv_color_t new_color = NRF24L01_DataReceived ? lv_color_hex(0x00FF00) : lv_color_hex(0xFF0000);
      lv_obj_set_style_text_color(Icon_WIFI_Label, new_color, LV_PART_MAIN);
      last_NRF24L01_state = NRF24L01_DataReceived;  // Update last state

      // Optional: Serial feedback for debugging
      Serial.println(NRF24L01_DataReceived ? "WiFi icon: GREEN (Data received)" : "WiFi icon: RED (No data)");
      Serial.println("ShotClock_Second: " + String(ShotClock_Second));
    }
  }
}

// Real-time ShotClock update function
void update_shotclock_realtime() {
  // Only update if we're on PROscoreRX screen and label exists
  if (CurrentScreenID == 0x2000 && Label_ShotClock != NULL) {
    
    // Check if the label object is still valid (safety check)
    if (!lv_obj_is_valid(Label_ShotClock)) {
      Label_ShotClock = NULL;
      return;
    }
    
    // Determine which values to check based on HasMillis setting
    bool values_changed = false;
    if (HasMillis) {
      // Check both seconds and milliseconds when HasMillis is true
      values_changed = (ShotClock_Second != last_ShotClock_Second || 
                       ShotClock_Millis != last_ShotClock_Millis);
    } else {
      // Only check seconds when HasMillis is false (more efficient)
      values_changed = (ShotClock_Second != last_ShotClock_Second);
    }
    
    // Only proceed if values actually changed
    if (values_changed) {
      char STR_ShotClock[6];
      if (HasMillis) {
        snprintf(STR_ShotClock, sizeof(STR_ShotClock), "%d.%d", ShotClock_Second, ShotClock_Millis);
      } else {
        snprintf(STR_ShotClock, sizeof(STR_ShotClock), "%d", ShotClock_Second);
      }

      // Update the label text
      lv_label_set_text(Label_ShotClock, STR_ShotClock);

      // Update last state for relevant values
      last_ShotClock_Second = ShotClock_Second;
      if (HasMillis) {
        last_ShotClock_Millis = ShotClock_Millis;
      }
    }
  }
}

// Real-time GameTime update function
void update_gametime_realtime() {
  // Only update if we're on PROscoreRX screen and label exists
  if (CurrentScreenID == 0x2000 && Label_GameTime != NULL) {
    
    // Check if the label object is still valid (safety check)
    if (!lv_obj_is_valid(Label_GameTime)) {
      Label_GameTime = NULL;
      return;
    }
    
    // Determine which values to check based on HasMillis setting
    bool values_changed = false;
    if (HasMillis) {
      // Check minute, second, and milliseconds when HasMillis is true
      values_changed = (GameTime_Minute != last_GameTime_Minute || 
                       GameTime_Second != last_GameTime_Second ||
                       GameTime_Millis != last_GameTime_Millis);
    } else {
      // Only check minute and second when HasMillis is false
      values_changed = (GameTime_Minute != last_GameTime_Minute || 
                       GameTime_Second != last_GameTime_Second);
    }
    
    // Only proceed if values actually changed
    if (values_changed) {
      char time_str[8];
      if (HasMillis) {
        snprintf(time_str, sizeof(time_str), "%02d:%02d.%d", GameTime_Minute, GameTime_Second, GameTime_Millis);
      } else {
        snprintf(time_str, sizeof(time_str), "%02d:%02d", GameTime_Minute, GameTime_Second);
      }

      // Update the label text
      lv_label_set_text(Label_GameTime, time_str);

      // Update last state for relevant values
      last_GameTime_Minute = GameTime_Minute;
      last_GameTime_Second = GameTime_Second;
      if (HasMillis) {
        last_GameTime_Millis = GameTime_Millis;
      }
    }
  }
}