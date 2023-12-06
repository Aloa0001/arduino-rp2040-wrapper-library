#include <Arduino.h>

enum ButtonType
{
    NO_PRESS,
    SINGLE_PRESS,
    DOUBLE_PRESS,
    LONG_PRESS,
    UNKNOWN,
    RESET
};

class Button
{
private:
    uint8_t pin;
    uint8_t ledPin;

public:
    ButtonType buttonState = ButtonType::UNKNOWN;
    uint8_t state = LOW;
    uint8_t lastState = LOW;
    uint16_t pressCount = 0;
    uint16_t prevCount = 0;
    unsigned long pressTime = 0;
    const unsigned long DOUBLE_CLICK_INTERVAL = 1000;

    Button(uint8_t pin, uint8_t ledPin);
    ButtonType getButtonState();
    void updateButtonState();

    ~Button();
};
