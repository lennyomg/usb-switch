// Copyright (c) 2026 Leonid R.
// SPDX-License-Identifier: MIT

#include <Arduino.h>
#include <Bounce2.h>
#include <EEPROM.h>
#define IR_USE_AVR_TIMER_A
#include <IRremote.hpp>

constexpr auto PIN_USB_PWR_1 = PIN_PA6;
constexpr auto PIN_USB_PWR_2 = PIN_PA5;
constexpr auto PIN_USB_DAT_1 = PIN_PB3;
constexpr auto PIN_USB_DAT_2 = PIN_PA7;
constexpr auto PIN_BUZZER = PIN_PB2;
constexpr auto PIN_BTN = PIN_PB1;
constexpr auto PIN_LED = PIN_PB0;
constexpr auto PIN_IR = PIN_PA1;

constexpr uint8_t STATE_ADDR = 2;
constexpr uint8_t STATE_HEADER_1 = 0x55;
constexpr uint8_t STATE_HEADER_2 = 0xFF;
constexpr uint8_t STATE_1_LENGTH_ADDR = 3;
constexpr uint8_t STATE_2_LENGTH_ADDR = 4;
constexpr uint8_t STATE_1_DATA_ADDR = 10;
constexpr uint8_t STATE_2_DATA_ADDR = 100;

uint8_t state;
Bounce2::Button btn;
uint8_t data[4];
uint8_t dataLength;
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

    pinMode(PIN_PA2, INPUT_PULLUP);
    pinMode(PIN_PA3, INPUT_PULLUP);
    pinMode(PIN_PA4, INPUT_PULLUP);

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

    IrReceiver.begin(PIN_IR, ENABLE_LED_FEEDBACK, PIN_LED);
}

void loop()
{
    auto needSwitch = false;
    btn.update();

    if (IrReceiver.decode())
    {
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

        IrReceiver.resume();
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
    const auto &code = IrReceiver.decodedIRData;

    if (code.flags & IRDATA_FLAGS_IS_REPEAT)
        return false;

    if (code.flags & IRDATA_FLAGS_WAS_OVERFLOW)
        return false;

    if (code.numberOfBits == 0)
        return false;

    if (code.numberOfBits > 32)
        return false;

    dataLength = (code.numberOfBits + 7) / 8;
    for (uint8_t i = 0; i < dataLength; i++)
        data[i] = (code.decodedRawData >> (8 * i)) & 0xFF;

    return true;
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

    disableLEDFeedback();
    for (uint8_t i = 0; i < 3; i++)
    {
        ledOn();
        delay(150);
        ledOff();
        delay(150);
    }
    enableLEDFeedback();
}

void ledOn()
{
    digitalWrite(PIN_LED, HIGH);
}

void ledOff()
{
    digitalWrite(PIN_LED, LOW);
}
