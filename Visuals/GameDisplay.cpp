/*****************************************************************************
 *
 * Copyright (C) 2013 - 2017 Texas Instruments Incorporated - http://www.ti.com/
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * * Redistributions of source code must retain the above copyright
 *   notice, this list of conditions and the following disclaimer.
 *
 * * Redistributions in binary form must reproduce the above copyright
 *   notice, this list of conditions and the following disclaimer in the
 *   documentation and/or other materials provided with the
 *   distribution.
 *
 * * Neither the name of Texas Instruments Incorporated nor the names of
 *   its contributors may be used to endorse or promote products derived
 *   from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 * OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 ******************************************************************************
 *
 * MSP432 empty main.c template
 *
 ******************************************************************************/
#include <Visuals/GameDisplay.h>
#include <Utils/GameInformation.h>
#include <Utils/StaticBase.h>
#include <Utils/SuccessObject.h>
#include <stdio.h>
#include "math.h"
#include "gpio_msp432.h"
#include "timer_msp432.h"
#include "spi_msp432.h"
#include "adc14_msp432.h"
#include "st7735s_drv.h"
#include "uGUI.h"
#include "uGUI_colors.h"
#include "font_4x6.h"
#include "font_6x10.h"
#include "yahal_String.h"
#include <string>
#include <Sound/Songs.h>

st7735s_drv* lcd = nullptr;
uGUI* gui = nullptr;
GameInformation* infos = new GameInformation();
BaseObject* gameObjects[20];
extern const uint16_t heart[121];

void callback(void * arg)
{
    (*static_cast<bool *>(arg)) = true;
}

void wait(int ms)
{
    bool expired = false;
    timer_msp432 timer(TIMER32_2);
    timer.setPeriod(ms * 1000, TIMER::ONE_SHOT);
    timer.setCallback(callback, &expired);
    timer.start();

    while (!expired)
        ;
}

void StartDisplay()
{
    // Setup SPI interface
    gpio_msp432_pin* lcd_cs = new gpio_msp432_pin(PORT_PIN(5, 0));
    spi_msp432* spi = new spi_msp432(EUSCI_B0_SPI, *lcd_cs);
    //spi->setSpeed(24000000);
    spi->setSpeed(240000000);

    // Setup LCD driver
    gpio_msp432_pin* lcd_rst = new gpio_msp432_pin(PORT_PIN(5, 7));
    gpio_msp432_pin* lcd_dc = new gpio_msp432_pin(PORT_PIN(3, 7));
    lcd = new st7735s_drv(*spi, *lcd_rst, *lcd_dc,
                          st7735s_drv::Crystalfontz_128x128);

    // Setup uGUI
    gui = new uGUI(*lcd);

    lcd->clearScreen(0x0);
    gui->SetForecolor(C_YELLOW);
    gui->FontSelect(&FONT_6X10);

    DrawOptions();
    SelectSong();
    lcd->clearScreen(0x0);
    gui->FontSelect(&FONT_4X6);

    gui->PutString(45, 53, "Have Fun!");
    char countdown[2];
    for (int i = 5; i > 0; i--)
    {
        sprintf(countdown, "%d", i);
        gui->PutString(61, 60, countdown);
        wait(1000);
    }
    lcd->clearScreen(0x0);
    DrawHeart();

    gui->PutString(4, 3, "Score: ");
    gui->PutString(4, 10, "Combo: ");
    char totalObjects[5];
    sprintf(totalObjects, "/%3d", infos->totalObjects);
    gui->PutString(18, 17, totalObjects);
}

void DrawSongInfo()
{
    //Song name and info
    int idx = 0;
    int objects = 0;
    int msLength = 0;
    CalculateDueTime(SongList[songSelection].song, SongList[songSelection].bpm);

    while (SongList[songSelection].song[idx].beatDivider != 0)
    {
        if (SongList[songSelection].song[idx].t != P && SongList[songSelection].song[idx].isPlayed)
        {
            objects++;
        }
        msLength = SongList[songSelection].song[idx].dueAtMS
                + SongList[songSelection].song[idx].durationMS;
        idx++;
    }

    infos->totalObjects = objects;

    char name[35];
    char objectCount[14];
    char duration[16];
    sprintf(name, "%-34s", SongList[songSelection].name);
    sprintf(objectCount, "Objects: %4d", objects);

    int minutes = msLength / 60000;
    int seconds = (msLength - minutes * 60000) / 1000;
    sprintf(duration, "Length: %dm %2ds", minutes, seconds);
    gui->PutString(4, 3, name);
    gui->PutString(4, 25, objectCount);
    gui->PutString(4, 36, duration);
}

void DrawOptions()
{
    DrawSongInfo();

    //Reaction time option
    gui->PutString(27, 54, "Reaction Time");
    char react[13];
    sprintf(react, "<- %4dms ->", objectSpeedMs);
    gui->PutString(30, 65, react);

    //Song speed option
    gui->PutString(33, 79, "Song Speed");
    int multiplicator = songSpeedMultiplicator;
    int multiplicatorDecimal = (songSpeedMultiplicator - multiplicator) * 10;
    char speed[11];
    sprintf(speed, "<- %d.%dx ->", multiplicator, multiplicatorDecimal);
    gui->PutString(33, 90, speed);

    //Random seed option
    gui->PutString(30, 105, "Random Seed");
    char sseed[11];
    sprintf(sseed, "<- %4d ->", seed);
    gui->PutString(33, 116, sseed);
}

void SelectObjectSpeed()
{
    gui->DrawFrame(1, 53, 126, 75, C_YELLOW);

    while (!IsJoystickPressed())
    {
        wait(300);
        Direction d = GetJoystickPosition();
        switch (d)
        {
        case up:
            gui->DrawFrame(1, 53, 126, 75, C_BLACK);
            SelectSong();
            return;
        case down:
            gui->DrawFrame(1, 53, 126, 75, C_BLACK);
            SelectSongSpeed();
            return;
        case left:
            if (objectSpeedMs > 500)
                objectSpeedMs -= 100;
            break;
        case right:
            if (objectSpeedMs < 3000)
                objectSpeedMs += 100;
            break;
        }

        char react[13];
        sprintf(react, "<- %4dms ->", objectSpeedMs);
        gui->PutString(30, 65, react);
    }
}

void SelectSongSpeed()
{
    gui->DrawFrame(1, 78, 126, 100, C_YELLOW);
    while (!IsJoystickPressed())
    {
        wait(300);
        Direction d = GetJoystickPosition();
        switch (d)
        {
        case up:
            gui->DrawFrame(1, 78, 126, 100, C_BLACK);
            SelectObjectSpeed();
            return;
        case down:
            gui->DrawFrame(1, 78, 126, 100, C_BLACK);
            SelectSeed();
            return;
        case left:
            if (songSpeedMultiplicator > 0.11)
            {
                songSpeedMultiplicator -= 0.1;
                DrawSongInfo();
            }
            break;
        case right:
            if (songSpeedMultiplicator < 2)
            {
                songSpeedMultiplicator += 0.1;
                DrawSongInfo();
            }
            break;
        }

        int multiplicator = songSpeedMultiplicator;
        int multiplicatorDecimal = round(
                (songSpeedMultiplicator - multiplicator) * 10);
        char speed[11];
        sprintf(speed, "<- %d.%dx ->", multiplicator, multiplicatorDecimal);
        gui->PutString(33, 90, speed);
    }
}

void SelectSeed()
{
    gui->DrawFrame(1, 104, 126, 126, C_YELLOW);

    while (!IsJoystickPressed())
    {
        wait(300);
        Direction d = GetJoystickPosition();
        switch (d)
        {
        case up:
            gui->DrawFrame(1, 104, 126, 126, C_BLACK);
            SelectSongSpeed();
            return;
        case down:
            gui->DrawFrame(1, 104, 126, 126, C_BLACK);
            SelectSong();
            return;
        case left:
            if (seed > 0)
            {
                seed -= 1;
            }
            break;
        case right:
            seed += 1;
            break;
        }

        char sseed[11];
        sprintf(sseed, "<- %4d ->", seed);
        gui->PutString(33, 116, sseed);
    }
}

void SelectSong()
{
    gui->DrawFrame(1, 0, 126, 52, C_YELLOW);

    while (!IsJoystickPressed())
    {
        wait(300);
        Direction d = GetJoystickPosition();
        switch (d)
        {
        case up:
            gui->DrawFrame(1, 0, 126, 52, C_BLACK);
            SelectSeed();
            return;
        case down:
            gui->DrawFrame(1, 0, 126, 52, C_BLACK);
            SelectObjectSpeed();
            return;
        case left:
            if (songSelection > 0)
            {
                songSelection -= 1;
                DrawSongInfo();
            }
            break;
        case right:
            if (songSelection < songCount - 1)
            {
                songSelection += 1;
                DrawSongInfo();
            }
            break;
        }
    }

}

void DrawArrow(int x, int y, Direction d, bool erase)
{
    auto color = erase ? C_BLACK : C_RED;

    switch (d)
    {
    case up:
        gui->FillFrame(x, y, x - 1, y - 1, color);
        break;
    case down:
        gui->FillFrame(x, y, x - 1, y + 1, color);
        break;
    case left:
        gui->FillFrame(x, y, x - 1, y - 1, color);
        break;
    case right:
        gui->FillFrame(x, y, x + 1, y - 1, color);
        break;
    }
}

void DrawGlitter(int x, int y, bool erase)
{
    auto color = erase ? C_BLACK : C_YELLOW;

    gui->DrawPixel(x, y, color);
    gui->DrawPixel(x + 2, y, color);
    gui->DrawPixel(x, y + 3, color);
    gui->DrawPixel(x + 5, y + 1, color);
    gui->DrawPixel(x + 2, y + 2, color);
    gui->DrawPixel(x + 4, y + 4, color);
    gui->DrawPixel(x, y + 4, color);
    gui->DrawPixel(x + 1, y + 2, color);
    gui->DrawPixel(x + 2, y, color);
    gui->DrawPixel(x, y + 3, color);
}

void DrawLine(int x1, int y1, int x2, int y2, bool erase)
{
    auto color = erase ? C_BLACK : C_RED;

    gui->DrawLine(x1, y1, x2, y2, color);
}

void DrawHeart()
{
    uGUI::BMP bmp;
    bmp.height = 11;
    bmp.width = 11;
    bmp.p = heart;
    bmp.bpp = 16;
    bmp.colors = BMP_RGB565;
    gui->DrawBMP(58, 58, &bmp);
}

bool IsJoystickPressed()
{
    gpio_msp432_pin button(PORT_PIN(4, 1));
    button.gpioMode(GPIO::INPUT | GPIO::PULLUP);

    return !button.gpioRead();
}

Direction GetJoystickPosition()
{
    adc14_msp432_channel joy_X(15);
    adc14_msp432_channel joy_Y(9);

    joy_X.adcMode(ADC::ADC_10_BIT);
    int x_raw = joy_X.adcReadRaw();
    double x_pos = (x_raw - 512) / 512.0;

    joy_Y.adcMode(ADC::ADC_10_BIT);
    int y_raw = joy_Y.adcReadRaw();
    double y_pos = ((y_raw - 512) / 512.0) * (-1);

    //Joystick is not idle
    if ((abs(x_pos) > 0.3 || abs(y_pos) > 0.3))
    {
        //Left or right triggered
        if (abs(x_pos) >= abs(y_pos))
        {
            //Left
            if (x_pos < 0)
            {
                return left;
            }
            //Right
            else
            {
                return right;
            }
        }

        //Up or down triggered
        else
        {
            //Up
            if (y_pos < 0)
            {
                return up;
            }
            //Down
            else
            {
                return down;
            }
        }
    }

    //Joystick is idle
    return idle;
}

void DrawScore()
{
    char scoreAsString[6];
    sprintf(scoreAsString, "%5d", infos->score);
    gui->PutString(36, 3, scoreAsString);
}

void DrawCombo()
{
    char comboAsString[5];
    sprintf(comboAsString, "%3dx", infos->combo);
    gui->PutString(36, 10, comboAsString);
}

void DrawAccuracy()
{
    char accuracyString[4];
    sprintf(accuracyString, "%3d", infos->hits);
    gui->PutString(4, 17, accuracyString);
}

void DrawShield(Direction d, UG_COLOR c)
{
    switch (d)
    {
    case up:
        gui->DrawLine(53, 53, 73, 53, c);
        break;
    case down:
        gui->DrawLine(53, 73, 73, 73, c);
        break;
    case left:
        gui->DrawLine(53, 53, 53, 73, c);
        break;
    case right:
        gui->DrawLine(73, 53, 73, 73, c);
        break;
    default:
        break;
    }
}

int lastTimems = 0;
bool mustChangeDirection = false;
void NextTick(int timems)
{
    bool updateUI = false;

    Direction joystick = GetJoystickPosition();

    if (joystick != infos->shieldDirection)
    {
        DrawShield(joystick, C_BLUE);
        DrawShield(infos->shieldDirection, C_BLACK);
        infos->shieldDirection = joystick;
        mustChangeDirection = false;
    }

    //Move and draw all objects
    for (int i = 0; i < 20; i++)
    {
        if (gameObjects[i] != nullptr)
        {
            gameObjects[i]->move(
                    timems - lastTimems,
                    mustChangeDirection ? idle : infos->shieldDirection);
            if (gameObjects[i]->state != undetermined)
            {
                if (gameObjects[i]->state == hit)
                {
                    updateUI = true;
                    infos->score += 1 * ++infos->combo;
                    infos->hits++;
                    SuccessObject* s = new SuccessObject(
                            gameObjects[i]->direction, 200);

                    if (gameObjects[i]->direction == infos->shieldDirection)
                    {
                        mustChangeDirection = true;
                        DrawShield(infos->shieldDirection, C_GREEN);
                    }
                }
                else if (gameObjects[i]->state == missed)
                {
                    if (infos->combo != 0)
                        updateUI = true;
                    infos->misses++;
                    infos->combo = 0;
                }
                delete gameObjects[i];
                gameObjects[i] = nullptr;
            }
        }
    }

    if (updateUI)
    {
        DrawScore();
        DrawCombo();
        DrawAccuracy();
    }

    lastTimems = timems;
}
