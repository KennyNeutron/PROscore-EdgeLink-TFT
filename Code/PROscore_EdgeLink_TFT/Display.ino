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