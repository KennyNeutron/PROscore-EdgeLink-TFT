void Display() {
  switch (CurrentScreen) {
    case 0x0000:
      Display_MainMenu();
      break;
    case 0x2000:
      Display_PROscoreRX();
      break;
    case 0x4000:
      Display_PROscoreTX();
      break;
    case 0x2100:
      Display_PROscoreRX_Settings();
      break;
  }
}

void SwitchToScreen(uint16_t newScreen) {
  // Clean up current screen first
  lv_obj_t* scr = lv_screen_active();
  if (scr) {
    // Remove all children (this cleans up event callbacks too)
    lv_obj_clean(scr);
  }
  
  // Small delay to ensure cleanup is complete
  delay(10);
  
  // Now switch screens
  CurrentScreen = newScreen;
  Display_PROscoreRX_Post();
}