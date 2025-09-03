bool Display_PROscoreRX_Settings_Init = false;
void Display_PROscoreRX_Settings() {
  if (!Display_PROscoreRX_Settings_Init) {
    Serial.println("Settings Screen");  
    ClearScreen();
    lv_obj_t* scr = lv_screen_active();
    
    // Set screen background
    lv_obj_set_style_bg_color(scr, lv_color_hex(0x000000), 0);
    lv_obj_set_style_bg_opa(scr, LV_OPA_COVER, 0);
    //PRINTING SCREEN ID
    char hex_buffer[8];
    snprintf(hex_buffer, sizeof(hex_buffer), "0x%04X", CurrentScreen);
    lv_label_set_text(CurrentScreen_Label, hex_buffer);
    lv_obj_align(CurrentScreen_Label, LV_ALIGN_TOP_LEFT, 0, 0);
    lv_obj_set_style_text_font(CurrentScreen_Label, &lv_font_montserrat_12, 0);
    Display_PROscoreRX_Settings_Init = true;
  }
}