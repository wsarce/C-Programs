/*
    Name        : Walker Arce
    Class       : CSCI 2240-003
    Institution : University of Nebraska at Omaha
    Honor Pledge: On my honor as a student of the University of Nebraska
                  of Omaha, I have neither given nor received unauthorized
                  help on this homework assignment.
    NUID        : 149
    Email       : warce@unomaha.edu
    Partners    : None.
*/

/*
   Include statements.
*/
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>

/*
   Function declarations.
*/
int genQuestion(int diff);
bool answerQuestion();
int response(bool answer, int correctAnswer);
int getAnswer(int randOp, int varOne, int varTwo);
char getOp(int opVal);

/*  Method Name     : main
    Parameters      : N/A
    Return value(S) : int, a zero
    Partners        : None.
    Description     : Main function that performs all needed operations.
*/
int main()
{
    int numQs = 0;
    int difficulty = 0;
    int correctQs = 0;
    int wrongQs = 0;
    int currentQ = 1;
    int correctAnswer;
    bool answered;
    int totalQs = 0;

    do
    {
       printf ("How many questions for this test (1 - 20)? ");
       scanf ("%d", &numQs);
    } while (numQs < 1 || numQs > 20);
    
    do
    {
        printf ("Select difficulty (1 - 4): ");
        scanf ("%d", &difficulty);
    } while (difficulty < 1 || difficulty > 4);
    
    totalQs = numQs;
    srand(time(NULL));

    do
    {
        printf ("Question %d: ", currentQ);
        correctAnswer = genQuestion(difficulty);
        answered = answerQuestion(correctAnswer);
        if (answered)
        {
            correctQs = correctQs + 1;;
        }
        else
        {
            wrongQs++;
        }
        response(answered, correctAnswer); 
        numQs--;
        currentQ++;
    } while (numQs != 0);

    printf ("Your score was %d/%d\n", correctQs, totalQs);

    return 0;
}

/*  Method Name     : genQuestion
    Parameters      : The difficulty specified by the user.
    Return Value(s) : The correct answer to generated question.
    Partners        : None.
    Description     : Generates a randomized arithmetic question with opVal as a 
                      helper function.
*/
int genQuestion(int diff)
{
    int varOne;
    int varTwo;
    int opVal = (rand() % (4 - 1 + 1)) + 1;
    int answer;
    char operation;

    if (diff == 1)
    {
        varOne = (rand() % (10 - 1 + 1)) + 1;
        varTwo = (rand() % (10 - 1 + 1)) + 1;
    }
    if (diff == 2)
    {
        varOne = (rand() % (50 - 1 + 1)) + 1;
        varTwo = (rand() % (50 - 1 + 1)) + 1;
    }
    if (diff == 3)
    {
        varOne = (rand() % (100 - 1 + 1)) + 1;
        varTwo = (rand() % (100 - 1 + 1)) + 1;
    }
    if (diff == 4)
    {
        varOne = (rand() % 100 + (-100));
        varTwo = (rand() % 100 + (-100));
    }

    operation = getOp(opVal);

    printf ("%d %c %d", varOne, operation, varTwo);
    answer = getAnswer(opVal, varOne, varTwo);
    
    return answer; 
}

/*  Method Name     : getOp
    Parameters      : The randomly generated operation value.
    Return Value(s) : Returns the operation char.
    Partners        : None.
    Description     : Generates the operation to be performed based on passed value.
*/
char getOp(int opVal)
{
    char operation;

    if (opVal == 1)
    {
        operation = '+';
    }
    if (opVal == 2)
    {
        operation = '-';
    }
    if (opVal == 3)
    {
        operation = '*';
    }
    if (opVal == 4)
    {
        operation = '/';
    }

    return operation;
}

/*  Method Name     : answerQuestion
    Parameters      : The passed answer supplied by the user.
    Return Value(s) : Returns a boolean if the answer is correct or wrong.
    Partners        : None.
    Description     : Determines if the answer supplied by the user is correct.
*/
bool answerQuestion(int passedAnswer)
{
    int answer;
    bool answered;

    printf("\nEnter Answer: ");
    scanf("%d", &answer);

    if (answer == passedAnswer)
    {
        answered = true;
    }
    else
    {
        answered = false;
    }

    return answered; 
}

/*  Method Name     : response
    Parameters      : Boolean if answer supplied is true or false, if false displays
                      the correct answer for the user.
    Return Value(s) : Returns a zero to return control to main()
    Partners        : None.
    Description     : Generates a random response to the user's answer based on passed
                      boolean.
*/
int response(bool answer, int correctAnswer)
{
    int r = (rand() % (3 - 1 + 1)) + 1;

    if (r == 1 && answer)
    {
        printf ("You're right!\n");
    }
    if (r == 1 && !answer)
    {
        printf ("Wrong answer!\n");
        printf ("The correct answer was %d\n", correctAnswer);
    }
    if (r == 2 && answer)
    {
        printf ("Good job!\n");
    }
    if (r == 2 && ! answer)
    {
        printf ("Sorry!\n");
        printf ("The correct answer was %d\n", correctAnswer);
    }
    if (r == 3 && answer)
    {
        printf ("Nice!\n");
    }
    if (r == 3 && !answer)
    {
        printf ("Try again!\n");
        printf ("The correct answer was %d\n", correctAnswer);
    }
    return 0;
}

/*  Method Name     : getAnswer
    Parameters      : The operation to be performed, the first variable, the second
                      variable.
    Return Value(s) : Returns the answer to the question.
    Partners        : None.
    Description     : Finds the answer to the question based on the passed parameters.
*/
int getAnswer(int randOp, int varOne, int varTwo)
{
    int answer = 0;

    if (randOp == 1)
    {
        answer = varOne + varTwo;
    }
    if (randOp == 2)
    {
        answer = varOne - varTwo;
    }
    if (randOp == 3)
    {
        answer = varOne * varTwo;
    }
    if (randOp == 4)
    {
        answer = varOne / varTwo;
    }
    return answer;
}
