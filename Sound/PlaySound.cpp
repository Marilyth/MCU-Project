/*
 * PlaySound.cpp
 *
 *  Created on: 04.04.2020
 *      Author: student
 *
 *  Beispielloesung des Praktikums 2
 */

#include <Sound/PlaySound.h>
#include <math.h>
#include <GameDisplay.h>
#include <Utils/BaseObject.h>
#include <Utils/HoldObject.h>
#include <Utils/HitObject.h>
#include <Utils/StaticBase.h>

void PlaySound::_tone_callback(void *p)
{
    PlaySound * _this = static_cast<PlaySound *>(p);
    if (!_this->_is_silent)
        _this->_buzzer.gpioToggle();
    _this->_sum_us += _this->_delay_us;
}

PlaySound::PlaySound() :
        _delay_us(0), _sum_us(0), _buzzer(PORT_PIN(2, 7))
{
    _buzzer.gpioMode(GPIO::OUTPUT);
}

void PlaySound::playTone(TONE t, int ms)
{
    _delay_us = _calculateDelay_us(t);
    _sum_us = 0;
    _tone_timer.setPeriod(_delay_us, TIMER::PERIODIC);
    _tone_timer.setCallback(_tone_callback, this);
    _buzzer.gpioMode(t == P ? GPIO::INPUT : GPIO::OUTPUT);
    _tone_timer.start();
    while (_sum_us / 1000 < ms)
        ;
    _tone_timer.stop();
}

void CalculateDueTime(ENTRY t[], float BPM)
{
    int idx = 0;
    double msPerBeat = 60000.0 / BPM;
    double usGoneBy = 0;
    while (t[idx].beatDivider != 0)
    {
        double duration = (1.0 / t[idx].beatDivider) * (msPerBeat / songSpeedMultiplicator);
        t[idx].dueAtMS = usGoneBy / 1000;
        t[idx].durationMS = (int)duration;
        usGoneBy += duration * 1000;
        idx++;
    }
}

int PlaySound::_calculateDelay_us(TONE t)
{
    float exponent = (37.0 - (float) t) / 12.0;
    float cnt = pow(2, exponent) * 2272.7272;
    return (int) (cnt + 0.5);
}
