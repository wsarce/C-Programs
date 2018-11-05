/*
 *  Name:           Walker Arce
 *  NUID:           149
 *  Email:          warce@unomaha.edu
 *  Class:          CSCI 2240
 *  Assignment:     Word Processor
 *  Due Date:       10/31/18
 *  Honor Pledge:   I pledge on my honor as a student at the University of Nebraska at Omaha that I did not use any unauthorized materials in the creation
 *                  of this program.
 */

#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

/*
 *  Function declarations.
 */
char *strdup(const char *str);
int compare (const void * a, const void * b);

/*
 *  Name:       main
 *  Parameters: argc: number of arguments, argv: array of arguments
 *  Function:   Driver for the Word Processor
 */
int main (int argc, char** argv)
{
    FILE *file;

    char **wordList = malloc(sizeof(char*));

    int arguments = argc;
    char *charLimit = argv[1];
    int characterLimit = atoi(charLimit);
    char *filename = argv[2];
    
    if (arguments != 3 || !(characterLimit >= 25 || characterLimit <= 100))
    {
        printf("Recheck input parameters and try again.");
        return 0;
    }
    
    char outputLine[characterLimit];
    char word[1023];
    char outputName[80];
    char dataName[80];
    int numWords = 0;

    strcpy (outputName, filename);
    strcat (outputName, ".out");
    
    strcpy(dataName, filename);
    strcat(dataName, ".data");

    FILE *dataFile = fopen(dataName, "w");
    FILE *outputFile = fopen(outputName, "w");

    if ((file = fopen(filename, "r")))
    {
        strcpy(outputLine, "");

        while (feof(file) == 0)
        {
            strcpy(word, "");
            fscanf(file, "%s", word);
            int wordLength = strlen(word);
            int outputLength = strlen(outputLine);
            if ((wordLength + outputLength) < characterLimit)
            {
                strcat(outputLine, word);
                strcat(outputLine, " ");
                wordList = realloc(wordList, (numWords + 1) * sizeof(char *));
                wordList[numWords] = strdup(word);
                numWords++;
            }
            else
            {
                strcat(outputLine, "\n");
                fprintf(outputFile, "%s", outputLine);
                strcpy(outputLine, "");
                strcat(outputLine, word);
                strcat(outputLine, " ");
                wordList = realloc(wordList, (numWords + 1) * sizeof(char *));
                wordList[numWords] = strdup(word);
                numWords++;
            }
        }
    }
    else
    {
        //File not found
        printf("\nFile Not Found.\n");
        return 0;
    }
    
    qsort(wordList, numWords, sizeof(const char *), compare);

    int i;   
    int num = 1;
    char output[characterLimit];
    char numFound[characterLimit];
    int spaceFound = 0;

    for (i = 0; i < numWords; i++)
    {
        spaceFound = strcmp(wordList[i], "");

        if (((i + 1) < numWords) && (spaceFound != 0))
        {
            if (!(strcmp(wordList[i], wordList[i + 1])))
            {
                num++;
            }
            else
            {
                sprintf(numFound, "%d", num);
                strcpy(output, "");
                strcpy(output, wordList[i]);
                strcat(output, " - ");
                strcat(output, numFound);
                strcat(output, "\n");
                fprintf(dataFile, "%s", output);
                num = 1;
            }
        }
    }
    return 0;
}

/*
 *  Name:       compare
 *  Parameters: pointer a, pointer b 
 *  Function:   Compares two strings for qsort
 */
int compare (const void * a, const void * b)
{
    const char *sa = *(const char **)a;
    const char *sb = *(const char **)b;
    return strcmp(sa, sb);
}

/*
 *  Name:       *strdup
 *  Parameters: Passed string pointer
 *  Function:   Duplicates string to maintain conformity with C99
 */
char *strdup(const char *str)
{
    int n = strlen(str) + 1;
    char *dup = malloc(n);
    if(dup)
    {
        strcpy(dup, str);
    }
    return dup;
}
