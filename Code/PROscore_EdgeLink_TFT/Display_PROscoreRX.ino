bool Display_PROscoreRX_Init = false;

static void CloseIcon_Clicked(lv_event_t* e) {
  CurrentScreen = 0x0000;
  Display_PROscoreRX_Post();
}

static void SettingsIcon_Clicked(lv_event_t* e) {
  CurrentScreen = 0x2100;
  Display_PROscoreRX_Post();
}

// Helper function to create and style labels - reduces code duplication
static lv_obj_t* create_label(lv_obj_t* parent, const char* text, const lv_font_t* font, lv_color_t color) {
  lv_obj_t* label = lv_label_create(parent);
  lv_label_set_text(label, text);
  lv_obj_set_style_text_font(label, font, 0);
  lv_obj_set_style_text_color(label, color, 0);
  return label;
}

void Display_PROscoreRX() {
  if (!Display_PROscoreRX_Init) {
    ClearScreen();
    lv_obj_t* scr = lv_screen_active();

    // Set screen background
    lv_obj_set_style_bg_color(scr, lv_color_hex(0x000000), 0);
    lv_obj_set_style_bg_opa(scr, LV_OPA_COVER, 0);

    // Close icon
    lv_obj_t* Icon_Close_Label = create_label(scr, LV_SYMBOL_CLOSE, &lv_font_montserrat_20, lv_color_white());
    lv_obj_align(Icon_Close_Label, LV_ALIGN_TOP_LEFT, 5, 5);
    lv_obj_add_flag(Icon_Close_Label, LV_OBJ_FLAG_CLICKABLE);
    lv_obj_add_event_cb(Icon_Close_Label, CloseIcon_Clicked, LV_EVENT_CLICKED, NULL);

    // WiFi icon
    lv_obj_t* Icon_WIFI_Label = create_label(scr, LV_SYMBOL_WIFI, &lv_font_montserrat_16, lv_color_hex(0x00FF00));
    lv_obj_align(Icon_WIFI_Label, LV_ALIGN_TOP_RIGHT, -5, 5);

    //Settings Icon
    lv_obj_t* Icon_Settings_Label = create_label(scr, LV_SYMBOL_SETTINGS, &lv_font_montserrat_24, lv_palette_main(LV_PALETTE_GREY));
    lv_obj_align(Icon_Settings_Label, LV_ALIGN_BOTTOM_MID, 0, -70);
    lv_obj_add_flag(Icon_Settings_Label, LV_OBJ_FLAG_CLICKABLE);
    lv_obj_add_event_cb(Icon_Settings_Label, SettingsIcon_Clicked, LV_EVENT_CLICKED, NULL);

    // Initialize game values
    GameTime_Minute = 10;
    GameTime_Second = 0;
    GameTime_Millis = 0;
    Period = 3;
    ShotClock_Second = 24;
    ShotClock_Millis = 0;
    HomeScore = 20;
    GuestScore = 20;
    HomeFoul = 4;
    GuestFoul = 3;
    HomeTOut = 2;
    GuestTOut = 3;

    // Game time Section
    lv_obj_t* Label_GameTime_Text = create_label(scr, "GAME TIME", &lv_font_montserrat_18, lv_color_white());
    lv_obj_align(Label_GameTime_Text, LV_ALIGN_TOP_MID, 5, 5);

    char time_str[8];
    snprintf(time_str, sizeof(time_str), "%02d:%02d.%d", GameTime_Minute, GameTime_Second, GameTime_Millis);
    lv_obj_t* Label_GameTime = create_label(scr, time_str, &lv_font_montserrat_48, lv_color_hex(0xFF8C00));
    lv_obj_align(Label_GameTime, LV_ALIGN_TOP_MID, 0, 20);


    // ShotClock Section
    lv_obj_t* Label_ShotClock_Text = create_label(scr, "SHOT CLOCK", &lv_font_montserrat_18, lv_color_white());
    lv_obj_align(Label_ShotClock_Text, LV_ALIGN_CENTER, 0, -30);

    char STR_ShotClock[6];
    snprintf(STR_ShotClock, sizeof(STR_ShotClock), "%d.%d", ShotClock_Second, ShotClock_Millis);
    lv_obj_t* Label_ShotClock = create_label(scr, STR_ShotClock, &lv_font_montserrat_48, lv_color_hex(0x00FF00));
    lv_obj_align(Label_ShotClock, LV_ALIGN_CENTER, 0, 0);
    lv_obj_set_style_text_align(Label_ShotClock, LV_TEXT_ALIGN_CENTER, 0);

    // Scores Section
    // Home team
    lv_obj_t* Label_HomeTeam_Text = create_label(scr, "HOME", &lv_font_montserrat_18, lv_color_white());
    lv_obj_align(Label_HomeTeam_Text, LV_ALIGN_BOTTOM_LEFT, 21, -44);

    char STR_HomeScore[6];
    snprintf(STR_HomeScore, sizeof(STR_HomeScore), "%d", HomeScore);
    lv_obj_t* Label_HomeScore = create_label(scr, STR_HomeScore, &lv_font_montserrat_48, lv_color_hex(0xFF0000));
    uint Margin_HomeScore = (HomeScore < 10) ? 50 : (HomeScore < 100) ? 25
                                                                      : 30;
    lv_obj_align(Label_HomeScore, LV_ALIGN_BOTTOM_LEFT, Margin_HomeScore, 0);

    // Guest team
    lv_obj_t* Label_GuestTeam_Text = create_label(scr, "GUEST", &lv_font_montserrat_18, lv_color_white());
    lv_obj_align(Label_GuestTeam_Text, LV_ALIGN_BOTTOM_RIGHT, -15, -44);

    char STR_GuestScore[6];
    snprintf(STR_GuestScore, sizeof(STR_GuestScore), "%d", GuestScore);
    lv_obj_t* Label_GuestScore = create_label(scr, STR_GuestScore, &lv_font_montserrat_48, lv_color_hex(0xFF0000));
    lv_obj_align(Label_GuestScore, LV_ALIGN_BOTTOM_RIGHT, -12, 0);

    //Foul Section
    //Home Foul
    lv_obj_t* Label_HomeFoul_Text = create_label(scr, "FOUL", &lv_font_montserrat_14, lv_color_white());
    lv_obj_align(Label_HomeFoul_Text, LV_ALIGN_LEFT_MID, 12, -0);

    char STR_HomeFoul[3];
    snprintf(STR_HomeFoul, sizeof(STR_HomeFoul), "%d", HomeFoul);
    lv_obj_t* Label_HomeFoul = create_label(scr, STR_HomeFoul, &lv_font_montserrat_36, lv_color_hex(0xFF00FF));
    lv_obj_align(Label_HomeFoul, LV_ALIGN_LEFT_MID, 18, 20);

    //Guest Foul
    lv_obj_t* Label_GuestFoul_Text = create_label(scr, "FOUL", &lv_font_montserrat_14, lv_color_white());
    lv_obj_align(Label_GuestFoul_Text, LV_ALIGN_RIGHT_MID, -12, 0);

    char STR_GuestFoul[3];
    snprintf(STR_GuestFoul, sizeof(STR_GuestFoul), "%d", GuestFoul);
    lv_obj_t* Label_GuestFoul = create_label(scr, STR_GuestFoul, &lv_font_montserrat_36, lv_color_hex(0xFF00FF));
    lv_obj_align(Label_GuestFoul, LV_ALIGN_RIGHT_MID, -18, 20);

    //Time Out Section
    //Home Time Out
    lv_obj_t* Label_TimeOut_Text = create_label(scr, "T.OUT", &lv_font_montserrat_14, lv_color_white());
    lv_obj_align(Label_TimeOut_Text, LV_ALIGN_LEFT_MID, 12, -60);

    char STR_TimeOut[3];
    snprintf(STR_TimeOut, sizeof(STR_TimeOut), "%d", HomeTOut);
    lv_obj_t* Label_TimeOut = create_label(scr, STR_TimeOut, &lv_font_montserrat_36, lv_color_hex(0xFFFF00));
    lv_obj_align(Label_TimeOut, LV_ALIGN_LEFT_MID, 18, -40);

    //Guest Time Out
    lv_obj_t* Label_GuestTimeOut_Text = create_label(scr, "T.OUT", &lv_font_montserrat_14, lv_color_white());
    lv_obj_align(Label_GuestTimeOut_Text, LV_ALIGN_RIGHT_MID, -12, -60);

    char STR_GuestTimeOut[3];
    snprintf(STR_GuestTimeOut, sizeof(STR_GuestTimeOut), "%d", GuestTOut);
    lv_obj_t* Label_GuestTimeOut = create_label(scr, STR_GuestTimeOut, &lv_font_montserrat_36, lv_color_hex(0xFFFF00));
    lv_obj_align(Label_GuestTimeOut, LV_ALIGN_RIGHT_MID, -18, -40);

    // Period Section
    lv_obj_t* Label_Period_Text = create_label(scr, "QTR", &lv_font_montserrat_14, lv_color_white());
    lv_obj_align(Label_Period_Text, LV_ALIGN_BOTTOM_MID, 0, -40);

    char STR_Period[3];
    snprintf(STR_Period, sizeof(STR_Period), "%d", Period);
    lv_obj_t* Label_Period = create_label(scr, STR_Period, &lv_font_montserrat_36, lv_color_hex(0x00FFFF));
    lv_obj_align(Label_Period, LV_ALIGN_BOTTOM_MID, 0, 0);
    Display_PROscoreRX_Init = true;
  }
}

void Display_PROscoreRX_Post() {
  Display_PROscoreRX_Init = false;
}