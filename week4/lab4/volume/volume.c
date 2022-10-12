// Modifies the volume of an audio file

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

// Number of bytes in .wav header
const int HEADER_SIZE = 44;

int main(int argc, char *argv[])
{
    // Check command-line arguments
    if (argc != 4)
    {
        printf("Usage: ./volume input.wav output.wav factor\n");
        return 1;
    }

    // Open files and determine scaling factor
    FILE *input = fopen(argv[1], "r");
    if (input == NULL)
    {
        printf("Could not open file.\n");
        return 1;
    }

    FILE *output = fopen(argv[2], "w");
    if (output == NULL)
    {
        printf("Could not open file.\n");
        return 1;
    }

    float factor = atof(argv[3]);

    // Copy header from input file to output file
    // Create a header array of characters to store the input's header
    uint8_t header[HEADER_SIZE];
    // Read the input file
    fread(header, HEADER_SIZE, 1, input);
    // Write to output file
    fwrite(header, HEADER_SIZE, 1, output);

    // Read samples from input file and write updated data to output file
    // Create the buffer of the 'samples' within the input file
    int16_t buffer;
    // Read the whole input until it's end
    while (fread(&buffer, sizeof(int16_t), 1, input) > 0)
    {
        // Update the volume according to the factor given
        buffer *= factor;
        // For each value read and modified, write the new 'sample' to the output
        fwrite(&buffer, sizeof(int16_t), 1, output);
    }

    // Close files
    fclose(input);
    fclose(output);
}

