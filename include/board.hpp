#include <Arduino.h>
#include <time.h>
#include <WiFiNINA.h>

#include "button.hpp"
#include "buzzer.hpp"
#include "ledRing.hpp"
#include "joystick.hpp"
#include "display.hpp"

typedef struct
{
    uint8_t timesCompleted;
    uint16_t timeBuffer;
    uint16_t pauseBuffer;
    bool isPaused;
    bool isRunning;
    bool isFinished;
} Timer;

class Board
{
private:
    Button *button;
    Buzzer *buzzer;
    LedRing *ledRing;
    Display *display;
    Joystick *joystick;
    Timer timer;

public:
    Board(
        uint8_t buttonPin, uint8_t buttonLedPin,
        uint8_t buzzerPin,
        uint8_t joystickXPin, uint8_t joystickYPin,
        uint8_t ledRingPin, uint8_t ledRingBulbs);

    /* Button methods */
    void updateButtonState();
    ButtonType getButtonState();

    /* LED Ring methods */
    void solidLedRing(const uint32_t &color);
    void flashLedRing(const uint32_t &color, const uint8_t &times, const uint8_t &waitTime);

    /* OLED Display methods */
    void clearDisplay();
    void displayTime(const uint8_t &lineNumber);
    void displayDate(const uint8_t &lineNumber);
    void setDisplayLine(const uint8_t &lineNumber, const char *text);

    /* Joystick methods */
    JoystickDirection getJoystickDirection();
    JoystickCoords getJoystickCoords();

    /* Timer methods */
    void setTimer(const uint16_t &time, const uint16_t &pause);
    void startTimer();
    void pauseTimer();
    uint8_t checkTimer();

    /* Reset Method */
    void reboot();

    ~Board();
};
