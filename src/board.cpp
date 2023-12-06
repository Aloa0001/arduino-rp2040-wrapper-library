#include "board.hpp"
#include "pico/bootrom.h"

Board::Board(
    uint8_t buttonPin, uint8_t buttonLedPin,
    uint8_t buzzerPin,
    uint8_t joystickXPin, uint8_t joystickYPin,
    uint8_t ledRingPin, uint8_t ledRingBulbs)
{
    button = new Button(buttonPin, buttonLedPin);
    buzzer = new Buzzer(buzzerPin);
    ledRing = new LedRing(ledRingBulbs, ledRingPin);
    display = new Display();
    joystick = new Joystick(joystickXPin, joystickYPin);
}

void Board::updateButtonState()
{
    button->updateButtonState();
}

ButtonType Board::getButtonState()
{
    return button->getButtonState();
}

void Board::solidLedRing(const uint32_t &color)
{
    ledRing->solidColor(color);
}

void Board::flashLedRing(const uint32_t &color, const uint8_t &times, const uint8_t &waitTime)
{
    ledRing->flashColor(color, times, waitTime);
}

void Board::clearDisplay()
{
    display->clearDisplay();
}

void Board::displayTime(const uint8_t &lineNumber)
{
    unsigned long epochTime = WiFi.getTime();
    unsigned long hours = (epochTime % 86400L) / 3600 + /* UTC+1 */ 1;
    unsigned long minutes = (epochTime % 3600) / 60;
    unsigned long seconds = epochTime % 60;

    char *time = (char *)calloc(21, sizeof(char));
    sprintf(time, "Time: %02d:%02d:%02d", hours, minutes, seconds);

    display->setLineText(lineNumber, time);
    free(time);
}

void Board::displayDate(const uint8_t &lineNumber)
{
    time_t time = WiFi.getTime();
    struct tm *timeinfo = gmtime(&time);
    char *date = (char *)calloc(21, sizeof(char));
    sprintf(date, "Date: %02d/%02d/%04d", timeinfo->tm_mday, timeinfo->tm_mon + 1, timeinfo->tm_year + 1900);

    display->setLineText(lineNumber, date);
    free(date);
}

void Board::setDisplayLine(const uint8_t &lineNumber, const char *text)
{
    display->setLineText(lineNumber, text);
}

JoystickDirection Board::getJoystickDirection()
{
    return joystick->getDirection();
}

JoystickCoords Board::getJoystickCoords()
{
    return joystick->getCoords();
}

void Board::setTimer(const uint16_t &time, const uint16_t &pause)
{
    timer.timeBuffer = time;
    timer.pauseBuffer = pause;
    timer.isPaused = false;
    timer.isRunning = false;
    timer.isFinished = false;
}

void Board::startTimer()
{
    timer.isRunning = true;
    timer.isPaused = false;
    timer.isFinished = false;
}

void Board::pauseTimer()
{
    timer.isPaused = true;
    timer.isRunning = false;
    timer.isFinished = false;
}

uint8_t Board::checkTimer()
{
    if (timer.isFinished)
        return 0;

    if (timer.isRunning)
    {
        if (timer.timeBuffer > 0)
        {
            timer.timeBuffer--;
            return timer.timeBuffer / 60;
        }
        else
        {
            timer.isRunning = false;
            timer.isFinished = true;
            timer.timesCompleted++;
            return 0;
        }
    }
    else if (timer.isPaused)
    {
        if (timer.pauseBuffer > 0)
        {
            timer.pauseBuffer--;
            return timer.pauseBuffer / 60;
        }
        else
        {
            timer.isPaused = false;
            timer.isRunning = true;
            return timer.timeBuffer / 60;
        }
    }

    return 0;
}

void Board::reboot()
{
    // reset_usb_boot(1 << digitalPinToPinName(LED_BUILTIN), 0);
    _ontouch1200bps_();
}

Board::~Board()
{
    if (button != nullptr)
        delete button;
    if (buzzer != nullptr)
        delete buzzer;
    if (ledRing != nullptr)
        delete ledRing;
    if (display != nullptr)
        delete display;
}
