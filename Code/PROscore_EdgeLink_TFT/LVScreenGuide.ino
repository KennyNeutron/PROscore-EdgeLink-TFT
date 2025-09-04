static  lv_obj_t *scr_1;
static  lv_obj_t *scr_2;

void create_screen_1(void);
void create_screen_2(void);

static void screen1_event_handler(lv_event_t *e)
{
  lv_event_code_t code = lv_event_get_code(e);
  switch (code)
  {
    case LV_EVENT_CLICKED:
    {
      lv_obj_del(scr_1);
      create_screen_2();
    }
    break;

    default: break;
  }
}

static void screen2_event_handler(lv_event_t *e)
{
  lv_event_code_t code = lv_event_get_code(e);
  switch (code)
  {
    case LV_EVENT_CLICKED:
    {
      lv_obj_del(scr_2);
      create_screen_1();
    }
    break;

    default: break;
  }
}

void create_screen_1(void)
{
  scr_1 = lv_obj_create(NULL);
  lv_scr_load(scr_1);

  lv_obj_t * exit_btn = lv_btn_create(scr_1);
  lv_obj_set_style_border_width(exit_btn, 1, LV_PART_MAIN);
  lv_obj_set_style_border_opa(exit_btn, LV_OPA_100, LV_PART_MAIN);
  lv_obj_set_style_border_color(exit_btn, lv_color_black(), LV_PART_MAIN);
  lv_obj_set_size(exit_btn, 80, 30);
  lv_obj_align(exit_btn, LV_ALIGN_CENTER, 0,0);
  lv_obj_add_event_cb(exit_btn, screen1_event_handler, LV_EVENT_CLICKED, NULL);

  lv_obj_t * exit_btn_label = lv_label_create(exit_btn);
  lv_obj_set_style_text_font(exit_btn_label, &lv_font_montserrat_18, LV_PART_MAIN);
  lv_obj_set_style_text_opa(exit_btn_label, LV_OPA_100, LV_PART_MAIN);
  lv_obj_set_style_text_color(exit_btn_label, lv_color_black(), LV_PART_MAIN);
  lv_label_set_text(exit_btn_label, "screen 1");
  lv_obj_align_to(exit_btn_label, exit_btn, LV_ALIGN_CENTER, 0, 0);
}

void create_screen_2(void)
{
  scr_2 = lv_obj_create(NULL);
  lv_scr_load(scr_2);

  lv_obj_t * exit_btn = lv_btn_create(scr_1);
  lv_obj_set_style_border_width(exit_btn, 1, LV_PART_MAIN);
  lv_obj_set_style_border_opa(exit_btn, LV_OPA_100, LV_PART_MAIN);
  lv_obj_set_style_border_color(exit_btn, lv_color_white(), LV_PART_MAIN);
  lv_obj_set_size(exit_btn, 80, 30);
  lv_obj_align(exit_btn, LV_ALIGN_CENTER, 0,0);
  lv_obj_add_event_cb(exit_btn, screen2_event_handler, LV_EVENT_CLICKED, NULL);

  lv_obj_t * exit_btn_label = lv_label_create(exit_btn);
  lv_label_set_text(exit_btn_label, "screen 2");
  lv_obj_set_style_text_font(exit_btn_label, &lv_font_montserrat_14, LV_PART_MAIN);
  lv_obj_set_style_text_opa(exit_btn_label, LV_OPA_100, LV_PART_MAIN);
  lv_obj_set_style_text_color(exit_btn_label, lv_color_white(), LV_PART_MAIN);
  lv_obj_align_to(exit_btn_label, exit_btn, LV_ALIGN_CENTER, 0, 0);
}