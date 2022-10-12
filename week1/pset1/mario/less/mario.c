#include <cs50.h>
#include <stdio.h>

int main(void)
{
    int height;
    // Ensure that the user type a positive value between 1 and 8, inclusive
    do
    {
        height = get_int("Height: ");
    }
    while (height <= 0 || height > 8);

    int n = height;

    for (int i = 0; i < height; i++)
    {
        // Print the spaces
        for (int s = n - 1; s > 0; s--)
        {
            printf(" ");
        }
        // Print the hashes
        for (int j = i + 1; j > 0; j--)
        {
            printf("#");
        }
        // The number of spaces decreases at each row
        n --;
        // In the end of the internal loop, jump to the next line
        printf("\n");
    }
}