bool Display_PROscoreRX_Init = false;
void Display_PROscoreRX() {
  if (!Display_PROscoreRX_Init) {
    ClearScreen();

    lv_obj_t* scr = lv_screen_active();


    //PRINTING SCREEN ID
    CurrentScreen_Label = lv_label_create(scr);
    char hex_buffer[8];
    snprintf(hex_buffer, sizeof(hex_buffer), "0x%04X", CurrentScreen);
    lv_label_set_text(CurrentScreen_Label, hex_buffer);
    lv_obj_align(CurrentScreen_Label, LV_ALIGN_TOP_LEFT, 5, 5);
    lv_obj_set_style_text_font(CurrentScreen_Label, &lv_font_montserrat_12, 0);

    lv_obj_t* icon_label = lv_label_create(scr);
    lv_label_set_text(icon_label, LV_SYMBOL_WIFI);
    lv_obj_align(icon_label, LV_ALIGN_TOP_RIGHT, -3, 3);
    lv_obj_set_style_text_font(icon_label, &lv_font_montserrat_16, 0);
    lv_obj_set_style_text_color(icon_label, lv_color_hex(0x00FF00), 0);

    lv_obj_t* Label_GameTime = lv_label_create(scr);
    lv_label_set_text(Label_GameTime, "10:00.0");
    lv_obj_align(Label_GameTime, LV_ALIGN_TOP_MID, 0, 10);
    lv_obj_set_style_text_font(Label_GameTime, &lv_font_montserrat_48, 0);
    lv_obj_set_style_text_color(Label_GameTime, lv_color_hex(0xFFFF00), 0);

    Display_PROscoreRX_Init = true;
  }
}