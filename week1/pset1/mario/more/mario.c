#include <cs50.h>
#include <stdio.h>

int main(void)
{
    // Declare the height
    int height;
    //Ask the user for the heiht of the pyramide and check if the value
    // is greater than zero
    do
    {
        height = get_int("Height: ");
        if (height < 1 || height > 8)
        {
            printf("The height must be an integer from 1 to 8\n");
        }
    }
    while (height < 1 || height > 8);
    // Declare the numer of initial gaps (spaces) for formatting
    int gaps = height - 1;
    // The foor loop responsible for the height
    for (int i = 0; i < height; i++)
    {
        //The loops responsible for the weight of the pyramid

        //The initial spaces (for formatting)
        for (int s = gaps; s > 0; s--)
        {
            printf(" ");
        }
        // Discount minus one space for the next loop
        gaps --;
        // The first half of hashes
        for (int j = 0; j < i + 1; j++)
        {
            printf("#");
        }
        // The gap between them
        printf("  ");
        //The second half of hashes
        for (int j = 0; j < i + 1; j++)
        {
            printf("#");
        }
        // Jump to the next line
        printf("\n");
    }
}