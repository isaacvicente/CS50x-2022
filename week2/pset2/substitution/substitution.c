#include <cs50.h>
#include <ctype.h>
#include <string.h>
#include <stdio.h>

// Prototypes
bool is_valid(string text);
string convert_to_lower(string key_text);
string encipher(string p_text, string key);

int main(int argc, string argv[])
{
    string key;
    // Validate the key provided by the user
    if (argc == 2)
    {
        if (strlen(argv[1]) == 26)
        {
            //Check if the key is valid
            if (is_valid(argv[1]))
            {
                // For make things simple, convert all to lowercase
                key = convert_to_lower(argv[1]);
                // Prompt to user type the plaintext
                string plaintext = get_string("plaintext:  ");
                // Encipher the plaintext
                string ciphertext = encipher(plaintext, key);
                // Prompt the ciphertext
                printf("ciphertext: %s\n", ciphertext);
                return 0;
            }
            // If the key is not valid, we should return a 'error exit', i.e., 1 (or another non zero value)
            else
            {
                return 1;
            }
        }
        else
        {
            printf("Key must contain 26 characters.\n");
            return 1;
        }
    }
    else if (argc > 2)
    {
        // If the user provide many arguments, tell him/her he/her must give just one
        printf("You must specify only one argument, the key.\n");
        return 1;
    }
    else
    {
        // If the user didn't provide any argument at all, show the apropriate use
        printf("Usage: ./substitution key\n");
        return 1;
    }
}

bool is_valid(string text)
{
    bool value = false;
    // All characters must be alphabetic
    for (int i = 0, n = strlen(text); i < n; i++)
    {
        if (isalpha(text[i]))
        {
            if (isupper(text[i]))
            {
                text[i] = tolower(text[i]);
            }
            // Test for all previous values if the current value is unique or not (it must be)
            for (int j = i; j > 0; j--)
            {
                // The comparation
                if (i > 0 && text[i] != text[j - 1])
                {
                    value = true;
                }
                else
                {
                    printf("Key must not contain repeated characters.\n");
                    return false;
                }
            }
        }
        else
        {
            printf("Key must only contain alphabetic characters.\n");
            return false;
        }
    }
    return value;
}

string convert_to_lower(string key_text)
{
    for (int i = 0, n = strlen(key_text); i < n; i++)
    {
        // Convert the key's characters to lowercase (if they are uppercase)
        if (isupper(key_text[i]))
        {
            key_text[i] = tolower(key_text[i]);
        }
    }
    return key_text;
}

string encipher(string p_text, string key)
{
    int index = 0;
    for (int i = 0, n = strlen(p_text); i < n; i++)
    {
        if (isalpha(p_text[i]))
        {
            if (isupper(p_text[i]))
            {
                // If p_text[i] is A, the index is 0, if B, 1, and so on
                index = p_text[i] - 65;
                // Don't forget to uppercase the key's letter
                p_text[i] = toupper(key[index]);
            }
            else
            {
                // If p_text[i] is a, the index is 0, if b, 1, and so on
                index = p_text[i] - 97;
                p_text[i] = key[index];
            }
        }
    }
    // The plaintext is no more "the plaintext", but the encrypted one, so let's return it
    return p_text;
}