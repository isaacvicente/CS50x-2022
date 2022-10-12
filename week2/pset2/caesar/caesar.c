#include <cs50.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

// Prototypes
bool only_digits(string arg);
char rotate(char letter, long key);

int main(int argc, string argv[])
{
    long key;
    // Make sure that the user is just typing only one argument and
    if (argc == 2) // The actual first argument is the invocation itself
    {
        // Garantee that the first argument is a positive integer
        if (only_digits(argv[1]))
        {
            // Then, we have to convert the argument the user typed (a string value) to its actual integer value
            key = atoi(argv[1]);

            string text = get_string("Plaintext:  ");

            for (int i = 0, n = strlen(text); i < n; i++)
            {
                text[i] = rotate(text[i], key);
            }
            printf("Ciphertext: %s\n", text);
        }
        else
        {
            printf("Usage: ./caesar key\n");
            return 1;
        }
    }
    else
    {
        printf("Usage: ./caesar key\n");
        return 1;
    }
}

bool only_digits(string arg)
{
    bool value = false;
    for (int i = 0, n = strlen(arg); i < n; i++)
    {
        if (isdigit(arg[i]))
        {
            value = true;
        }
        else
        {
            value = false;
            return value;
        }
    }
    return value;
}

char rotate(char letter, long key)
{
    int n;
    int remainder = 0;
    // The value of the key could be a number higher than 26
    // If so, we have to take the rest of key / 26, i.e., how much we have to transform one char into another
    if (key >= 26)
    {
        n = key % 26;
    }
    // If not, the value of the key is valid and we should continue
    else
    {
        n = key;
    }

    // The letter could be capitalized (A...Z)
    if ((letter >= 65 && letter <= 90))
    {
        // The letter could overpass the (capital) Z (90) in the maximum of 25 units (115)
        if (letter + n > 90 && letter + n < 116)
        {
            remainder = (letter + n) - 90;
            letter = 64 + remainder;
            return letter;
        }
        else
        {
            letter += n;
            return letter;
        }
    }
    // Or the letter could be a lowercase one (a...z)
    else if (letter >= 97 && letter <= 122)
    {
        // Also, the letter could overpass the (lowercase) z (122)
        if (letter + n > 122)
        {
            remainder = (letter + n) - 122;
            letter = 96 + remainder;
            return letter;
        }
        else
        {
            letter += n;
            return letter;
        }
    }
    // If the "letter" is not a letter (either capitalized or not), so, we should return that value itself, i.e., don't change it.
    else
    {
        return letter;
    }
}