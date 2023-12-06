#include "button.hpp"

unsigned long pressingTime = 0;

Button::Button(uint8_t pin, uint8_t ledPin)
{
    this->pin = pin;
    this->ledPin = ledPin;
    pinMode(pin, INPUT_PULLDOWN);
    pinMode(ledPin, OUTPUT);
}

ButtonType Button::getButtonState()
{
    return this->buttonState;
}

void Button::updateButtonState()
{
    this->state = digitalRead(this->pin);

    if (this->state != this->lastState)
    {
        this->pressCount++;
        if ((this->buttonState == UNKNOWN || this->buttonState == NO_PRESS) && this->pressCount % 2 == 0)
        {
            this->buttonState = SINGLE_PRESS;
            Serial.println("Button ON !");
            this->prevCount++;
            pressingTime = millis();
            digitalWrite(this->ledPin, HIGH);
        }
        else if ((this->buttonState == SINGLE_PRESS) && this->pressCount % 2 == 0)
        {
            this->buttonState = NO_PRESS;
            this->pressTime = millis();
            Serial.println("Button OFF !");
            digitalWrite(this->ledPin, HIGH);
        }
        else if ((this->buttonState == DOUBLE_PRESS) && this->pressCount % 2 == 0)
        {
            this->buttonState = SINGLE_PRESS;
            Serial.println("Button ON !");
            digitalWrite(this->ledPin, HIGH);
        }
        else if (this->pressCount % 2 == 1 && this->buttonState == SINGLE_PRESS && millis() - this->pressTime < this->DOUBLE_CLICK_INTERVAL)
        {
            Serial.println("Double Click!");
            this->buttonState = DOUBLE_PRESS;
        }

        this->lastState = this->state;
    }

    if (millis() - pressingTime > 5 * 1000 && this->buttonState == SINGLE_PRESS)
    {
        Serial.println("Long Press -> reset!");
        this->buttonState = RESET;
    }
    digitalWrite(this->ledPin, LOW);
}

Button::~Button()
{
    pinMode(pin, INPUT);
}
