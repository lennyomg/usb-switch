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

uint8_t state;
Bounce2::Button btn;

void switchModeOff();
void switchMode1();
void switchMode2();
void ledOn();
void ledOff();

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

    pinMode(PIN_PA1, INPUT_PULLUP);
    pinMode(PIN_PA2, INPUT_PULLUP);
    pinMode(PIN_PA3, INPUT_PULLUP);
    pinMode(PIN_PA4, INPUT_PULLUP);

    btn.attach(PIN_BTN, INPUT_PULLUP);
    btn.interval(5);
    btn.setPressedState(LOW);
}

void loop()
{
    btn.update();
    if (btn.isPressed())
    {
        if (state != 1)
        {
            state = 1;
            switchMode1();
        }
    }
    else
    {
        if (state != 2)
        {
            state = 2;
            switchMode2();
        }
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

void ledOn()
{
    digitalWrite(PIN_LED, HIGH);
}

void ledOff()
{
    digitalWrite(PIN_LED, LOW);
}
