uint16_t CurrentScreenID = 0x0000;

//PROscore Variables
uint8_t GameTime_Minute;
uint8_t GameTime_Second;
uint8_t GameTime_Millis;

uint8_t ShotClock_Second;
uint8_t ShotClock_Millis;

uint8_t HomeScore;
uint8_t GuestScore;

uint8_t HomeFoul;
uint8_t GuestFoul;

uint8_t HomeTOut;
uint8_t GuestTOut;

uint8_t Period;
uint8_t BallPoss;
bool Buzzer;

//msc
bool GT_SC_sync = false;

bool SetupMode = false;
uint8_t DataToSet = 0;

uint32_t blinker_millis = 0;
bool blink = false;

bool EndOfGame = false;

//TeamNames
bool TeamNameSetupMode = false;

//PROscore Settings
bool HasMillis = false;
bool HasTimeOut = false;

//NRF24L01
const byte address[6] = "PT001";
bool NRF24L01_DataReceived = false;