#include <cs50.h>
#include <stdio.h>
/*
    TODO:
    - Prompt for input
    - Calculate the checksum
    - Check for card lenght and starting digits
    - Print AMEX, MASTERCARD, VISA or INVALID
*/
int main(void)
{
    long number;

    do
    {
        number = get_long("Number: ");
        if (number < 0)
        {
            printf("The number must be an integer!\n");
        }
    }
    while (number < 0);

    int last = 0, sec_last = 0, digits = 0, sum_of_odds = 0, sum_of_evens = 0, multiplier = 0, n1 = 0, n2 = 0;

    // The number will decrease until it becomes a float type number, so the result of 'number / 10' will
    // be zero, since, for instance, with integers, 1 / 10 = 0
    while (number > 0)
    {
        // We are "reading" the number from right to left, so the first number is the last one, and so on.
        digits ++;

        //Store the second last digit for the end
        sec_last = last;

        // This gives us the last digit of the number
        last = number % 10;

        // The second-to-last digits (i.e, the second, forth, sixth... from right to left)
        if (digits % 2 == 0)
        {
            multiplier = last * 2;
            if (multiplier >= 10)
            {
                n1 = multiplier / 10;
                n2 = multiplier % 10;
                multiplier = n1 + n2;
            }
            sum_of_evens += multiplier;
        }
        else // So we have the odd numbers, i.e., the first, third, fifth, etc, from right to left
        {
            sum_of_odds += last;
        }
        // Now that we have the last digit, lets reduce the number by one digit (e.g. 123 becomes 12)
        number /= 10;
    }

    //Sometimes its required to know the 'firsts' digits of the whole number, i.e the first and the second one
    // from left to right
    int firsts = (last * 10) + sec_last;

    //Sums the sums
    int total_sum = sum_of_evens + sum_of_odds;
    //That final sum must have the last digit as been a zero, i.e 10, 50 or 80 would be a valid value, whereas 89 not
    if (total_sum % 10 == 0)
    {
        if ((digits == 13 || digits == 16) && last == 4)
        {
            printf("VISA\n");
        }
        else if (digits == 15 && (firsts == 34 || firsts == 37))
        {
            printf("AMEX\n");
        }
        else if (digits == 16 && (firsts >= 51 && firsts <= 55))
        {
            printf("MASTERCARD\n");
        }
        else
        {
            printf("INVALID\n");
        }
    }
    else
    {
        printf("INVALID\n");
    }
}