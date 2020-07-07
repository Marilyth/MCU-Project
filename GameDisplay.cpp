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
#include <Utils/BaseObject.h>
#include <stdlib.h>     /* srand, rand */
#include "math.h"
#include "gpio_msp432.h"
#include "timer_msp432.h"
#include "spi_msp432.h"
#include "adc14_msp432.h"
#include "st7735s_drv.h"
#include "uGUI.h"
#include "uGUI_colors.h"
#include "font_4x6.h"

st7735s_drv* lcd = nullptr;
uGUI* gui = nullptr;
Position* playerPos[15];
BaseObject* gameObjects[100];

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
    gui->PutString(4, 3, "Move the joystick!");
    gui->DrawFrame(54, 54, 74, 74, C_YELLOW);
}

void DrawObject(int x, int y)
{
    gui->DrawPixel(x, y, C_RED);
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
    gui->DrawPixel(x, y, C_BLACK);
}

int triggeredAt = 0;
bool DrawShieldSimple(bool activate)
{
    adc14_msp432_channel joy_X(15);
    adc14_msp432_channel joy_Y(9);

    joy_X.adcMode(ADC::ADC_10_BIT);
    int x_raw = joy_X.adcReadRaw();
    double x_pos = (x_raw - 512) / 512.0;

    joy_Y.adcMode(ADC::ADC_10_BIT);
    int y_raw = joy_Y.adcReadRaw();
    double y_pos = ((y_raw - 512) / 512.0) * (-1);

    if ((abs(x_pos) > 0.3 || abs(y_pos) > 0.3))
    {
        if (!activate)
            return true;

        //Left or right triggered
        if (abs(x_pos) >= abs(y_pos))
        {
            if (x_pos < 0)
                gui->DrawLine(54, 54, 54, 74, C_BLUE);
            else
                gui->DrawLine(74, 54, 74, 74, C_BLUE);
        }

        //Up or down triggered
        else
        {
            if (y_pos < 0)
                gui->DrawLine(54, 54, 74, 54, C_BLUE);
            else
                gui->DrawLine(54, 74, 74, 74, C_BLUE);
        }

        return true;
    }

    return false;
}

static const double PI = 3.1415926535;
void DrawShield(int radius)
{
    adc14_msp432_channel joy_X(15);
    adc14_msp432_channel joy_Y(9);

    joy_X.adcMode(ADC::ADC_10_BIT);
    int x_raw = joy_X.adcReadRaw();
    double x_pos = (x_raw - 512) / 512.0;

    joy_Y.adcMode(ADC::ADC_10_BIT);
    int y_raw = joy_Y.adcReadRaw();
    double y_pos = ((y_raw - 512) / 512.0) * (-1);

    int cur_x = 64 + (int) (x_pos * 10);
    int cur_y = 64 + (int) (y_pos * 10);

    double length = sqrt(y_pos * y_pos + x_pos * x_pos);
    double starting_angle = (acos((x_pos) / length) * 180 / PI);
    if (y_pos < 0)
        starting_angle *= -1;
    int index = 0;
    for (int angle = starting_angle - 7; angle < starting_angle + 7; angle++)
    {
        int x_circ = 64 + radius * cos(angle * PI / 180);
        int y_circ = 64 + radius * sin(angle * PI / 180);
        gui->DrawPixel(x_circ, y_circ, C_YELLOW);
        if (playerPos[index]->x != x_circ || playerPos[index]->y != y_circ)
        {
            gui->DrawPixel(playerPos[index]->x, playerPos[index]->y, C_BLACK);
            playerPos[index]->x = x_circ;
            playerPos[index]->y = y_circ;
        }
    }
}

bool isTriggered = false;
int lastTimems = 0;
void NextTick(int timems)
{
    //Draw Player
    if (isTriggered && !DrawShieldSimple(false))
    {
        isTriggered = false;
    }

    if (timems - triggeredAt > 200)
    {
        gui->DrawFrame(54, 54, 74, 74, C_BLACK);
        if (!isTriggered && DrawShieldSimple(true))
        {
            triggeredAt = timems;
            isTriggered = true;
        }
    }

    //Move and draw all objects
    for (int i = 0; i < 100; i++)
    {
        if (gameObjects[i] != nullptr)
        {
            gameObjects[i]->move(timems - lastTimems);
            if (gameObjects[i]->isDone())
            {
                delete gameObjects[i];
                gameObjects[i] = nullptr;
            }
        }
    }

    lastTimems = timems;
}
