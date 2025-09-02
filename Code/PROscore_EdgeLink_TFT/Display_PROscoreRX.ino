bool Display_PROscoreRX_Init = false;
void Display_PROscoreRX() {
  if (!Display_PROscoreRX_Init) {
    ClearScreen();

    // Convert uint16_t to hex string
    char hex_buffer[8];  // Buffer for "0x" + 4 hex digits + null terminator
    snprintf(hex_buffer, sizeof(hex_buffer), "0x%04X", CurrentScreen);
    lv_label_set_text(CurrentScreen_Label, hex_buffer);
    lv_obj_align(CurrentScreen_Label, LV_ALIGN_TOP_LEFT, 0, 0);  // Changed to TOP_LEFT with 5px margins
    lv_obj_set_style_text_font(CurrentScreen_Label, &lv_font_montserrat_12, 0);
    Display_PROscoreRX_Init = true;
  }
}