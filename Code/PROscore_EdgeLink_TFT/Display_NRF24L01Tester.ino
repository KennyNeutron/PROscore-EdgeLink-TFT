static lv_obj_t* SCR_NRF24L01Tester;

void Display_NRF24L01Tester(void) {
  SCR_NRF24L01Tester = lv_obj_create(NULL);
  lv_scr_load(SCR_NRF24L01Tester);

  lv_obj_set_style_bg_color(SCR_NRF24L01Tester, lv_color_hex(0x000000), 0);
  lv_obj_set_style_bg_opa(SCR_NRF24L01Tester, LV_OPA_COVER, 0);

  // Show current screen ID (top-left), consistent with other screens
  char hex_buffer[8];
  snprintf(hex_buffer, sizeof(hex_buffer), "0x%04X", CurrentScreen);
  lv_obj_t* id_label = create_label(SCR_NRF24L01Tester, hex_buffer, &lv_font_montserrat_12, lv_color_white());
  lv_obj_align(id_label, LV_ALIGN_TOP_LEFT, 0, 0);
}