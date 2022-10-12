#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include "helpers.h"

// Prototypes
int cut(int color_number);

// Convert image to grayscale
void grayscale(int height, int width, RGBTRIPLE image[height][width])
{
    int avg = 0;
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            // Calculate the average of the RBG values of a pixel and round it to the nearest integer
            avg = round((image[i][j].rgbtRed
                         + image[i][j].rgbtGreen
                         + image[i][j].rgbtBlue) / 3.0);
            // Apply that average to each RBG value of the pixel
            image[i][j].rgbtRed = avg;
            image[i][j].rgbtGreen = avg;
            image[i][j].rgbtBlue = avg;
        }
    }
    return;
}

// Make sure that the maximum value for a pixel (225) wasn't exceeded (cut it)
int cut(int color)
{
    if (color > 255)
    {
        color = 255;
    }

    return color;
}

// Convert image to sepia
void sepia(int height, int width, RGBTRIPLE image[height][width])
{
    float sepiaRed, sepiaGreen, sepiaBlue;
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            // Calculate the respective amount of sepia red, sepia green and sepia blue
            int originalRed = image[i][j].rgbtRed;
            int originalGreen = image[i][j].rgbtGreen;
            int originalBlue = image[i][j].rgbtBlue;

            // Let's put the actual original values into the formula
            sepiaRed = cut(round(.393 * originalRed + .769 * originalGreen + .189 * originalBlue));
            sepiaGreen = cut(round(.349 * originalRed + .686 * originalGreen + .168 * originalBlue));
            sepiaBlue = cut(round(.272 * originalRed + .534 * originalGreen + .131 * originalBlue));

            // Apply those amount of colors to their original values, i.e., convert each RGB value to its sepia value
            image[i][j].rgbtRed = sepiaRed;
            image[i][j].rgbtGreen = sepiaGreen;
            image[i][j].rgbtBlue = sepiaBlue;
        }
    }
    return;
}

// Reflect image horizontally
void reflect(int height, int width, RGBTRIPLE image[height][width])
{
    // Set the 'index' and the 'reflected_index'
    /*
        An example of this would be:
        [0] [1] [2] [3]

        Note that if [0] is the 'index', [3] is its 'reflected_index'
        The same goes for [1], which is our 'index', and for [2], which is our 'reflected_index'
    */
    int index = 0, reflected_index = 0;
    int n = 0;
    // Set the temporaries values (which we'll use in swap)
    int tmp_red = 0, tmp_green = 0, tmp_blue = 0;
    n = width / 2; // The number of swaps we'll implement is the half of the width
    for (int i = 0; i < height; i++)
    {
        // At the beginning of each iteration on the rows, our 'reflected_index' will be the last value of the row
        reflected_index = width - 1;
        index = 0; // Make sure that at each iteration, the index backs to zero
        // Let's go through all values of the row
        for (int j = 0; j < n; j++) // Also declare 'index' as the same of 'j'
        {
            // Swap the items
            /*
            For instance, if we have 4 items:

              1st swap  [0] [1] [2] [3]
                         ^           ^
                        [3] [1] [2] [0]

              2nd swap  [3] [1] [2] [0]
                             ^   ^
                        [3] [2] [1] [0]
            */
            // (notice that 'i' doesn't change, because we're touching each row at the time)
            tmp_red = image[i][index].rgbtRed;
            tmp_green = image[i][index].rgbtGreen;
            tmp_blue = image[i][index].rgbtBlue;

            image[i][index].rgbtRed = image[i][reflected_index].rgbtRed;
            image[i][index].rgbtGreen = image[i][reflected_index].rgbtGreen;
            image[i][index].rgbtBlue = image[i][reflected_index].rgbtBlue;

            image[i][reflected_index].rgbtRed = tmp_red;
            image[i][reflected_index].rgbtGreen = tmp_green;
            image[i][reflected_index].rgbtBlue = tmp_blue;

            index ++; // The 'index' goes forwards, i.e., in the same way 'j' goes
            reflected_index --; // The 'reflected_index' goes backwards
        }
    }
    return;
}

RGBTRIPLE blurPixel(int height, int width, int i, int j, RGBTRIPLE copy[height][width]);

// Blur image
void blur(int height, int width, RGBTRIPLE image[height][width])
{
    int i = 0, j = 0;
    RGBTRIPLE blurred_pixel;
    // Make a copy of the image
    RGBTRIPLE copy[height][width];
    // Fill that copy with the image RGB values
    for (i = 0; i < height; i++)
    {
        for (j = 0; j < width; j++)
        {
            copy[i][j] = image[i][j];
        }
    }
    // Iterates through the image
    for (i = 0; i < height; i++)
    {
        for (j = 0; j < width; j++)
        {
            // Calculate the blurred pixel given the color (a RGBTRIPLE value), the height, width, the i and j indexes and the image's copy itself
            // Once finished, apply that blurred pixel to the original pixel (that's why we created a copy, in the first place)
            image[i][j] = blurPixel(height, width, i, j, copy);
        }
    }
    return;
}

RGBTRIPLE blurPixel(int height, int width, int i, int j, RGBTRIPLE copy[height][width])
{
    RGBTRIPLE blurred_pixel;
    int red_sum = 0, green_sum = 0, blue_sum = 0; // The sums of each RGB value
    double count = 0.0; // Our counter
    /*
        Let's draw a 3x3 "block" to represent the RELATIVE positions according to the values of 'i' and 'j'
        +--------+-------+-------+
        | -1, -1 | -1, 0 | -1, 1 |
        +--------+-------+-------+
        | 0 , -1 | 0 , 0 | 0 , 1 |
        +--------+-------+-------+
        | 1 , -1 | 1 , 0 | 1 , 1 |
        +--------+-------+-------+
        So, if i = 2 and j = 3, the coordinates of the very first "cell" of the block will be
        actualX = 2 + (-1) = 1 and actualY = 3 + (-1) = 2, as per bellow:
        +-------+ ...
        | 1 , 2 | ...
        +-------+ ...
           ...
    */
    int actualX, actualY;
    // Then, let's iterate throught the relative values
    for (int x = -1; x < 2; x++)
    {
        for (int y = -1; y < 2; y++)
        {
            // Define the actual coordinate of each "cell" in the "block"
            actualX = i + x;
            actualY = j + y;
            // The most inside parentheses check if the coordinates are invalid, i.e, if they are false
            // The exclamation point (!) negates the expression among the parentheses, i.e, makes it true if the coordinates are valid
            if (!(actualX < 0 || actualY < 0 || actualX > height - 1 || actualY > width - 1))
            {
                // Sums the actual values of red, green and blue
                red_sum += copy[actualX][actualY].rgbtRed;
                green_sum += copy[actualX][actualY].rgbtGreen;
                blue_sum += copy[actualX][actualY].rgbtBlue;
                // For each valid "cell", add 1 to count
                count ++;
            }
        }
    }
    // Once calculated the sums of the red, green and blue (of the neighboring "cells"), apply the avarage of them into a pixel (our blurred pixel)
    blurred_pixel.rgbtRed = cut(round((red_sum) / count));
    blurred_pixel.rgbtGreen = cut(round((green_sum) / count));
    blurred_pixel.rgbtBlue = cut(round((blue_sum) / count));

    return blurred_pixel;
}