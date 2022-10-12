#include <cs50.h>
#include <math.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

// Prototypes
int count_letters(string text);
int count_words(string text);
int count_sentences(string text);

int main(void)
{
    // Ask the user to insert the text
    string txt = get_string("Text: ");

    // Count the number of letters within the text
    int n_let = count_letters(txt);
    printf("Letters: %i\n", n_let);

    // Count the number of words within the text
    int n_wor = count_words(txt);
    printf("Words: %i\n", n_wor);

    // Count the number of sentences within the text
    int n_sen = count_sentences(txt);
    printf("Sentences: %i\n", n_sen);

    // Calculate the L (the average number of letters per 100 words in the text)
    float L = ((float) n_let / n_wor) * 100.0;
    // Calculate the S (the average number of sentences per 100 words in the text)
    float S = ((float) n_sen / n_wor) * 100.0;
    // The Coleman-Liau index
    float index = 0.0588 * L - 0.296 * S - 15.8;
    // Round an ugly floated number to a nice integer one, i.e. the user's English grade
    int grade = (int) round(index);

    if (grade < 1)
    {
        printf("Before Grade 1\n");
    }
    else if (grade >= 1 && grade < 16)
    {
        printf("Grade %i\n", grade);
    }
    else // Then, the grade is equal or higher than 16
    {
        printf("Grade 16+\n");
    }
}

int count_letters(string text)
{
    int letters = 0;
    for (int i = 0, n = strlen(text); i < n; i++)
    {
        if (isalpha(text[i]))
        {
            letters ++;
        }
    }
    return letters;
}

int count_words(string text)
{
    int spaces = 0;
    for (int i = 0, n = strlen(text); i < n; i++)
    {
        if (text[i] == ' ')
        {
            spaces ++;
        }
    }
    // As described in the "walkthrough" video, the number of the words is generally the number of spaces plus one
    return spaces + 1;
}

int count_sentences(string text)
{
    int punctuation = 0;
    for (int i = 0, n = strlen(text); i < n; i++)
    {
        // Considering the cases of having "?", "??", "!" or "!!" in a text, in such way that if one these patterns are found, we consider having just one punctuation
        if (text[i] == '?' || (text[i] == '?' && text[i + 1] == '?') || text[i] == '!' || (text[i] == '!' && text[i + 1] == '!'))
        {
            punctuation ++;
        }
        // Here, we are wondering the case the user type either <some text>... or <some text>.
        if (text[i] == '.' || (text[i] == '.' && text[i + 1] == '.' && text[i + 2] == '.'))
        {
            punctuation ++;
        }
    }
    // Unlike the previous function, the number of punctuations is the number of sentences, once the very last character has to be a period
    return punctuation;
}