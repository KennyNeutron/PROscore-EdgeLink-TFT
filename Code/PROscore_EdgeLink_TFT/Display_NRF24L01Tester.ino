bool Display_NRF24L01Tester_Init = false;

// NRF24L01 Tester specific variables
lv_obj_t* Label_ConnectionStatus = NULL;
lv_obj_t* Label_SignalStrength = NULL;
lv_obj_t* Label_PacketCount = NULL;
lv_obj_t* Label_TestResults = NULL;
lv_obj_t* Label_DataRate = NULL;
lv_obj_t* Label_PowerLevel = NULL;
lv_obj_t* Label_Channel = NULL;
lv_obj_t* Button_StartTest = NULL;
lv_obj_t* Button_StopTest = NULL;
lv_obj_t* Button_Reset = NULL;

// Test variables
static bool testRunning = false;
static unsigned long testStartTime = 0;
static unsigned long lastPacketTime = 0;
static uint32_t packetsReceived = 0;
static uint32_t packetsExpected = 0;
static uint8_t currentChannel = 76;
static rf24_pa_dbm_e currentPowerLevel = RF24_PA_MAX;
static rf24_datarate_e currentDataRate = RF24_250KBPS;

// Test patterns
struct TestPacket {
  uint32_t packetNumber;
  uint32_t timestamp;
  char testData[20];
  uint8_t checksum;
};

// Button event handlers
static void StartTest_Clicked(lv_event_t* e) {
  testRunning = true;
  testStartTime = millis();
  packetsReceived = 0;
  packetsExpected = 0;
  lastPacketTime = 0;

  // Configure radio for testing
  radio.stopListening();
  radio.setChannel(currentChannel);
  radio.setPALevel(currentPowerLevel);
  radio.setDataRate(currentDataRate);
  radio.openReadingPipe(0, address);
  radio.startListening();

  // Update button states
  lv_obj_add_state(Button_StartTest, LV_STATE_DISABLED);
  lv_obj_clear_state(Button_StopTest, LV_STATE_DISABLED);

  Serial.println("NRF24L01 Test Started");
  lv_label_set_text(Label_TestResults, "Test Running...");
}

static void StopTest_Clicked(lv_event_t* e) {
  testRunning = false;

  // Update button states
  lv_obj_clear_state(Button_StartTest, LV_STATE_DISABLED);
  lv_obj_add_state(Button_StopTest, LV_STATE_DISABLED);

  // Calculate test results
  unsigned long testDuration = millis() - testStartTime;
  float successRate = packetsExpected > 0 ? (float)packetsReceived / packetsExpected * 100.0f : 0.0f;

  char results[100];
  snprintf(results, sizeof(results), "Test Complete\nDuration: %lu ms\nSuccess: %.1f%%",
           testDuration, successRate);
  lv_label_set_text(Label_TestResults, results);

  Serial.printf("NRF24L01 Test Stopped - Success Rate: %.1f%%\n", successRate);
}

static void ResetStats_Clicked(lv_event_t* e) {
  packetsReceived = 0;
  packetsExpected = 0;
  lv_label_set_text(Label_PacketCount, "RX: 0 | TX: 0");
  lv_label_set_text(Label_TestResults, "Ready to test");
  lv_label_set_text(Label_SignalStrength, "Signal: --");
  Serial.println("NRF24L01 Test Stats Reset");
}

static void ConfigButton_Clicked(lv_event_t* e) {
  // Cycle through power levels
  static int powerIndex = 3;  // Start at RF24_PA_MAX
  const rf24_pa_dbm_e powerLevels[] = { RF24_PA_MIN, RF24_PA_LOW, RF24_PA_HIGH, RF24_PA_MAX };
  const char* powerNames[] = { "MIN (-18dBm)", "LOW (-12dBm)", "HIGH (-6dBm)", "MAX (0dBm)" };

  powerIndex = (powerIndex + 1) % 4;
  currentPowerLevel = powerLevels[powerIndex];

  char powerText[50];
  snprintf(powerText, sizeof(powerText), "Power: %s", powerNames[powerIndex]);
  lv_label_set_text(Label_PowerLevel, powerText);

  if (!testRunning) {
    radio.setPALevel(currentPowerLevel);
  }
}

static void ChannelButton_Clicked(lv_event_t* e) {
  // Cycle through common channels
  static int channelIndex = 0;
  const uint8_t channels[] = { 1, 26, 51, 76, 101, 126 };
  const int numChannels = sizeof(channels) / sizeof(channels[0]);

  channelIndex = (channelIndex + 1) % numChannels;
  currentChannel = channels[channelIndex];

  char channelText[30];
  snprintf(channelText, sizeof(channelText), "Channel: %d", currentChannel);
  lv_label_set_text(Label_Channel, channelText);

  if (!testRunning) {
    radio.setChannel(currentChannel);
  }
}

static void DataRateButton_Clicked(lv_event_t* e) {
  // Cycle through data rates
  static int rateIndex = 0;
  const rf24_datarate_e dataRates[] = { RF24_250KBPS, RF24_1MBPS, RF24_2MBPS };
  const char* rateNames[] = { "250 Kbps", "1 Mbps", "2 Mbps" };

  rateIndex = (rateIndex + 1) % 3;
  currentDataRate = dataRates[rateIndex];

  char rateText[30];
  snprintf(rateText, sizeof(rateText), "Rate: %s", rateNames[rateIndex]);
  lv_label_set_text(Label_DataRate, rateText);

  if (!testRunning) {
    radio.setDataRate(currentDataRate);
  }
}

void Display_NRF24L01Tester_PRE(void) {
  SCR_NRF24L01Tester = lv_obj_create(NULL);
  lv_scr_load(SCR_NRF24L01Tester);

  lv_obj_set_style_bg_color(SCR_NRF24L01Tester, lv_color_hex(0x000000), 0);
  lv_obj_set_style_bg_opa(SCR_NRF24L01Tester, LV_OPA_COVER, 0);

  SCR_CurrentScreen = SCR_NRF24L01Tester;
  CurrentScreenID = 0x1000;

  // // Screen title
  // lv_obj_t* id_label = create_label(SCR_NRF24L01Tester, "NRF24L01 Tester", &lv_font_montserrat_12, lv_color_white());
  // lv_obj_align(id_label, LV_ALIGN_TOP_LEFT, 0, 0);

  // Close Icon
  lv_obj_t* Icon_Close_Label = create_label(SCR_NRF24L01Tester, LV_SYMBOL_CLOSE, &lv_font_montserrat_32, lv_color_white());
  lv_obj_align(Icon_Close_Label, LV_ALIGN_TOP_RIGHT, 0, 0);
  lv_obj_add_flag(Icon_Close_Label, LV_OBJ_FLAG_CLICKABLE);
  lv_obj_add_event_cb(Icon_Close_Label, CloseIcon_Clicked, LV_EVENT_CLICKED, NULL);

  // Main title
  lv_obj_t* title = create_label(SCR_NRF24L01Tester, "NRF24L01 TESTER", &lv_font_montserrat_20, lv_color_hex(0x00FFFF));
  lv_obj_align(title, LV_ALIGN_TOP_LEFT, 0, 0);

  // Connection Status
  Label_ConnectionStatus = create_label(SCR_NRF24L01Tester, "Status: Checking...", &lv_font_montserrat_14, lv_color_hex(0xFFFF00));
  lv_obj_align(Label_ConnectionStatus, LV_ALIGN_TOP_LEFT, 0, 25);

  // Signal Strength
  Label_SignalStrength = create_label(SCR_NRF24L01Tester, "Signal: --", &lv_font_montserrat_14, lv_color_hex(0x00FF00));
  lv_obj_align(Label_SignalStrength, LV_ALIGN_TOP_LEFT, 0, 40);

  // Packet Count
  Label_PacketCount = create_label(SCR_NRF24L01Tester, "RX: 0 | TX: 0", &lv_font_montserrat_16, lv_color_white());
  lv_obj_align(Label_PacketCount, LV_ALIGN_TOP_RIGHT, -30, 40);

  // Configuration section
  lv_obj_t* config_title = create_label(SCR_NRF24L01Tester, "CONFIGURATION", &lv_font_montserrat_14, lv_color_hex(0xFF8C00));
  lv_obj_align(config_title, LV_ALIGN_TOP_LEFT, 0, 55);

  // Channel (clickable)
  Label_Channel = create_label(SCR_NRF24L01Tester, "Channel: 76", &lv_font_montserrat_12, lv_color_hex(0x00FFFF));
  lv_obj_align(Label_Channel, LV_ALIGN_TOP_LEFT, 0, 75);
  lv_obj_add_flag(Label_Channel, LV_OBJ_FLAG_CLICKABLE);
  lv_obj_add_event_cb(Label_Channel, ChannelButton_Clicked, LV_EVENT_CLICKED, NULL);

  // Power Level (clickable)
  Label_PowerLevel = create_label(SCR_NRF24L01Tester, "Power: MAX (0dBm)", &lv_font_montserrat_12, lv_color_hex(0x00FFFF));
  lv_obj_align(Label_PowerLevel, LV_ALIGN_TOP_LEFT, 0, 95);
  lv_obj_add_flag(Label_PowerLevel, LV_OBJ_FLAG_CLICKABLE);
  lv_obj_add_event_cb(Label_PowerLevel, ConfigButton_Clicked, LV_EVENT_CLICKED, NULL);

  // Data Rate (clickable)
  Label_DataRate = create_label(SCR_NRF24L01Tester, "Rate: 250 Kbps", &lv_font_montserrat_12, lv_color_hex(0x00FFFF));
  lv_obj_align(Label_DataRate, LV_ALIGN_TOP_RIGHT, -10, 95);
  lv_obj_add_flag(Label_DataRate, LV_OBJ_FLAG_CLICKABLE);
  lv_obj_add_event_cb(Label_DataRate, DataRateButton_Clicked, LV_EVENT_CLICKED, NULL);

  // Control Buttons
  // Start Test Button
  Button_StartTest = lv_button_create(SCR_NRF24L01Tester);
  lv_obj_set_size(Button_StartTest, 80, 30);
  lv_obj_align(Button_StartTest, LV_ALIGN_TOP_LEFT, 10, 165);
  lv_obj_set_style_bg_color(Button_StartTest, lv_color_hex(0x00AA00), 0);
  lv_obj_add_event_cb(Button_StartTest, StartTest_Clicked, LV_EVENT_CLICKED, NULL);

  lv_obj_t* start_label = lv_label_create(Button_StartTest);
  lv_label_set_text(start_label, "START");
  lv_obj_center(start_label);
  lv_obj_set_style_text_color(start_label, lv_color_white(), 0);

  // Stop Test Button
  Button_StopTest = lv_button_create(SCR_NRF24L01Tester);
  lv_obj_set_size(Button_StopTest, 80, 30);
  lv_obj_align(Button_StopTest, LV_ALIGN_TOP_MID, 0, 165);
  lv_obj_set_style_bg_color(Button_StopTest, lv_color_hex(0xAA0000), 0);
  lv_obj_add_state(Button_StopTest, LV_STATE_DISABLED);
  lv_obj_add_event_cb(Button_StopTest, StopTest_Clicked, LV_EVENT_CLICKED, NULL);

  lv_obj_t* stop_label = lv_label_create(Button_StopTest);
  lv_label_set_text(stop_label, "STOP");
  lv_obj_center(stop_label);
  lv_obj_set_style_text_color(stop_label, lv_color_white(), 0);

  // Reset Button
  Button_Reset = lv_button_create(SCR_NRF24L01Tester);
  lv_obj_set_size(Button_Reset, 80, 30);
  lv_obj_align(Button_Reset, LV_ALIGN_TOP_RIGHT, -10, 165);
  lv_obj_set_style_bg_color(Button_Reset, lv_color_hex(0x0066AA), 0);
  lv_obj_add_event_cb(Button_Reset, ResetStats_Clicked, LV_EVENT_CLICKED, NULL);

  lv_obj_t* reset_label = lv_label_create(Button_Reset);
  lv_label_set_text(reset_label, "RESET");
  lv_obj_center(reset_label);
  lv_obj_set_style_text_color(reset_label, lv_color_white(), 0);

  // Test Results Area
  Label_TestResults = create_label(SCR_NRF24L01Tester, "Ready to test", &lv_font_montserrat_14, lv_color_white());
  lv_obj_align(Label_TestResults, LV_ALIGN_CENTER, 0, 20);
  lv_obj_set_width(Label_TestResults, 280);
  lv_label_set_long_mode(Label_TestResults, LV_LABEL_LONG_WRAP);

  // Instructions
  lv_obj_t* instructions = create_label(SCR_NRF24L01Tester, "• Tap config items to change\n• START begins reception test\n• Monitor packet success rate", &lv_font_montserrat_10, lv_palette_main(LV_PALETTE_GREY));
  lv_obj_align(instructions, LV_ALIGN_BOTTOM_LEFT, 10, -10);
  lv_obj_set_width(instructions, 300);
  lv_label_set_long_mode(instructions, LV_LABEL_LONG_WRAP);

  // Initialize hardware check
  checkNRF24L01Hardware();
}

void Display_NRF24L01Tester(void) {
  if (Display_NRF24L01Tester_Init == false) {
    Display_NRF24L01Tester_PRE();
    Display_NRF24L01Tester_Init = true;
  }
}

void Display_NRF24L01Tester_POST() {
  Display_NRF24L01Tester_Init = false;
  testRunning = false;

  // Clear label references
  Label_ConnectionStatus = NULL;
  Label_SignalStrength = NULL;
  Label_PacketCount = NULL;
  Label_TestResults = NULL;
  Label_DataRate = NULL;
  Label_PowerLevel = NULL;
  Label_Channel = NULL;
  Button_StartTest = NULL;
  Button_StopTest = NULL;
  Button_Reset = NULL;
}

// Hardware check function
void checkNRF24L01Hardware() {
  if (!radio.begin()) {
    lv_label_set_text(Label_ConnectionStatus, "Status: FAILED - No Response");
    lv_obj_set_style_text_color(Label_ConnectionStatus, lv_color_hex(0xFF0000), 0);
    Serial.println("NRF24L01 hardware check failed!");
  } else {
    lv_label_set_text(Label_ConnectionStatus, "Status: CONNECTED");
    lv_obj_set_style_text_color(Label_ConnectionStatus, lv_color_hex(0x00FF00), 0);
    Serial.println("NRF24L01 hardware check passed!");

    // Print detailed info
    radio.printDetails();

    // Get radio info for display
    bool isPVariant = radio.isPVariant();
    uint8_t channel = radio.getChannel();

    char info[50];
    snprintf(info, sizeof(info), "Chip: %s | Ch: %d",
             isPVariant ? "nRF24L01+" : "nRF24L01", channel);
    // You could display this info somewhere if needed
  }
}

// Real-time update function for NRF24L01 Tester
void update_nrf24l01_tester_realtime() {
  if (CurrentScreenID != 0x1000) return;

  static unsigned long lastUpdate = 0;
  unsigned long currentTime = millis();

  // Update every 200ms
  if (currentTime - lastUpdate < 200) return;
  lastUpdate = currentTime;

  if (!testRunning) return;

  // Check for incoming packets during test
  if (radio.available()) {
    TestPacket receivedPacket;
    radio.read(&receivedPacket, sizeof(TestPacket));
    packetsReceived++;
    lastPacketTime = currentTime;

    // Calculate signal quality (simplified)
    bool carrierDetect = radio.testCarrier();
    bool rpdDetected = radio.testRPD();

    char signalText[30];
    if (carrierDetect && rpdDetected) {
      snprintf(signalText, sizeof(signalText), "Signal: STRONG");
      lv_obj_set_style_text_color(Label_SignalStrength, lv_color_hex(0x00FF00), 0);
    } else if (rpdDetected) {
      snprintf(signalText, sizeof(signalText), "Signal: MEDIUM");
      lv_obj_set_style_text_color(Label_SignalStrength, lv_color_hex(0xFFFF00), 0);
    } else {
      snprintf(signalText, sizeof(signalText), "Signal: WEAK");
      lv_obj_set_style_text_color(Label_SignalStrength, lv_color_hex(0xFF8800), 0);
    }
    lv_label_set_text(Label_SignalStrength, signalText);
  }

  // Update packet count estimate
  unsigned long testDuration = currentTime - testStartTime;
  packetsExpected = testDuration / 100;  // Expect ~10 packets per second

  char packetText[40];
  snprintf(packetText, sizeof(packetText), "RX: %lu | Expected: %lu", packetsReceived, packetsExpected);
  lv_label_set_text(Label_PacketCount, packetText);

  // Check for timeout (no packets for 2 seconds)
  if (currentTime - lastPacketTime > 2000 && lastPacketTime > 0) {
    lv_label_set_text(Label_SignalStrength, "Signal: TIMEOUT");
    lv_obj_set_style_text_color(Label_SignalStrength, lv_color_hex(0xFF0000), 0);
  }
}