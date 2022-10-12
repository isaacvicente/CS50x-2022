#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

typedef uint8_t BYTE;

int main(int argc, char *argv[])
{
    // Define the size of a block
    const int BLOCK_SIZE = 512;
    // Check if the user typed one command-line argument
    if (argc != 2)
    {
        printf("Usage: ./recover IMAGE\n");
        return 1;
    }
    // Opens the file
    FILE *raw_file = fopen(argv[1], "r");
    if (raw_file == NULL)
    {
        return 1;
    }
    // The buffer we will write to
    BYTE buffer[BLOCK_SIZE];
    // The counter of JPEGs
    int jpgs_found = 0;
    // Our image files (see the rest of the code)
    FILE *img;
    // Our filename with the size of a byte (8 bits)
    char filename[8];

    while (fread(&buffer, 1, BLOCK_SIZE, raw_file) == BLOCK_SIZE)
    {
        // Check for the beginning of a JPEG file
        if (buffer[0] == 0xff && buffer[1] == 0xd8 && buffer[2] == 0xff && (buffer[3] & 0xff) == 0xe0)
        {
            if (jpgs_found == 0) // For the first JPEG
            {
                // If it's the first JPEG, then create it's filename in the format ###.jpg, where 000.jpg would be the first image
                sprintf(filename, "%03i.jpg", jpgs_found);
                // Open the file, so that it's possible to write into it
                img = fopen(filename, "w");
                // Write the block into the JPEG
                fwrite(&buffer, 1, BLOCK_SIZE, img);
                // Flag that one JPEG was found
                jpgs_found ++;
            }
            else
            {
                // If not the first JPEG, close the previous one in order to make a new one
                fclose(img);

                // If it's the first JPEG, then create it's filename in the format ###.jpg, where 000.jpg would be the first image
                sprintf(filename, "%03i.jpg", jpgs_found);
                // Open the file, so that it's possible to write into it
                img = fopen(filename, "w");
                // Write the block into the JPEG
                fwrite(&buffer, 1, BLOCK_SIZE, img);
                // Flag that one JPEG was found
                jpgs_found ++;
            }
        }
        else if (jpgs_found > 0)
        {
            // If we already found a JPEG but not reached it's end, keep writing until a new one appears (or the file gets to it's end)
            fwrite(&buffer, 1, BLOCK_SIZE, img);
        }
    }
    // Finally, close the files open
    fclose(raw_file);
    fclose(img);
}