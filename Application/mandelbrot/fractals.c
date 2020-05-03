/*
 * fractals.c
 *
 *  Created on: May 3, 2020
 *      Author: icis4
 */
#include <stdio.h>
#include "FreeRTOS.h"
#include "cmsis_os.h"
#include "bsp/stm32746g_discovery_lcd.h"

static uint32_t colors[] = {
	LCD_COLOR_BLACK,
	LCD_COLOR_DARKBLUE,
	LCD_COLOR_DARKGREEN,
	LCD_COLOR_DARKCYAN,
	LCD_COLOR_DARKRED,
	LCD_COLOR_DARKMAGENTA,
	LCD_COLOR_DARKYELLOW,
	LCD_COLOR_DARKGRAY,
	LCD_COLOR_BLUE,
	LCD_COLOR_GREEN,
	LCD_COLOR_CYAN,
	LCD_COLOR_RED,
	LCD_COLOR_MAGENTA,
	LCD_COLOR_YELLOW,
	LCD_COLOR_BROWN,
	LCD_COLOR_ORANGE,
	LCD_COLOR_GRAY,
	LCD_COLOR_LIGHTBLUE,
	LCD_COLOR_LIGHTGREEN,
	LCD_COLOR_LIGHTCYAN,
	LCD_COLOR_LIGHTRED,
	LCD_COLOR_LIGHTMAGENTA,
	LCD_COLOR_LIGHTYELLOW,
	LCD_COLOR_LIGHTGRAY,
	LCD_COLOR_WHITE,
};

#define MAXCOUNT sizeof(colors)

// Function to draw mandelbrot set
void fractal(float left, float top, float xside, float yside)
{
    float xscale, yscale, zx, zy, cx, tempx, cy;
    int x, y;
    int maxx, maxy, count;

    // getting maximum value of x-axis of screen
    maxx = BSP_LCD_GetXSize() - 1;

    // getting maximum value of y-axis of screen
    maxy = BSP_LCD_GetYSize() - 1;

    // setting up the xscale and yscale
    xscale = xside / maxx;
    yscale = yside / maxy;

    // calling rectangle function
    // where required image will be seen
    BSP_LCD_SetTextColor(LCD_COLOR_RED);
    BSP_LCD_DrawRect(0, 0, maxx, maxy);
//    BSP_LCD_DrawHLine(0, 0, maxx + 1);

    // scanning every point in that rectangular area.
    // Each point represents a Complex number (x + yi).
    // Iterate that complex number
    for (y = 0; y <= maxy ; y++) {
        for (x = 0; x <= maxx; x++)
        {
            // c_real
            cx = x * xscale + left;

            // c_imaginary
            cy = y * yscale + top;

            // z_real
            zx = 0;

            // z_imaginary
            zy = 0;
            count = 0;

            // Calculate whether c(c_real + c_imaginary) belongs
            // to the Mandelbrot set or not and draw a pixel
            // at coordinates (x, y) accordingly
            // If you reach the Maximum number of iterations
            // and If the distance from the origin is
            // greater than 2 exit the loop
            while ((zx * zx + zy * zy < 4) && (count < MAXCOUNT))
            {
                // Calculate Mandelbrot function
                // z = z*z + c where z is a complex number

                // tempx = z_real*_real - z_imaginary*z_imaginary + c_real
                tempx = zx * zx - zy * zy + cx;

                // 2*z_real*z_imaginary + c_imaginary
                zy = 2 * zx * zy + cy;

                // Updating z_real = tempx
                zx = tempx;

                // Increment count
                count = count + 1;
            }

            // To display the created fractal
            // putpixel(x, y, count);
            BSP_LCD_DrawPixel(x, y, colors[count]);
        }
    }
}



void test_fractal() {
	char sbuff[200];
	float left, top, xside, yside;

// setting the left, top, xside and yside
// for the screen and image to be displayed
	left = -1.75;
	top = -0.25;
	xside = 0.45;
	yside = 0.45;
	uint32_t time = osKernelGetTickCount();
	fractal(left, top, xside, yside);
	sprintf(sbuff,"  C Mandelbrot time: %ldms", osKernelGetTickCount() - time);
	BSP_LCD_SetTextColor(LCD_COLOR_WHITE);
	BSP_LCD_SetBackColor(LCD_COLOR_BLACK);
	BSP_LCD_DisplayStringAtLine(1, (uint8_t*)sbuff);
	sprintf(sbuff,"  https://www.geeksforgeeks.org/fractals-in-cc/");
	BSP_LCD_DisplayStringAtLine(2, (uint8_t*)sbuff);
}
