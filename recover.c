#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

// jpeg header size and block size
const int JHEADER = 4;
const int BLOCK = 512;
//define BYTE
typedef uint8_t BYTE;

int main(int argc, char *argv[])
{
    // make sure there is only 2 commands on CL
    if (argc != 2)
    {
        printf("Usage: ./recover image\n");
        return 1;
    }

    //open the CL file
    FILE *input = fopen(argv[1], "r");
    if (input == NULL)
    {
        printf("File cannot be opened\n");
        return 1;
    }
    //output file
    FILE *output;
    //mem allocation for filename
    char *filename = malloc(8);
    //counter for output files
    int counter = 0;
    //create buffer for fread to use
    BYTE buffer[BLOCK];
    //read the input file in 512 blocks as of FAT file format
    while (fread(buffer, BLOCK, 1, input))
    {
        // if the jpg header is true then check to see if it is the first file or not
        if (buffer[0] == 0xff && buffer[1] == 0xd8 && buffer[2] == 0xff && (buffer[3] & 0xf0) == 0xe0)
        {
            // if counter = 0 then it is the first file, open new file and write first block and add 1 to counter
            // this will then go back to the if above which will then be false and go to the else if which will
            // write the next block until the if statement becomes true again
            // this will then check if the counter is 0, which it wont be, so it will go to else
            // this statement will now close the file we were working on because a jpg header has been read
            // it will then open a new file, write a block and add 1 to the counter and the process repeats.
            if (counter == 0)
            {
                sprintf(filename, "%03i.jpg", counter);
                output = fopen(filename, "w");
                fwrite(buffer, BLOCK, 1, output);
                counter++;
            }

            else
            {
                fclose(output);
                sprintf(filename, "%03i.jpg", counter++);
                output = fopen(filename, "w");
                fwrite(buffer, BLOCK, 1, output);

            }
        }
        else if (counter != 0)
        {
            fwrite(buffer, BLOCK, 1, output);
        }
    }

    //free mem alloc close files
    fclose(input);
    free(filename);
    return 0;
}
