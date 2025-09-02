bool Display_PROscoreRX_Init = false;
void Display_PROscoreRX() {
  if (!Display_PROscoreRX_Init) {
    ClearScreen();

    lv_obj_t* scr = lv_screen_active();

    lv_obj_set_style_bg_color(scr, lv_color_hex(0x000000), 0);
    lv_obj_set_style_bg_opa(scr, LV_OPA_COVER, 0);


    //PRINTING SCREEN ID
    CurrentScreen_Label = lv_label_create(scr);
    char hex_buffer[8];
    snprintf(hex_buffer, sizeof(hex_buffer), "0x%04X", CurrentScreen);
    lv_label_set_text(CurrentScreen_Label, hex_buffer);
    lv_obj_align(CurrentScreen_Label, LV_ALIGN_TOP_LEFT, 5, 5);
    lv_obj_set_style_text_font(CurrentScreen_Label, &lv_font_montserrat_12, 0);

    lv_obj_t* icon_label = lv_label_create(scr);
    lv_label_set_text(icon_label, LV_SYMBOL_WIFI);
    lv_obj_align(icon_label, LV_ALIGN_TOP_RIGHT, -5, 5);
    lv_obj_set_style_text_font(icon_label, &lv_font_montserrat_16, 0);
    lv_obj_set_style_text_color(icon_label, lv_color_hex(0x00FF00), 0);


    //GAMETIME
    GameTime_Minute = 5;
    GameTime_Second = 0;
    GameTime_Millis = 0;

    lv_obj_t* Label_GameTime_Text = lv_label_create(scr);
    lv_label_set_text(Label_GameTime_Text, "GAME TIME");
    lv_obj_align(Label_GameTime_Text, LV_ALIGN_TOP_MID, 5, 5);
    lv_obj_set_style_text_font(Label_GameTime_Text, &lv_font_montserrat_18, 0);
    lv_obj_set_style_text_color(Label_GameTime_Text, lv_color_white(), 0);


    lv_obj_t* Label_GameTime = lv_label_create(scr);
    char time_str[16];
    snprintf(time_str, sizeof(time_str), "%02d:%02d.%d",
             GameTime_Minute,
             GameTime_Second,
             GameTime_Millis);  // Convert milliseconds to tenths

    lv_label_set_text(Label_GameTime, time_str);
    lv_obj_align(Label_GameTime, LV_ALIGN_TOP_MID, 0, 20);
    lv_obj_set_style_text_font(Label_GameTime, &lv_font_montserrat_48, 0);
    lv_obj_set_style_text_color(Label_GameTime, lv_color_hex(0xFF8C00), 0);

    //SHOTCLOCK
    ShotClock_Second=24;
    ShotClock_Millis=0;
    lv_obj_t* Label_ShotClock_Text = lv_label_create(scr);
    lv_label_set_text(Label_ShotClock_Text, "SHOT CLOCK");
    lv_obj_align(Label_ShotClock_Text, LV_ALIGN_CENTER, 0, -30);
    lv_obj_set_style_text_font(Label_ShotClock_Text, &lv_font_montserrat_18, 0);
    lv_obj_set_style_text_color(Label_ShotClock_Text, lv_color_white(), 0);
    lv_obj_t* Label_ShotClock = lv_label_create(scr);

    char shotclock_str[16];
    snprintf(shotclock_str, sizeof(shotclock_str), "%d.%d",
             ShotClock_Second,
             ShotClock_Millis);

    lv_label_set_text(Label_ShotClock, shotclock_str);
    lv_obj_align(Label_ShotClock, LV_ALIGN_CENTER, 0, 0);
    lv_obj_set_style_text_align(Label_ShotClock, LV_TEXT_ALIGN_CENTER, 0);
    lv_obj_set_style_text_font(Label_ShotClock, &lv_font_montserrat_48, 0);
    lv_obj_set_style_text_color(Label_ShotClock, lv_color_hex(0x00FF00), 0);

    //SCORES
    HomeScore = 100;
    GuestScore = 100;

    // Home team label and score
    lv_obj_t* Label_HomeTeam_Text = lv_label_create(scr);
    lv_label_set_text(Label_HomeTeam_Text, "HOME");
    lv_obj_align(Label_HomeTeam_Text, LV_ALIGN_BOTTOM_LEFT, 21, -44);
    lv_obj_set_style_text_font(Label_HomeTeam_Text, &lv_font_montserrat_18, 0);
    lv_obj_set_style_text_color(Label_HomeTeam_Text, lv_color_white(), 0);

    lv_obj_t* Label_HomeScore = lv_label_create(scr);
    // Format and set home score
    char home_score_str[16];
    snprintf(home_score_str, sizeof(home_score_str), "%d", HomeScore);
    lv_label_set_text(Label_HomeScore, home_score_str);
    lv_obj_align(Label_HomeScore, LV_ALIGN_BOTTOM_LEFT, 12, 0);
    lv_obj_set_style_text_font(Label_HomeScore, &lv_font_montserrat_48, 0);
    lv_obj_set_style_text_color(Label_HomeScore, lv_color_hex(0xFF0000), 0);

    // Guest team label and score
    lv_obj_t* Label_GuestTeam_Text = lv_label_create(scr);
    lv_label_set_text(Label_GuestTeam_Text, "GUEST");
    lv_obj_align(Label_GuestTeam_Text, LV_ALIGN_BOTTOM_RIGHT, -21, -44);
    lv_obj_set_style_text_font(Label_GuestTeam_Text, &lv_font_montserrat_18, 0);
    lv_obj_set_style_text_color(Label_GuestTeam_Text, lv_color_white(), 0);

    lv_obj_t* Label_GuestScore = lv_label_create(scr);
    // Format and set guest score
    char guest_score_str[16];
    snprintf(guest_score_str, sizeof(guest_score_str), "%d", GuestScore);
    lv_label_set_text(Label_GuestScore, guest_score_str);
    lv_obj_align(Label_GuestScore, LV_ALIGN_BOTTOM_RIGHT, -12, 0);
    lv_obj_set_style_text_font(Label_GuestScore, &lv_font_montserrat_48, 0);
    lv_obj_set_style_text_color(Label_GuestScore, lv_color_hex(0xFF0000), 0);
    Display_PROscoreRX_Init = true;
  }
}