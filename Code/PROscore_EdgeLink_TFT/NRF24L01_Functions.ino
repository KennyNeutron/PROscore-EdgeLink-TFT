void NRF24L01_DecodeData() {
  radio.read(&payload, sizeof(Payload_Data));  // Read the whole data and store it into the 'data' structure
  TeamNameSetupMode = payload.TeamNameSetupMode;

  if (!TeamNameSetupMode) {
    HomeScore = payload.HomeScore;
    GuestScore = payload.GuestScore;

    HomeFoul = payload.HomeFoul;
    GuestFoul = payload.GuestFoul;

    HomeTOut = payload.HomeTOut;
    GuestTOut = payload.GuestTOut;

    GameTime_Minute = payload.GameTime_Minute;
    GameTime_Second = payload.GameTime_Second;
    GameTime_Millis = payload.GameTime_Millis;

    ShotClock_Second = payload.ShotClock_Second;
    ShotClock_Millis = payload.ShotClock_Millis;

    BallPoss = payload.BallPoss;

    Period = payload.Period;

    GT_SC_sync = payload.GT_SC_sync;

    SetupMode = payload.SetupMode;

    DataToSet = payload.DataToSet;

    TeamNameSetupMode = payload.TeamNameSetupMode;

    EndOfGame = payload.EndOfGame;

    // if (payload.buzzer) {
    //   tone(BUZZER_PIN, btn.getBuzzerFrequency());
    // } else {
    //   noTone(BUZZER_PIN);
    // }

  } else {
    TeamNameSetupMode = payload.TeamNameSetupMode;
    //   SerialData[0] = 'a';
    //   SerialData[1] = 'b';
    //   for (int i = 2; i <= 11; i++) {
    //     SerialData[i] = payload.CurrentTeamName[i - 2];
    //   }
    //   SerialData[12] = payload.TeamNameSetupMode + 48;
    //   SerialData[13] = payload.TN_ID + 48;
    //   SerialData[14] = 'c';
    //   Serial.print(SerialData);
    //for (int a = 0; a < 16; a++) {Serial.print("SD["); Serial.print(a); Serial.print("]:"); Serial.println(SerialData[a],HEX);}
  }
}