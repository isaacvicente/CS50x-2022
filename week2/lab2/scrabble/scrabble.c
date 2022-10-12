#include <ctype.h>
#include <cs50.h>
#include <stdio.h>
#include <string.h>

// Points assigned to each letter of the alphabet
int POINTS[] = {1, 3, 3, 2, 1, 4, 2, 4, 1, 8, 5, 1, 3, 1, 1, 3, 10, 1, 1, 1, 1, 4, 4, 8, 4, 10};

int compute_score(string word);

int main(void)
{
    // Get input words from both players
    string word1 = get_string("Player 1: ");
    string word2 = get_string("Player 2: ");

    // Score both words
    int score1 = compute_score(word1);
    int score2 = compute_score(word2);

    // Print the winner
    if (score1 > score2)
    {
        printf("Player 1 wins!\n");
    }
    else if (score2 > score1)
    {
        printf("Player 2 wins!\n");
    }
    else
    {
        printf("Tie!\n");
    }
}

int compute_score(string word)
{
    // Compute and return score for string
    // Define the length of the word
    int lenght = strlen(word), i = 0, score = 0;

    // Itirates over every character of the word
    for (i = 0; i < lenght; i++)
    {
        // Checks if each character are an uppercase one, and if so, converts it to lowercase
        if (isupper(word[i]))
        {
            word[i] = tolower(word[i]);
        }
        // Converts a = 0, b = 1, c = 2 ...
        int n = word[i] - 97;
        // Adds to score the apropriate points for each letter
        if (n >= 0 && n < 26)
        {
            score += POINTS[n];
        }
    }
    // Once finished the iteration, returns the value of the score
    return score;
}

