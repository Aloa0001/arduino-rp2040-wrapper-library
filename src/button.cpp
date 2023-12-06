// #include "button.hpp"

// Button::Button(uint8_t pin, uint8_t ledPin)
// {
//     this->pin = pin;
//     this->ledPin = ledPin;
//     pinMode(pin, INPUT_PULLUP);
//     pinMode(ledPin, OUTPUT);
// }

// ButtonType Button::checkButtonState()
// {
//     this->state = digitalRead(this->pin);

//     if (this->state == LOW && this->lastState == HIGH)
//     {
//         Serial.println("Button pressed");
//         this->pressCount++;
//         delay(250);

//         this->state = digitalRead(this->pin);
//         if (this->state == LOW)
//         {
//             Serial.println("Button was held (SINGLE CLICK)");
//             digitalWrite(this->ledPin, HIGH);
//             this->lastState = this->state;
//             return SINGLE_PRESS;
//         } else {
//             Serial.println("Button was released (DOUBLE CLICK)");
//             digitalWrite(this->ledPin, LOW);
//             for(uint8_t i = 0; i < 40; i++) {
//                 delay(50);
//                 this->state = digitalRead(this->pin);
//                 if (this->state == LOW) {
//                     Serial.println("Button was pressed (DOUBLE CLICK)");
//                     digitalWrite(this->ledPin, HIGH);
//                     this->pressCount++;
//                     this->lastState = this->state;
//                     return DOUBLE_PRESS;
//                 }
//             }

//             this->lastState = this->state;
//             return NO_PRESS;
//         }
//     } else if (this->state == HIGH && this->lastState == LOW) {
//         Serial.println("Button was released (UNKNOWN STATE)");
//         digitalWrite(this->ledPin, LOW);
//         this->lastState = this->state;
//         return NO_PRESS;
//     } else if (this->state == LOW && this->lastState == LOW) {
//         Serial.println("Button is being held... (LONG PRESS)");

//         // wait for 10 seconds:
//         for (uint8_t i = 0; i < 200; i++) {
//             delay(50);
//             this->state = digitalRead(this->pin);
//             if (this->state == HIGH) {
//                 Serial.println("Button was released before 10 sec mark (LONG PRESS)");
//                 digitalWrite(this->ledPin, LOW);
//                 this->lastState = this->state;
//                 return NO_PRESS;
//             }
//         }

//         Serial.println("Button was held for 10 seconds (LONG PRESS)");
//         digitalWrite(this->ledPin, HIGH);
//         this->lastState = this->state;
//         return LONG_PRESS;
//     }

//     return NO_PRESS;
// }

// Button::~Button()
// {
//     pinMode(pin, INPUT);
// }
#include "button.hpp"

unsigned long pressingTime = 0;
// void (*resetFunc)(void) = 0;

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
        // Serial.println("Button pressed!");
        if ((this->buttonState == UNKNOWN || this->buttonState == NO_PRESS) && this->pressCount % 2 == 0)
        {
            this->buttonState = SINGLE_PRESS;
            Serial.println("Button ON !");
            // this->pressTime = millis();
            this->prevCount++;
            pressingTime = millis();
        }
        else if ((this->buttonState == SINGLE_PRESS) && this->pressCount % 2 == 0)
        {

            this->buttonState = NO_PRESS;
            this->pressTime = millis();
            Serial.println("Button OFF !");
        }
        else if ((this->buttonState == DOUBLE_PRESS) && this->pressCount % 2 == 0)
        {
            this->buttonState = SINGLE_PRESS;
            Serial.println("Button ON !");
        }
        else if (this->pressCount % 2 == 1 && this->buttonState == SINGLE_PRESS && millis() - this->pressTime < this->DOUBLE_CLICK_INTERVAL)
        {
            Serial.println("DOUBLE!");
            this->buttonState = DOUBLE_PRESS;
        }

        this->lastState = this->state;
    }

    if (millis() - pressingTime > 5 * 1000 && this->buttonState == SINGLE_PRESS)
    {
        Serial.println("LONG!");
        // delay(3000);
        // this->reset();
        this->buttonState = RESET;
        // pressingTime = millis();
    }
}

Button::~Button()
{
    pinMode(pin, INPUT);
}
