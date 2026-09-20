#include <Arduino.h>
#include <Adafruit_PN532.h>
#include <Bounce2.h>
#include <EEPROM.h>

constexpr auto PIN_USB_PWR_1 = PIN_PA6;
constexpr auto PIN_USB_PWR_2 = PIN_PA5;
constexpr auto PIN_USB_DAT_1 = PIN_PB3;
constexpr auto PIN_USB_DAT_2 = PIN_PA7;
constexpr auto PIN_BUZZER = PIN_PB2;
constexpr auto PIN_BTN = PIN_PB1;
constexpr auto PIN_LED = PIN_PB0;
constexpr auto PIN_NFC_CS = PIN_PA4;

constexpr uint8_t STATE_ADDR = 2;
constexpr uint8_t STATE_HEADER_1 = 0x65;
constexpr uint8_t STATE_HEADER_2 = 0xBC;
constexpr uint8_t STATE_1_LENGTH_ADDR = 3;
constexpr uint8_t STATE_2_LENGTH_ADDR = 4;
constexpr uint8_t STATE_1_DATA_ADDR = 10;
constexpr uint8_t STATE_2_DATA_ADDR = 100;

constexpr uint8_t DATA_MAX_LENGTH = 50;

uint8_t state;
Bounce2::Button btn;
Adafruit_PN532 pn532(PIN_NFC_CS);
uint8_t data[DATA_MAX_LENGTH];
uint8_t dataLength = 0;
uint8_t prev_data[DATA_MAX_LENGTH];
uint8_t prev_dataLength = 0;
bool learning = false;

void switchModeOff();
void switchMode1();
void switchMode2();
void ledOn();
void ledOff();
void readState();
void writeState();
bool readData();
bool matchData(uint8_t lengthAddr, uint8_t dataAddr);
void learnData(uint8_t learnAddr, uint8_t dataAddr);

void setup()
{
    pinMode(PIN_LED, OUTPUT);
    ledOff();

    switchModeOff();
    pinMode(PIN_USB_PWR_1, OUTPUT);
    pinMode(PIN_USB_PWR_2, OUTPUT);
    pinMode(PIN_USB_DAT_1, OUTPUT);
    pinMode(PIN_USB_DAT_2, OUTPUT);

    pinMode(PIN_BUZZER, OUTPUT);
    digitalWrite(PIN_BUZZER, LOW);

    btn.attach(PIN_BTN, INPUT_PULLUP);
    btn.interval(5);
    btn.setPressedState(LOW);

    if (EEPROM.read(0) != STATE_HEADER_1 || EEPROM.read(1) != STATE_HEADER_2)
    {
        EEPROM.write(0, STATE_HEADER_1);
        EEPROM.write(1, STATE_HEADER_2);
        EEPROM.write(STATE_ADDR, 1);
        for (auto addr = 3; addr < EEPROM.length(); addr++)
            EEPROM.write(addr, 0);
    }

    readState();
    switch (state)
    {
    case 1:
        switchMode1();
        break;
    case 2:
        switchMode2();
        break;
    }

    pn532.begin();
    pn532.SAMConfig();
    pn532.setPassiveActivationRetries(0x01);
}

void loop()
{
    auto needSwitch = false;
    btn.update();

    if (readData())
    {
        if (btn.isPressed() && !learning)
        {
            learning = true;
            switch (state)
            {
            case 1:
                learnData(STATE_1_LENGTH_ADDR, STATE_1_DATA_ADDR);
                break;
            case 2:
                learnData(STATE_2_LENGTH_ADDR, STATE_2_DATA_ADDR);
                break;
            }
        }
        else
        {
            if (matchData(STATE_1_LENGTH_ADDR, STATE_1_DATA_ADDR))
            {
                if (state != 1)
                    needSwitch = true;
            }
            else if (matchData(STATE_2_LENGTH_ADDR, STATE_2_DATA_ADDR))
            {
                if (state != 2)
                    needSwitch = true;
            }
        }
    }

    if (btn.released())
    {
        if (!learning)
        {
            needSwitch = true;
        }
        else
        {
            learning = false;
        }
    }

    if (needSwitch)
    {
        switch (state)
        {
        case 2:
            state = 1;
            writeState();
            switchMode1();
            break;
        default:
            state = 2;
            writeState();
            switchMode2();
            break;
        }
    }
}

void switchModeOff()
{
    digitalWrite(PIN_USB_DAT_1, LOW);
    digitalWrite(PIN_USB_DAT_2, LOW);
    digitalWrite(PIN_USB_PWR_1, LOW);
    digitalWrite(PIN_USB_PWR_2, LOW);
}

void switchMode1()
{
    switchModeOff();
    delay(200);

    digitalWrite(PIN_USB_PWR_1, HIGH);
    delay(20);
    digitalWrite(PIN_USB_DAT_1, HIGH);
}

void switchMode2()
{
    switchModeOff();
    delay(200);

    digitalWrite(PIN_USB_PWR_2, HIGH);
    delay(20);
    digitalWrite(PIN_USB_DAT_2, HIGH);
}

void readState()
{
    state = EEPROM.read(STATE_ADDR);
}

void writeState()
{
    EEPROM.write(STATE_ADDR, state);
}

bool readData()
{
    uint8_t uidLength = 0;
    if (pn532.readPassiveTargetID(PN532_MIFARE_ISO14443A, data, &uidLength, 100) && uidLength <= DATA_MAX_LENGTH)
    {
        dataLength = uidLength;
    }
    else
    {
        dataLength = 0;
    }

    bool changed = dataLength != prev_dataLength;
    if (!changed)
    {
        for (uint8_t i = 0; i < dataLength; i++)
        {
            if (data[i] != prev_data[i])
            {
                changed = true;
                break;
            }
        }
    }

    prev_dataLength = dataLength;
    for (uint8_t i = 0; i < dataLength; i++)
        prev_data[i] = data[i];

    return changed && dataLength > 0;
}

bool matchData(uint8_t lengthAddr, uint8_t dataAddr)
{
    if (EEPROM.read(lengthAddr) != dataLength)
        return false;

    for (uint8_t i = 0; i < dataLength; i++)
    {
        if (EEPROM.read(dataAddr + i) != data[i])
            return false;
    }

    return true;
}

void learnData(uint8_t learnAddr, uint8_t dataAddr)
{
    EEPROM.write(learnAddr, dataLength);
    for (uint8_t i = 0; i < dataLength; i++)
        EEPROM.write(dataAddr + i, data[i]);

    for (uint8_t i = 0; i < 3; i++)
    {
        ledOn();
        delay(150);
        ledOff();
        delay(150);
    }
}

void ledOn()
{
    digitalWrite(PIN_LED, HIGH);
}

void ledOff()
{
    digitalWrite(PIN_LED, LOW);
}
