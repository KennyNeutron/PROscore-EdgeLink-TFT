bool Display_NRF24L01Tester_Init = false;

void Display_NRF24L01Tester_PRE(void){
  SCR_NRF24L01Tester = lv_obj_create(NULL);
  lv_scr_load(SCR_NRF24L01Tester);

  lv_obj_set_style_bg_color(SCR_NRF24L01Tester, lv_color_hex(0x000000), 0);
  lv_obj_set_style_bg_opa(SCR_NRF24L01Tester, LV_OPA_COVER, 0);

  SCR_CurrentScreen = SCR_NRF24L01Tester;
  CurrentScreenID = 0x1000;

  lv_obj_t* id_label = create_label(SCR_NRF24L01Tester, "NRF24L01 Tester", &lv_font_montserrat_12, lv_color_white());
  lv_obj_align(id_label, LV_ALIGN_TOP_LEFT, 0, 0);

    //Close Icon
  lv_obj_t* Icon_Close_Label = create_label(SCR_NRF24L01Tester, LV_SYMBOL_CLOSE, &lv_font_montserrat_32, lv_color_white());
  lv_obj_align(Icon_Close_Label, LV_ALIGN_TOP_RIGHT, 0, 0);
  lv_obj_add_flag(Icon_Close_Label, LV_OBJ_FLAG_CLICKABLE);
  lv_obj_add_event_cb(Icon_Close_Label, CloseIcon_Clicked, LV_EVENT_CLICKED, NULL);
}

void Display_NRF24L01Tester(void) {
  if (Display_NRF24L01Tester_Init == false) {
    Display_NRF24L01Tester_PRE();
    Display_NRF24L01Tester_Init = true;
  }
}

void Display_NRF24L01Tester_POST() {
  Display_NRF24L01Tester_Init = false;
}