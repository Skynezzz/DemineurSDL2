#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <windows.h>
#include "Array.h"
#include "DemineurConsole.h"

/*
    0 : True
    1 : False / Err
*/

int demineurConsole(void)
{
    HANDLE CONSOLE_COLOR = GetStdHandle(STD_OUTPUT_HANDLE);
    int grid[GRID_SIZE_C][GRID_SIZE_C] = { GRID_HIDE_CASE_C };
    int tryPlayerPos[2] = { 0 };
    int inputOk = 0;
    int i = 0;

    // 1er tour
    displayGrid_C(grid, 0);
    for (i = 0; i < 2; i++)
    {
        if (i == 0)
            printf("x: ");
        else
            printf("y: ");

        inputOk = scanf_s("%d", &tryPlayerPos[i]);

        while (inputOk == 0 || !(tryPlayerPos[i] > 0 && tryPlayerPos[i] <= GRID_SIZE_C))
        {

            if (i == 0)
                printf("Coordonnees invalides\nx: ");
            else
                printf("Coordonnees invalides\ny: ");
            inputOk = scanf_s("%d", &tryPlayerPos[i]);
        }

        tryPlayerPos[i]--;
    }

    if (setBomb_C(grid, tryPlayerPos[0], tryPlayerPos[1]) == 1)
    {
        printf("Erreur: setBomb_C echoue");
        return 1;
    }

    if (findSafeCase_C(grid, tryPlayerPos[0], tryPlayerPos[1]) == 1)
    {
        displayGrid_C(grid, 1);
        printf("Game Over!");
        return 1;
    }

    if (isVictory_C(grid) == 0)
    {
        printf("Bravo!");
        return 0;
    }

    // Boucle jeu
    while (1)
    {
        displayGrid_C(grid, 0);
        for (i = 0; i < 2; i++)
        {
            if (i == 0)
                printf("x: ");
            else
                printf("y: ");

            inputOk = scanf_s("%d", &tryPlayerPos[i]);

            while (inputOk == 0 || !(tryPlayerPos[i] > 0 && tryPlayerPos[i] <= GRID_SIZE_C))
            {
                if (i == 0)
                    printf("Coordonnees invalides\nx: ");
                else
                    printf("Coordonnees invalides\ny: ");
                inputOk = scanf_s("%d", &tryPlayerPos[i]);
            }

            tryPlayerPos[i]--;
        }

        if (findSafeCase_C(grid, tryPlayerPos[0], tryPlayerPos[1]) == 1)
        {
            displayGrid_C(grid, 1);
            printf("Game Over!");
            return 1;
        }

        if (isVictory_C(grid) == 0)
        {
            printf("Bravo!");
            return 0;
        }
    }
    return 0;
}

void displayGrid_C(int grid[GRID_SIZE_C][GRID_SIZE_C], int loose)
{
    system("CLS");
    HANDLE CONSOLE_COLOR = GetStdHandle(STD_OUTPUT_HANDLE);
    int i = 0;
    int j = 0;
    int k = 0;


    printf("\n    ");
    for (k = 0; k < GRID_SIZE_C; k++)
        if (k >= 9)
            printf(" %d ", k + 1);
        else
            printf(" %d  ", k + 1);
    printf("\n   |");
    for (k = 0; k < GRID_SIZE_C; k++)
        printf("---|");
    printf("\n");
    for (i = 0; i < GRID_SIZE_C; i++) {
        if (i >= 9)
            printf(" %d|", i + 1);
        else
            printf(" %d |", i + 1);
        for (j = 0; j < GRID_SIZE_C; j++) {
            switch (grid[i][j])
            {
            case GRID_HIDE_CASE_C:
                printf(" - |");
                break;

            case GRID_SHOW_CASE_C:
                printf("   |");
                break;

            case GRID_BOMB_CASE_C:
                if (loose == 1)
                {
                    SetConsoleTextAttribute(CONSOLE_COLOR, 4);
                    printf(" X ");
                    SetConsoleTextAttribute(CONSOLE_COLOR, 15);
                    printf("|");
                    break;
                }
                else
                {
                    printf(" - |");
                    break;
                }

            default:
                printf(" %d |", grid[i][j] - 4);
                break;
            }
        }
        printf("\n   |");
        for (k = 0; k < GRID_SIZE_C; k++)
            printf("---|");
        printf("\n");
    }
}


int setBomb_C(int grid[GRID_SIZE_C][GRID_SIZE_C], int startPosX, int startPosY)
{
    srand(time(NULL));
    const int FREE_CASE_COUNT = (GRID_SIZE_C) * (GRID_SIZE_C);
    if (FREE_CASE_COUNT < BOMB_COUNT_C)
        return 1;
    stArray freeIndex = createTab(FREE_CASE_COUNT);
    int randomPos, i, j = 0;

    for (i = 3; i > 0; i--)
    {
        for (j = 3; j > 0; j--)
        {
            if (startPosX + i - 2 >= 0 && startPosX + i - 2 < GRID_SIZE_C && startPosY + j - 2 >= 0 && startPosY + j - 2 < GRID_SIZE_C)
            {
                removeAt(&freeIndex, (startPosY + i - 2) * 10 + startPosX + j - 2);
            }

        }
    }

    for (i = 0; i < BOMB_COUNT_C; i++)
    {
        randomPos = rand() % (freeIndex.size - i - 9);
        grid[freeIndex.point[randomPos] / 10][freeIndex.point[randomPos] % 10] = GRID_BOMB_CASE_C;
        removeAt(&freeIndex, randomPos);
    }

    free(freeIndex.point);
    return 0;
}

int bombArround_C(int grid[GRID_SIZE_C][GRID_SIZE_C], int posX, int posY)
{
    int bombCount = 0;
    int i = 0;
    int j = 0;

    for (i = 0; i < 3; i++)
    {
        for (j = 0; j < 3; j++)
        {
            if (i - 1 != 0 || j - 1 != 0)
            {
                if ((posX + i - 1 >= 0 && posX + i - 1 < GRID_SIZE_C && posY + j - 1 >= 0 && posY + j - 1 < GRID_SIZE_C) && grid[posY + j - 1][posX + i - 1] == GRID_BOMB_CASE_C)
                {
                    bombCount++;
                }
            }
        }
    }

    return bombCount;
}

int findSafeCase_C(int grid[GRID_SIZE_C][GRID_SIZE_C], int posX, int posY)
{
    int selfbombArround_C = bombArround_C(grid, posX, posY);
    int i = 0;
    int j = 0;

    if (grid[posY][posX] == GRID_BOMB_CASE_C)
        return 1;
    if (selfbombArround_C != 0)
    {
        grid[posY][posX] = selfbombArround_C + 4;
        return 0;
    }

    grid[posY][posX] = GRID_SHOW_CASE_C;
    for (i = 0; i < 3; i++)
    {
        for (j = 0; j < 3; j++)
        {
            if (i - 1 != 0 || j - 1 != 0)
            {
                if ((posX + i - 1 >= 0 && posX + i - 1 < GRID_SIZE_C && posY + j - 1 >= 0 && posY + j - 1 < GRID_SIZE_C) && grid[posY + j - 1][posX + i - 1] == GRID_HIDE_CASE_C)
                {
                    findSafeCase_C(grid, posX + i - 1, posY + j - 1);
                }
            }
        }
    }
    return 0;
}

int isVictory_C(int grid[GRID_SIZE_C][GRID_SIZE_C])
{
    int i = 0;
    int j = 0;

    for (i = 0; i < GRID_SIZE_C; i++)
    {
        for (j = 0; j < GRID_SIZE_C; j++)
        {
            if (grid[i][j] == GRID_HIDE_CASE_C)
                return 1;
        }
    }
    return 0;
}