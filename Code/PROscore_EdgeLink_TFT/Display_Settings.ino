bool Display_Settings_Init = false;

void Display_Settings_PRE(){
    SCR_Settings = lv_obj_create(NULL);
    lv_scr_load(SCR_Settings);

    lv_obj_set_style_bg_color(SCR_Settings, lv_color_hex(0x000000), 0);
    lv_obj_set_style_bg_opa(SCR_Settings, LV_OPA_COVER, 0);

    SCR_CurrentScreen = SCR_Settings;

    //Screen title
    lv_obj_t* id_label = create_label(SCR_Settings, "Settings", &lv_font_montserrat_22, lv_color_white());
    lv_obj_align(id_label, LV_ALIGN_TOP_LEFT, 0, 0);

    // Close icon
    lv_obj_t* Icon_Close_Label = create_label(SCR_Settings, LV_SYMBOL_CLOSE, &lv_font_montserrat_32, lv_color_white());
    lv_obj_align(Icon_Close_Label, LV_ALIGN_TOP_RIGHT, 0, 0);
    lv_obj_add_flag(Icon_Close_Label, LV_OBJ_FLAG_CLICKABLE);
    lv_obj_add_event_cb(Icon_Close_Label, CloseIcon_Clicked, LV_EVENT_CLICKED, NULL);


    //Test Font
    LV_FONT_DECLARE(font_SevenSegment_48_bpp1);
    lv_obj_t* TestFontLabel = create_label(SCR_Settings, "12:34.5", &font_SevenSegment_48_bpp1, lv_color_white());
    lv_obj_align(TestFontLabel, LV_ALIGN_TOP_LEFT, 0, 50);


}


void Display_Settings(){
    if(!Display_Settings_Init){
        CurrentScreenID = 0x5000;
        Display_Settings_PRE();
    }
    Display_Settings_Init = true;
}

void Display_Settings_POST(){
    Display_Settings_Init = false;
}