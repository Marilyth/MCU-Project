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
#include <GameDisplay.h>
#include <Utils/GameInformation.h>
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
#include "yahal_String.h"
#include <string>

st7735s_drv* lcd = nullptr;
uGUI* gui = nullptr;
GameInformation* infos = new GameInformation();
BaseObject* gameObjects[20];

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
    gui->FontSelect(&FONT_4X6);
    gui->PutString(4, 3, "Score: ");
    gui->PutString(4, 10, "Combo: ");
}

void DrawObject(int x, int y)
{
    gui->FillFrame(x, y, x + 1, y + 1, C_RED);
}

void DrawLine(int x1, int y1, int x2, int y2)
{
    gui->DrawLine(x1, y1, x2, y2, C_RED);
}

void EraseLine(int x1, int y1, int x2, int y2)
{
    gui->DrawLine(x1, y1, x2, y2, C_BLACK);
}

void EraseObject(int x, int y)
{
    gui->FillFrame(x, y, x + 1, y + 1, C_BLACK);
}

Direction GetShieldPosition()
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
                gui->DrawLine(54, 54, 54, 74, C_BLUE);
            }
            //Right
            else
            {
                return right;
                gui->DrawLine(74, 54, 74, 74, C_BLUE);
            }
        }

        //Up or down triggered
        else
        {
            //Up
            if (y_pos < 0)
            {
                return up;
                gui->DrawLine(54, 54, 74, 54, C_BLUE);
            }
            //Down
            else
            {
                return down;
                gui->DrawLine(54, 74, 74, 74, C_BLUE);
            }
        }
    }

    //Joystick is idle
    return idle;
}

void DrawScore()
{
    char scoreAsString[7];
    sprintf(scoreAsString, "%d", infos->score);
    gui->PutString(36, 3, scoreAsString);
}

void DrawCombo()
{
    char comboAsString[4];
    sprintf(comboAsString, "%dx", infos->combo);
    gui->PutString(36, 10, comboAsString);
}

void DrawAccuracy()
{
    char accuracyString[7];
    double accuracy = 100
            * ((double) infos->hits / (infos->hits + infos->misses));
    sprintf(accuracyString, "%03d%%", (int) accuracy);
    gui->PutString(36, 17, accuracyString);
}

void DrawShield(Direction d, UG_COLOR c)
{
    switch (d)
    {
    case up:
        gui->DrawLine(54, 54, 74, 54, c);
        break;
    case down:
        gui->DrawLine(54, 74, 74, 74, c);
        break;
    case left:
        gui->DrawLine(54, 54, 54, 74, c);
        break;
    case right:
        gui->DrawLine(74, 54, 74, 74, c);
        break;
    default:
        break;
    }
}

int lastTimems = 0;
bool mustChangeDirection = false;
void NextTick(int timems)
{
    Direction joystick = GetShieldPosition();

    if (joystick != infos->shieldDirection)
    {
        DrawShield(joystick, C_BLUE);
        DrawShield(infos->shieldDirection, C_BLACK);
        infos->shieldDirection = joystick;
        mustChangeDirection = false;
    }

    //Move and draw all objects
    for (int i = 0; i < 5; i++)
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
                    infos->score += 1 * ++infos->combo;
                    infos->hits++;
                    if (gameObjects[i]->direction == infos->shieldDirection)
                        mustChangeDirection = true;
                }
                else
                {
                    if (infos->combo != 0)
                    {
                        infos->combo = 0;
                        infos->misses++;
                    }
                }

                delete gameObjects[i];
                gameObjects[i] = nullptr;
            }
        }
    }

    DrawScore();
    DrawCombo();
    DrawAccuracy();

    lastTimems = timems;
}
