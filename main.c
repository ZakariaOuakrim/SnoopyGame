#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <windows.h>
#include <time.h>
#define max_rows_cols 9
void setConsoleColor(int color)
{
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color);
}

void resetConsoleColor()
{
    setConsoleColor(15); // Set color back to white
}

void showLevel(int **character)
{
    for (int i = 0; i < max_rows_cols; i++)
    {
        printf("|\t");
        for (int j = 0; j < max_rows_cols; j++)
        {
            if (character[i][j] == 1) // set the color of snoopy to red
            {
                setConsoleColor(12); // Set color to red for Snoopy
                printf("%c\t", character[i][j]);
                resetConsoleColor();
            }
            else if (character[i][j] == 11)
            {                       // set the color of the ball
                setConsoleColor(9); // Set color to red for Snoopy
                printf("%c\t", character[i][j]);
                resetConsoleColor();
            }
            else
            {
                printf("%c\t", character[i][j]);
            }
        }
        printf("|\n");
    }
}
int **readFromFile(FILE *f)
{
    if (f == NULL)
    {
        return 0;
    }
    int **character = (int **)malloc(max_rows_cols * sizeof(int *));
    for (int i = 0; i < max_rows_cols; i++)
    {
        character[i] = (int *)malloc(max_rows_cols * sizeof(int));
    }
    // reading data
    for (int i = 0; i < max_rows_cols; i++)
    {
        for (int j = 0; j < max_rows_cols; j++)
        {
            fscanf(f, "%d", &character[i][j]);
        }
    }
    // print data
    // showLevel(character);
    return character;
}

int **movingBallFromSideToSide(int **p, int *ballRow, int *ballColumn, int diagonalDirection, int *ballValueStanding, int *snoopyRow, int *snoopyColumn)
{
    int newRow = *ballRow;
    int newColumn = *ballColumn;

    switch (diagonalDirection)
    {
    case 1: // Move diagonally up-right
        if (*ballRow - 1 >= 0 && *ballColumn + 1 < max_rows_cols)
        {
            newRow = *ballRow - 1;
            newColumn = *ballColumn + 1;
        }
        break;
    case 2: // Move diagonally up-left
        if (*ballRow - 1 >= 0 && *ballColumn - 1 >= 0)
        {
            newRow = *ballRow - 1;
            newColumn = *ballColumn - 1;
        }
        break;
    case 3: // Move diagonally down-right
        if (*ballRow + 1 < max_rows_cols && *ballColumn + 1 < max_rows_cols)
        {
            newRow = *ballRow + 1;
            newColumn = *ballColumn + 1;
        }
        break;
    case 4: // Move diagonally down-left
        if (*ballRow + 1 < max_rows_cols && *ballColumn - 1 >= 0)
        {
            newRow = *ballRow + 1;
            newColumn = *ballColumn - 1;
        }
        break;
    }

    p[*ballRow][*ballColumn] = *ballValueStanding;
    *ballValueStanding = p[newRow][newColumn];
    *ballRow = newRow;
    *ballColumn = newColumn;
    p[*ballRow][*ballColumn] = 11;
    if (*snoopyColumn == *ballColumn && *snoopyRow == *ballRow)
    {
        printf("\nGame Over! Snoopy touched the ball!!");
        exit(0);
    }
    return p;
}

int **movingFromPointToPoint(int **p, int directionWhereGoing, int *snoopyRow, int *snoopyColumn, int *snoppyValueStanding, int *levelScore)
{
    int newRow = *snoopyRow;
    int newColumn = *snoopyColumn;

    switch (directionWhereGoing) 
    {
    case 1: // going right
        if (*snoopyColumn + 1 < max_rows_cols)
        {
            newColumn = *snoopyColumn + 1;
        }
        break;
    case 2: // going left
        if (*snoopyColumn - 1 >= 0)
        {
            newColumn = *snoopyColumn - 1;
        }
        break;
    case 3: // going up
        if (*snoopyRow - 1 >= 0)
        {
            newRow = *snoopyRow - 1;
        }
        break;
    case 4: // going down
        if (*snoopyRow + 1 < max_rows_cols)
        {
            newRow = *snoopyRow + 1;
        }
        break;
    }

    p[*snoopyRow][*snoopyColumn] = *snoppyValueStanding;
    *snoppyValueStanding = p[newRow][newColumn];
    // check if snoopy captured a bird
    if (*snoppyValueStanding == 14)
    {                             // if snoppy is standing on a bird
        *snoppyValueStanding = 0; // Make where he is standing empty
        *levelScore += 1;         // change the level score
    }
    *snoopyRow = newRow;
    *snoopyColumn = newColumn;
    p[*snoopyRow][*snoopyColumn] = 1; // move snoopy

    return p;
}

int main()
{
    FILE *f;
    int **p, *snoopyRow, *snoopyColumn, *ballRow, *ballColumn, randomVariableInWhichTheBallWillGo;
    int *levelScore; // keep track on how many birds snoopy captured
    int initialeStandingValueInTheMiddleOfSnoopy = 0;
    int initialeStandingValueOfTheBall = 0;
    int *snoppyValueStanding, *ballValueStanding;
    clock_t start, end;

    levelScore = (int *)malloc(sizeof(int));
    snoopyRow = (int *)malloc(sizeof(int));
    snoopyColumn = (int *)malloc(sizeof(int));
    ballColumn = (int *)malloc(sizeof(int));
    ballRow = (int *)malloc(sizeof(int));
    *levelScore = 0;
    *snoopyColumn = 4;
    *snoopyRow = 4;
    *ballColumn = 4;
    *ballRow = 0;
    f = fopen("level1.txt", "r");
    p = readFromFile(f);
    fclose(f);
    snoppyValueStanding = &initialeStandingValueInTheMiddleOfSnoopy;
    ballValueStanding = &initialeStandingValueOfTheBall;
    //*****************************movement***************************************
    char key;
    start = clock(); // start time
    while (1)
    {
        randomVariableInWhichTheBallWillGo = rand() % 4 + 1;
        movingBallFromSideToSide(p, ballRow, ballColumn, randomVariableInWhichTheBallWillGo, ballValueStanding, snoopyRow, snoopyColumn);
        showLevel(p);

        if (_kbhit())
        {
            switch (key = getch())
            { // the real value
            case 72:
                movingFromPointToPoint(p, 3, snoopyRow, snoopyColumn, snoppyValueStanding, levelScore);
                system("cls"); // clear the console
                showLevel(p);
                break;
            case 75:
                movingFromPointToPoint(p, 2, snoopyRow, snoopyColumn, snoppyValueStanding, levelScore);
                system("cls"); // clear the console
                showLevel(p);
                break;
            case 77:
                movingFromPointToPoint(p, 1, snoopyRow, snoopyColumn, snoppyValueStanding, levelScore);
                system("cls"); // clear the console
                showLevel(p);
                break;
            case 80:
                movingFromPointToPoint(p, 4, snoopyRow, snoopyColumn, snoppyValueStanding, levelScore);
                system("cls"); // clear the console
                showLevel(p);
                break;
            }
        }
        if (*levelScore == 4)
        {
            printf("you have finished the first level");
            break;
        }
        if((((double)clock()-start)/CLOCKS_PER_SEC>30)){
            printf("Times out !!");
            break;   
        }
        if (key == 'q')
        {
            break;
        }
        Sleep(500);

        system("cls"); // clear the console
        


    }
    for (int i = 0; i < max_rows_cols; i++)
    {
        free(p[i]);
    }
    free(p);
    free(snoopyRow);
    free(snoopyColumn);
    free(ballRow);
    free(ballColumn);
    free(levelScore);

    return 0;
}
