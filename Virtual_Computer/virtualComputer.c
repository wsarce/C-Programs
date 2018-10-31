/*
 *  Name:           Walker Arce
 *  NUID:           149
 *  Email:          warce@unomaha.edu
 *  Class:          CSCI 2240
 *  Assignment:     Virtual Computer
 *  Due Date:       10/16/18
 *  Honor Pledge:   I pledge on my honor as a student at the University of Nebraska at Omaha that I did not use any unauthorized materials in the creation
 *                  of this program.
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

/*
 *  Structure for virtual computer.
 */
struct VirtualComputer
{
    int accumulator;
    int instructionCounter;
    int instructionRegister;
    int operationCode;
    int operand;
    int memory[100];
};

/*
 *  Function declarations.
 */
bool execute(struct VirtualComputer *);
bool compile(struct VirtualComputer *, char filename[]);
void outputRegisters(struct VirtualComputer *);
bool processValue(int value);

/*
 *  Name:       main
 *  Parameters: argc: number of arguments, argv: array of arguments
 *  Function:   Driver for the virtual computer
 */
int main(int argc, char **argv)
{
    struct VirtualComputer vc = {0};
    
    if (compile(&vc, argv[1]))
    {
        stdin = fopen("/dev/tty", "r");
        execute(&vc);
        outputRegisters(&vc);
    }
    else
    {
        printf("Compilation terminated.\n");
        outputRegisters(&vc);
    }
    return 0;
}

/*
 *  Name:       execute
 *  Parameters: VirtualComputer structure
 *  Function:   Executes instructions that were compiled into instruction memory
 */
bool execute(struct VirtualComputer *vc)
{   
    for (vc->instructionCounter = 0; vc->instructionCounter < 99; vc->instructionCounter++)
    {
        vc->operationCode = (vc->memory[vc->instructionCounter]) / 100;
        
        if (vc->operationCode == 10)
        {
            //READ
            vc->operand = (vc->memory[vc->instructionCounter]) - 1000;
            scanf("%4d", &vc->memory[vc->operand]);
        }
        else if (vc->operationCode == 11)
        {
            //WRIT
            vc->operand = (vc->memory[vc->instructionCounter]) - 1100;
            printf ("%d", (vc->memory[vc->operand]));
        }
        else if (vc->operationCode == 12)
        {
            //PRNT
            vc->operand = (vc->memory[vc->instructionCounter]) - 1200;

            for (vc->instructionRegister = vc->operand; vc->memory[vc->instructionRegister] != 0; vc->instructionRegister++)
            {
                if (!(vc->instructionRegister > 99))
                {
                    printf ("%c", ((vc->memory[vc->instructionRegister]) / 100));
                    printf ("%c", ((vc->memory[vc->instructionRegister]) - (((vc->memory[vc->instructionRegister]) / 100) * 100)));
                }
                else
                {
                    //Segmentation Error
                    printf("Segmentation Error\n");
                    outputRegisters(vc);
                }
            }
        }
        else if (vc->operationCode == 20)
        {
            //LOAD
            vc->operand = (vc->memory[vc->instructionCounter]) - 2000;
            vc->accumulator = vc->memory[vc->operand];
        }
        else if (vc->operationCode == 21)
        {
            //STOR command
            vc->operand = (vc->memory[vc->instructionCounter]) - 2100;
            if (processValue(vc->operand))
            {
                vc->memory[vc->operand] = vc->accumulator;
            }
            else
            {
                //Word Overflow
                outputRegisters(vc);
                return false;
            }
        }
        else if (vc->operationCode == 30)
        {
            vc->operand = (vc->memory[vc->instructionCounter]) - 3000;
            vc->accumulator += vc->memory[vc->operand];
        }
        else if (vc->operationCode == 31)
        {
            vc->operand = (vc->memory[vc->instructionCounter]) - 3100;
            vc->accumulator -= vc->memory[vc->operand];
        }
        else if (vc->operationCode == 32)
        {
            vc->operand = (vc->memory[vc->instructionCounter]) - 3200;

            if (vc->memory[vc->operand] != 0)
            {
                vc->accumulator /= vc->memory[vc->operand];
            }
            else
            {
                //Divide by 0
                outputRegisters(vc);
                return false;
            }
        }
        else if (vc->operationCode == 33)
        {
            vc->operand = (vc->memory[vc->instructionCounter]) - 3300;
            vc->accumulator *= vc->memory[vc->operand];
        }
        else if (vc->operationCode == 34)
        {
            vc->operand = (vc->memory[vc->instructionCounter]) - 3400;
            vc->accumulator %= vc->memory[vc->operand];
        }
        else if (vc->operationCode == 40)
        {
            vc->operand = (vc->memory[vc->instructionCounter]) - 4000;

            if (vc->operand > 99)
            {
                //Segmentation Fault
                outputRegisters(vc);
                return false;
            }
            else
            {
                //BRAN
                vc->instructionCounter = vc->operand - 1;
            }
        }
        else if (vc->operationCode == 41)
        {
            vc->operand = (vc->memory[vc->instructionCounter]) - 4100;

            if (vc->operand > 99)
            {
                //Segmentation Fault
                outputRegisters(vc);
                return false;
            }
            else
            {
                //BRNG
                if (vc->accumulator < 0)
                {
                    vc->instructionCounter = vc->operand - 1;
                }
            }
        }
        else if (vc->operationCode == 42)
        {
            vc->operand = (vc->memory[vc->instructionCounter]) - 4200;

            if (vc->operand > 99)
            {
                //Segmentation Fault
                outputRegisters(vc);
                return false;
            }
            else
            {
                if (vc->accumulator == 0)
                {
                    //BRZR
                    vc->instructionCounter = vc->operand - 1;
                }
            }
        }
        else if (vc->operationCode == 99)
        {
            //HALT command
            break;
        }
        else
        {
            //Unknown Command.
            return false;
        }
    }
    return true;
}

/*
 *  Name:       compile
 *  Parameters: structure for computer, filename in string format
 *  Output:     True if successful, false if error encountered
 *  Function:   Compiles inputted file into usable machine code in memory
 */
bool compile(struct VirtualComputer *vc, char filename[])
{
    FILE *file;
    int count = 0;

    char op_buff[5];
    char mem_buff[2];
    char info_buff[5];

    int info;
    int compInst;
    int mem_add;
    int opCode;
    
    bool haltFound;

    //Verify file exists
    if ((file = fopen(filename, "r")))
    {
        //Process file using FSM
        while (!(feof(file)))
        {
            if (count == 0)
            {
                fscanf(file, "%s", mem_buff);
                mem_add = atoi(mem_buff);
                ++count;
            }
            else if (count == 1)
            {
                fscanf(file, "%s", op_buff);

                if (strcmp(op_buff, "READ") == 0)
                {
                    opCode = 1000;  
                    ++count;  
                }
                else if (strcmp(op_buff, "WRIT") == 0)
                {
                    opCode = 1100;
                    ++count;
                }
                else if (strcmp(op_buff, "PRNT") == 0)
                {
                    opCode = 1200;
                    ++count;
                }
                else if (strcmp(op_buff, "LOAD") == 0)
                {
                    opCode = 2000;
                    ++count;
                }
                else if (strcmp(op_buff, "STOR") == 0)
                {
                    opCode = 2100;
                    ++count;
                }
                else if (strcmp(op_buff, "SET") == 0)
                {
                    opCode = 2200;
                    fscanf (file, "%s", info_buff);
                    info = atoi(info_buff);
                    if (processValue(info))
                    {
                        vc->memory[mem_add] = info;
                        count = 0;
                    }
                    else
                    {
                        //Word Overflow.
                        printf ("\nWord Overflow: %d\n", info);
                        return false;
                    }
                }
                else if (strcmp(op_buff, "ADD") == 0)
                {
                    opCode = 3000;
                    ++count;
                }
                else if (strcmp(op_buff, "SUB") == 0)
                {
                    opCode = 3100;
                    ++count;
                }
                else if (strcmp(op_buff, "DIV") == 0)
                {
                    opCode = 3200;
                    ++count;
                }
                else if (strcmp(op_buff, "MULT") == 0)
                {
                    opCode = 3300;
                    ++count;
                }
                else if (strcmp(op_buff, "MOD") == 0)
                {
                    opCode = 3400;
                    ++count;
                }
                else if (strcmp(op_buff, "BRAN") == 0)
                {
                    opCode = 4000;
                    ++count;
                }
                else if (strcmp(op_buff, "BRNG") == 0)
                {
                    opCode = 4100;
                    ++count;
                }
                else if (strcmp(op_buff, "BRZR") == 0)
                {
                    opCode = 4200;
                    ++count;
                }
                else if (strcmp(op_buff, "HALT") == 0)
                {
                    opCode = 9900;
                    haltFound = true;
                    ++count;
                }
                else
                {
                    //Unknown Command.
                    printf("\nUnknown Command: %s\n", op_buff);
                    return false;    
                }
            }
            else if (count == 2)
            {
                fscanf(file, "%s", info_buff);
                info = atoi(info_buff);
                compInst = opCode + info;
                if ((processValue(compInst)))
                {
                    vc->memory[mem_add] = compInst;
                    count = 0;
                }
                else
                {
                    //Word Overflow.
                    printf ("\nWord Overflow: %d\n", compInst);
                    return false;
                }
            }
        }
        if (!haltFound)
        {
            //No HALT Error.
            printf ("\nHALT not found.\n");
            return false;
        }
    }
    else
    {
        //File Not Found
        printf ("\nFile Not Found.\n");
        return false;
    }
    //outputRegisters(vc);
    return true;
}

/*
 *  Name:       outputRegisters
 *  Parameters: Virtual Computer structure
 *  Output:     Outputs the full memory map to terminal
 */
void outputRegisters(struct VirtualComputer *vc)
{
    printf("\nREGISTERS:");
    
    if (vc->accumulator < 0)
    {
        printf("\naccumulator              %05d", vc->accumulator);              
    }
    else
    {
        printf("\naccumulator              +%04d", vc->accumulator);
    }   

    printf("\ninstructionCounter          %02d", vc->instructionCounter);
     
    if (vc->instructionRegister < 0)
    {
        printf("\ninstructionRegister      %05d", vc->instructionRegister);
    }
    else
    {
        printf("\ninstructionRegister      +%04d", vc->instructionRegister);
    }

    printf("\noperationCode               %02d", vc->operationCode);
    printf("\noperand                     %02d", vc->operand);
    printf("\nMEMORY:");
    printf("\n       0     1     2     3     4     5     6     7     8     9");

    int i;

    for (i = 0; i < 100; i++)
    {
        if (!(i % 10))
        {
            printf("\n%2d", i);
        }
        if (vc->memory[i] < 0)
        {
            printf (" %05d", vc->memory[i]);
        }
        else
        {
            printf (" +%04d", vc->memory[i]);
        }
    }
    printf ("\n");
}

/*
 *  Name:       processValue
 *  Parameters: int value to be processed
 *  Output:     Returns boolean
 *  Function:   Helper function to check if number is greater than four digits
 */
bool processValue(int value)
{
    int count = 0;

    while (value != 0)
    {
        value /= 10;
        ++count;
    }
    if (count > 4)
    {
        return false;
    }
    else
    {
        return true;
    }
}
