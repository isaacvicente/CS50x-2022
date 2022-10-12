#include <stdio.h>
#include <stdlib.h>
#include <math.h>
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

typedef struct
{
    int Gx;
    int Gy;
}
KERNEL;

RGBTRIPLE edgePixel(int height, int width, int i, int j, RGBTRIPLE copy[height][width]);

// Detect edges
void edges(int height, int width, RGBTRIPLE image[height][width])
{
    // The resulting pixel after the edge operation, so to speak
    RGBTRIPLE edge_pixel;
    int i, j; // Our counters
    // Make a copy to work with without change the original image
    RGBTRIPLE copy[height][width];
    // Copy each image's pixel to the copy's ones
    for (i = 0; i < height; i++)
    {
        for (j = 0; j < width; j++)
        {
            copy[i][j] = image[i][j];
        }
    }
    // Let's iterate by all the image and apply the filter to each pixel at time
    for (i = 0; i < height; i++)
    {
        for (j = 0; j < width; j++)
        {
            image[i][j] = edgePixel(i, j, height, width, copy);
        }
    }
    return;
}

RGBTRIPLE edgePixel(int i, int j, int height, int width, RGBTRIPLE copy[height][width])
{
    // The final pixel we'll return
    RGBTRIPLE edged_pixel;
    int Gx[3][3] = {{-1, 0, 1}, {-2, 0, 2}, {-1, 0, 1}}, Gy[3][3] = {{-1, -2, -1}, {0, 0, 0}, {1, 2, 1}};
    // Create the red, green and blue variables sums with the type KERNEL (with Gx and Gy built-in)
    KERNEL red_sum, green_sum, blue_sum;
    // Initialize all of the values to zero at the start
    red_sum.Gx = 0, red_sum.Gy = 0, green_sum.Gx = 0, green_sum.Gy = 0, blue_sum.Gx = 0, blue_sum.Gy = 0;
    /*
        Let's draw a 3x3 "block" to represent the RELATIVE positions according to the values of 'i' and 'j'
        (the approach here is very similar to the blur's implementation, but don't confuse actualX with Gx: they're different things)
         X
         ^
         |
         |  +--------+-------+-------+
         |  | -1, -1 | -1, 0 | -1, 1 |
         |  +--------+-------+-------+
         |  | 0 , -1 | 0 , 0 | 0 , 1 |
         |  +--------+-------+-------+
         |  | 1 , -1 | 1 , 0 | 1 , 1 |
         |  +--------+-------+-------+
         |
       --+----------------------------------> Y
         |

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
            // The coordinates are defined by the x's and y's axis, as seen in the example above.
            actualX = i + x;
            actualY = j + y;
            /*
                       Gx                          Gy
                +----+----+----+            +----+----+----+
                | -1 | +0 | +1 |            | -1 | -2 | -1 |
                +----+----+----+            +----+----+----+
                | -2 | +0 | +2 |            | +0 | +0 | +0 |
                +----+----+----+            +----+----+----+
                | -1 | +0 | +1 |            | +1 | +2 | +1 |
                +----+----+----+            +----+----+----+
            */
            // If some "cell" is beyond the border, it's Gx and Gy values, for each color sum, should be zero, i.e., be ignored
            // So, by negating the beyond-borders cases, we have the valid ones, i.e., the non-beyond-borders cases
            int kernel_i = x + 1, kernel_j = y + 1;
            if (!(actualX < 0 || actualY < 0 || actualX > height - 1 || actualY > width - 1))
            {
                // Once we discovered by what multiply both Gx and Gy, for each RGB color, let's apply the sum
                red_sum.Gx += Gx[kernel_i][kernel_j] * copy[actualX][actualY].rgbtRed;
                red_sum.Gy += Gy[kernel_i][kernel_j] * copy[actualX][actualY].rgbtRed;

                green_sum.Gx += Gx[kernel_i][kernel_j] * copy[actualX][actualY].rgbtGreen;
                green_sum.Gy += Gy[kernel_i][kernel_j] * copy[actualX][actualY].rgbtGreen;

                blue_sum.Gx += Gx[kernel_i][kernel_j] * copy[actualX][actualY].rgbtBlue;
                blue_sum.Gy += Gy[kernel_i][kernel_j] * copy[actualX][actualY].rgbtBlue;
            }
        }
    }
    // Once calculated the sums of the red, green and blue, for the Gx and Gy kernels (of the neighboring "cells"), apply the formula square_root_of(Gx² + Gy²)
    edged_pixel.rgbtRed = cut(round(sqrt(pow(red_sum.Gx, 2) + pow(red_sum.Gy, 2))));
    edged_pixel.rgbtGreen = cut(round(sqrt(pow(green_sum.Gx, 2) + pow(green_sum.Gy, 2))));
    edged_pixel.rgbtBlue = cut(round(sqrt(pow(blue_sum.Gx, 2) + pow(blue_sum.Gy, 2))));
    // Return the "edged" pixel, with all effects applied to it
    return edged_pixel;
}