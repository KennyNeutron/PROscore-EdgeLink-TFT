bool Display_PROscoreRX_Settings_Init = false;

void Display_PROscoreRX_Settings_PRE(){
    ClearScreen();
    lv_obj_t* scr = lv_screen_active();
    
    // Set screen background
    lv_obj_set_style_bg_color(scr, lv_color_hex(0x000000), 0);
    lv_obj_set_style_bg_opa(scr, LV_OPA_COVER, 0);

    char hex_buffer[8];
    snprintf(hex_buffer, sizeof(hex_buffer), "0x%04X", CurrentScreen);
    lv_obj_t* CurrentScreen_Label = create_label(scr, hex_buffer, &lv_font_montserrat_12, lv_color_white());
    lv_obj_align(CurrentScreen_Label, LV_ALIGN_TOP_LEFT, 0, 0);
}

void Display_PROscoreRX_Settings() {
  if (!Display_PROscoreRX_Settings_Init) {
    Display_PROscoreRX_Settings_PRE();

    Display_PROscoreRX_Settings_Init = true;
  }
}