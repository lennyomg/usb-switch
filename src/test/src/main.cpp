// Copyright (c) 2026 Leonid R.
// SPDX-License-Identifier: MIT

#include <Arduino.h>
#include <Bounce2.h>
#include <EEPROM.h>

constexpr auto PIN_USB_PWR_1 = PIN_PA6;
constexpr auto PIN_USB_PWR_2 = PIN_PA5;
constexpr auto PIN_USB_DAT_1 = PIN_PB3;
constexpr auto PIN_USB_DAT_2 = PIN_PA7;
constexpr auto PIN_BUZZER = PIN_PB2;
constexpr auto PIN_BTN = PIN_PB1;
constexpr auto PIN_LED = PIN_PB0;

constexpr auto PIN_EXT_1 = PIN_PA1; // hardware SPI MOSI
constexpr auto PIN_EXT_2 = PIN_PA2; // hardware SPI MISO
constexpr auto PIN_EXT_3 = PIN_PA3; // hardware SPI SCK
constexpr auto PIN_EXT_4 = PIN_PA4;
constexpr auto PIN_CS = PIN_PA4;

uint8_t state;
Bounce2::Button btn;
Bounce2::Button ext[4];

void switchModeOff();
void switchMode1();
void switchMode2();
void ledOn();
void ledOff();
void beep(unsigned int freq, unsigned long dur);

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

    ext[0].attach(PIN_EXT_1, INPUT_PULLUP);
    ext[1].attach(PIN_EXT_2, INPUT_PULLUP);
    ext[2].attach(PIN_EXT_3, INPUT_PULLUP);
    ext[3].attach(PIN_EXT_4, INPUT_PULLUP);
    for (auto &e : ext)
    {
        e.interval(5);
        e.setPressedState(LOW);
    }

    for (size_t i = 0; i < 5; i++)
    {
        delay(150);
        ledOff();
        delay(150);
        ledOn();
    }

    beep(660, 100);
    beep(440, 100);
    beep(660, 100);

    state = 0;
}

void loop()
{
    btn.update();

    ext[0].update();
    ext[1].update();
    ext[2].update();
    ext[3].update();

    if (ext[0].pressed())
    {
        switchModeOff();
        beep(440, 250);
    }

    if (ext[1].pressed())
    {
        switchMode1();
        state = 1;
        beep(520, 250);
    }

    if (ext[2].pressed())
    {
        switchMode2();
        state = 2;
        beep(580, 250);
    }

    if (btn.pressed() || ext[3].pressed())
    {
        switch (state)
        {
        case 2:
            state = 1;
            switchMode1();
            break;
        default:
            state = 2;
            switchMode2();
            break;
        }

        beep(680, 250);
        return;
    }
}

void switchModeOff()
{
    ledOn();

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

    ledOff();
}

void switchMode2()
{
    switchModeOff();
    delay(200);

    digitalWrite(PIN_USB_PWR_2, HIGH);
    delay(20);
    digitalWrite(PIN_USB_DAT_2, HIGH);

    ledOff();
}

void beep(const unsigned int freq, const unsigned long dur)
{
    tone(PIN_BUZZER, freq);
    delay(dur);
    noTone(PIN_BUZZER);
    digitalWrite(PIN_BUZZER, LOW);
}

void ledOn()
{
    digitalWrite(PIN_LED, HIGH);
}

void ledOff()
{
    digitalWrite(PIN_LED, LOW);
}
