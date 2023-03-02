#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <windows.h>
#include <SDL.h>
#include "Array.h"

#define GRID_SIZE 10
#define BOMB_COUNT 17

#define SIDE_PADDING 400

#define GRID_HIDE_CASE 0
#define GRID_SHOW_CASE 1
#define GRID_BOMB_CASE 2
#define GRID_FLAG_BOMB_CASE 3
#define GRID_FLAG_HIDE_CASE 4
#define GRID_ONE_CASE 5
#define GRID_TWO_CASE 6
#define GRID_THREE_CASE 7
#define GRID_FOUR_CASE 8
#define GRID_FIVE_CASE 9
#define GRID_SIX_CASE 10
#define GRID_SEVEN_CASE 11
#define GRID_EIGHT_CASE 12

/*
    0 : True
    1 : False / Err
    y:8 / x:10
    y:14 / x:18
    y:20 / x:24
*/

void DestroyWindowAndRenderer(SDL_Window* window, SDL_Renderer* renderer, const char* message);
void displayMenu(int state, SDL_Window* window, SDL_Renderer* renderer);
void displayGrid(int grid[GRID_SIZE][GRID_SIZE], int loose, SDL_Window* window, SDL_Renderer* renderer);
int setBomb(int grid[GRID_SIZE][GRID_SIZE], int startPosX, int startPosY);
int bombArround(int grid[GRID_SIZE][GRID_SIZE], int posX, int posY);
int findSafeCase(int grid[GRID_SIZE][GRID_SIZE], int posX, int posY);
int isVictory(int grid[GRID_SIZE][GRID_SIZE]);
void ExitWithError(const char* message);


int main(void)
{
    int grid[GRID_SIZE][GRID_SIZE] = { GRID_HIDE_CASE };
    int tryPlayerPos[2] = { 0 };
    int choice = 0;
    int i = 0;
    int flagRemaining = BOMB_COUNT;

    //-----------------SDL_INIT-----------------//

    SDL_Window* window = NULL;
    SDL_Renderer* renderer = NULL;

    if (SDL_Init(SDL_INIT_VIDEO) != 0)
    {
        ExitWithError("Initialisation SDL echouee");
    }

    if (window = SDL_CreateWindowAndRenderer(800, 600, SDL_WINDOW_ALWAYS_ON_TOP, &window, &renderer) != 0)
    {
        ExitWithError("Creation fenetre et rendu echoues");
    }

    SDL_bool program_launched = SDL_TRUE;


    //------------------------------------------//
    // 
    //------------------------------------------// Menu

    displayMenu(0, window, renderer);

    while (program_launched)
    {
        SDL_Event event;

        while (SDL_PollEvent(&event))
        {
            switch (event.type)
            {
            case SDL_MOUSEBUTTONDOWN:
                if (event.button.button == SDL_BUTTON_LEFT)
                {
                    tryPlayerPos[0] = event.button.x;
                    tryPlayerPos[1] = event.button.y;
                    if ((tryPlayerPos[0] >= 315 && tryPlayerPos[0] < 485) && (tryPlayerPos[1] >= 350 && tryPlayerPos[1] < 447))
                    {
                        displayMenu(1, window, renderer);
                        SDL_Delay(300);
                        program_launched = SDL_FALSE;
                    }
                }
                break;

            case SDL_QUIT:
                program_launched = SDL_FALSE;
                DestroyWindowAndRenderer(&window, &renderer, "None");
                return 0;

            default:
                break;
            }
        }
    }

    program_launched = SDL_TRUE;

    displayMenu(2, window, renderer);

    while (program_launched)
    {
        SDL_Event event;

        while (SDL_PollEvent(&event))
        {
            switch (event.type)
            {
            case SDL_MOUSEBUTTONDOWN:
                if (event.button.button == SDL_BUTTON_LEFT)
                {
                    tryPlayerPos[0] = event.button.x;
                    tryPlayerPos[1] = event.button.y;
                    if ((tryPlayerPos[0] >= 315 && tryPlayerPos[0] < 485) && (tryPlayerPos[1] >= 350 && tryPlayerPos[1] < 447))
                    {
                        displayMenu(1, window, renderer);
                        SDL_Delay(300);
                        program_launched = SDL_FALSE;
                    }
                }
                break;

            case SDL_QUIT:
                program_launched = SDL_FALSE;
                DestroyWindowAndRenderer(&window, &renderer, "None");
                return 0;

            default:
                break;
            }
        }
    }

    program_launched = SDL_TRUE;

    //------------------------------------------//
    // 
    //------------------------------------------// Tour 1

    displayGrid(grid, 0, window, renderer);
    while (program_launched)
    {
        SDL_Event event;

        while (SDL_PollEvent(&event))
        {
            switch (event.type)
            {
            case SDL_MOUSEBUTTONDOWN:
                if (event.button.button == SDL_BUTTON_LEFT)
                {
                    tryPlayerPos[0] = (event.button.x - SIDE_PADDING) / 50;
                    tryPlayerPos[1] = event.button.y / 50;
                    if ((tryPlayerPos[0] >= 0 && tryPlayerPos[0] < GRID_SIZE) && (tryPlayerPos[1] >= 0 && tryPlayerPos[1] < GRID_SIZE))
                        program_launched = SDL_FALSE;
                }
                break;
                
            case SDL_QUIT:
                program_launched = SDL_FALSE;
                DestroyWindowAndRenderer(&window, &renderer, "None");
                return 0;

            default:
                break;
            }
        }
    }
    program_launched = SDL_TRUE;

    if (setBomb(grid, tryPlayerPos[0], tryPlayerPos[1]) == 1)
    {
        printf("Erreur: setBomb echoue");
        return 1;
    }

    if (findSafeCase(grid, tryPlayerPos[0], tryPlayerPos[1]) == 1)
    {
        displayGrid(grid, 1, window, renderer);
        printf("Game Over!");
        DestroyWindowAndRenderer(&window, &renderer, "None");
        return 1;
    }

    if (isVictory(grid) == 0)
    {
        printf("Bravo!");
        DestroyWindowAndRenderer(&window, &renderer, "None");
        return 0;
    }

    //------------------------------------------// 
    // 
    //------------------------------------------// Boucle tours 1+

    displayGrid(grid, 0, window, renderer);

    while (program_launched)
    {
        SDL_Event event;

        while (SDL_PollEvent(&event))
        {
            switch (event.type)
            {
            case SDL_MOUSEBUTTONDOWN:
                tryPlayerPos[0] = (event.button.x - SIDE_PADDING) / 50;
                tryPlayerPos[1] = event.button.y / 50;
                if (event.button.button == SDL_BUTTON_LEFT)
                {
                    if ((tryPlayerPos[0] >= 0 && tryPlayerPos[0] < GRID_SIZE) && (tryPlayerPos[1] >= 0 && tryPlayerPos[1] < GRID_SIZE))
                    {
                        if (findSafeCase(grid, tryPlayerPos[0], tryPlayerPos[1]) == 1)
                        {
                            displayGrid(grid, 1, window, renderer);
                            printf("Game Over!");
                            SDL_Delay(3000);
                            DestroyWindowAndRenderer(&window, &renderer, "None");
                            return 1;
                        }

                        if (isVictory(grid) == 0)
                        {
                            printf("Bravo!");
                            SDL_Delay(3000);
                            DestroyWindowAndRenderer(&window, &renderer, "None");
                            return 0;
                        }
                        displayGrid(grid, 0, window, renderer);
                    }
                }
                if (event.button.button == SDL_BUTTON_RIGHT)
                {
                    if ((tryPlayerPos[0] >= 0 && tryPlayerPos[0] < GRID_SIZE) && (tryPlayerPos[1] >= 0 && tryPlayerPos[1] < GRID_SIZE))
                    {
                        switch (grid[tryPlayerPos[1]][tryPlayerPos[0]])
                        {
                        case GRID_HIDE_CASE:
                            grid[tryPlayerPos[1]][tryPlayerPos[0]] = GRID_FLAG_HIDE_CASE;
                            break;

                        case GRID_BOMB_CASE:
                            grid[tryPlayerPos[1]][tryPlayerPos[0]] = GRID_FLAG_BOMB_CASE;
                            break;

                        case GRID_FLAG_HIDE_CASE:
                            grid[tryPlayerPos[1]][tryPlayerPos[0]] = GRID_HIDE_CASE;
                            break;

                        case GRID_FLAG_BOMB_CASE:
                            grid[tryPlayerPos[1]][tryPlayerPos[0]] = GRID_BOMB_CASE;
                            break;

                        default:
                            break;
                        }
                        displayGrid(grid, 0, window, renderer);
                    }
                }
                break;

            case SDL_QUIT:
                program_launched = SDL_FALSE;
                break;

            default:
                break;
            }
        }
    }
    DestroyWindowAndRenderer(&window, &renderer, "None");
    return EXIT_SUCCESS;
}

void DestroyWindowAndRenderer(SDL_Window* window, SDL_Renderer* renderer, const char* message)
{
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    ExitWithError(message);
}

void ExitWithError(const char* message)
{
    if (message == "None")
    {
        SDL_Quit();
        exit(EXIT_FAILURE);
        return;
    }
    SDL_Log("ERREUR : %s > %s\n", message, SDL_GetError());
    SDL_Quit();
    exit(EXIT_FAILURE);
}

void displayMenu(int state, SDL_Window* window, SDL_Renderer* renderer)
{
    SDL_Surface* imageCase = NULL;
    SDL_Texture* texture = NULL;
    SDL_Rect rectangleTest;

    imageCase = NULL;
    texture = NULL;

    imageCase = SDL_LoadBMP("src/background.bmp");

    if (imageCase == NULL)
        DestroyWindowAndRenderer(&window, &renderer, "\"src/background.bmp\" n'a pas pu se charger");

    texture = SDL_CreateTextureFromSurface(renderer, imageCase);
    SDL_FreeSurface(imageCase);

    if (texture == NULL)
        DestroyWindowAndRenderer(&window, &renderer, "Impossible de creer \"texture\"");

    if (SDL_QueryTexture(texture, NULL, NULL, &rectangleTest.w, &rectangleTest.h) != 0)
        DestroyWindowAndRenderer(&window, &renderer, "Impossible d'acceder à \"texture\"");

    rectangleTest.x = 0;
    rectangleTest.y = 0;

    if (SDL_RenderCopy(renderer, texture, NULL, &rectangleTest) != 0)
        DestroyWindowAndRenderer(&window, &renderer, "Impossible d'afficher \"texture\"");


    imageCase = NULL;
    texture = NULL;
    switch (state)
    {
    case 0:
        imageCase = SDL_LoadBMP("src/button.bmp");
        break;

    case 1:
        imageCase = SDL_LoadBMP("src/button2.bmp");
        break;

    default:
        imageCase = SDL_LoadBMP("src/bomb.bmp");
        break;
    }

    if (imageCase == NULL)
        DestroyWindowAndRenderer(&window, &renderer, "\"src/button.bmp\" n'a pas pu se charger");

    texture = SDL_CreateTextureFromSurface(renderer, imageCase);
    SDL_FreeSurface(imageCase);

    if (texture == NULL)
        DestroyWindowAndRenderer(&window, &renderer, "Impossible de creer \"texture\"");

    if (SDL_QueryTexture(texture, NULL, NULL, &rectangleTest.w, &rectangleTest.h) != 0)
        DestroyWindowAndRenderer(&window, &renderer, "Impossible d'acceder à \"texture\"");

    rectangleTest.x = 400 - rectangleTest.w / 2;
    rectangleTest.y = 350;

    if (SDL_RenderCopy(renderer, texture, NULL, &rectangleTest) != 0)
        DestroyWindowAndRenderer(&window, &renderer, "Impossible d'afficher \"texture\"");

    SDL_RenderPresent(renderer);
    SDL_DestroyTexture(texture);

}

void displayGrid(int grid[GRID_SIZE][GRID_SIZE], int loose, SDL_Window* window, SDL_Renderer* renderer)
{
    const char* tab[13];
    tab[0] = "src/hide.bmp";
    tab[1] = "src/show.bmp";

    if (loose == 1)
        tab[2] = "src/bomb.bmp";
    else
        tab[2] = "src/hide.bmp";

    if (loose == 1)
        tab[3] = "src/bomb.bmp";
    else
        tab[3] = "src/flag.bmp";

    tab[4] = "src/flag.bmp";
    tab[5] = "src/one.bmp";
    tab[6] = "src/two.bmp";
    tab[7] = "src/three.bmp";
    tab[8] = "src/four.bmp";
    tab[9] = "src/five.bmp";
    tab[10] = "src/six.bmp";
    tab[11] = "src/seven.bmp";
    tab[12] = "src/eight.bmp";

    int i, j = 0;

    SDL_Surface* imageCase = NULL;
    SDL_Texture* texture = NULL;
    SDL_Rect rectangleTest;

    for (i = 0; i < GRID_SIZE; i++) {
        rectangleTest.y = 50 * i;
        for (j = 0; j < GRID_SIZE; j++) {
            imageCase = NULL;
            texture = NULL;

            imageCase = SDL_LoadBMP(tab[grid[i][j]]);

            if (imageCase == NULL)
                DestroyWindowAndRenderer(&window, &renderer, "\"imageCase\" n'a pas pu se charger");

            texture = SDL_CreateTextureFromSurface(renderer, imageCase);
            SDL_FreeSurface(imageCase);

            if (texture == NULL)
                DestroyWindowAndRenderer(&window, &renderer, "Impossible de creer \"texture\"");

            if (SDL_QueryTexture(texture, NULL, NULL, &rectangleTest.w, &rectangleTest.h) != 0)
                DestroyWindowAndRenderer(&window, &renderer, "Impossible d'acceder à \"texture\"");

            rectangleTest.x = 50 * j + SIDE_PADDING;

            if (SDL_RenderCopy(renderer, texture, NULL, &rectangleTest) != 0)
                DestroyWindowAndRenderer(&window, &renderer, "Impossible d'afficher \"texture\"");
        }
    }
    SDL_RenderPresent(renderer);
    SDL_DestroyTexture(texture);
}

int setBomb(int grid[GRID_SIZE][GRID_SIZE], int startPosX, int startPosY)
{
    srand(time(NULL));
    const int FREE_CASE_COUNT = (GRID_SIZE) * (GRID_SIZE);
    if (FREE_CASE_COUNT < BOMB_COUNT)
        return 1;
    stArray freeIndex = createTab(FREE_CASE_COUNT);
    int randomPos, i, j = 0;

    for (i = 3; i > 0; i--)
    {
        for (j = 3; j > 0; j--)
        {
            if (startPosX + i - 2 >= 0 && startPosX + i - 2 < GRID_SIZE && startPosY + j - 2 >= 0 && startPosY + j - 2 < GRID_SIZE)
            {
                //printf("indice [%d]: %d\n", (startPosY + i - 2) * 10 + startPosX + j - 2, freeIndex.point[(startPosY + i - 2) * 10 + startPosX + j - 2]);
                removeAt(&freeIndex, (startPosY + i - 2) * 10 + startPosX + j - 2);
            }

        }
    }

    for (i = 0; i < BOMB_COUNT; i++)
    {
        randomPos = rand() % (freeIndex.size - i - 9);
        //printf("RandomPos : %d    Coordonnes :%d | %d\n", randomPos ,freeIndex.point[randomPos] / 10, freeIndex.point[randomPos] % 10);
        grid[freeIndex.point[randomPos] / 10][freeIndex.point[randomPos] % 10] = GRID_BOMB_CASE;
        removeAt(&freeIndex, randomPos);
    }

    free(freeIndex.point);
    return 0;
}

int bombArround(int grid[GRID_SIZE][GRID_SIZE], int posX, int posY)
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
                if ((posX + i - 1 >= 0 && posX + i - 1 < GRID_SIZE && posY + j - 1 >= 0 && posY + j - 1 < GRID_SIZE) && (grid[posY + j - 1][posX + i - 1] == GRID_BOMB_CASE || grid[posY + j - 1][posX + i - 1] == GRID_FLAG_BOMB_CASE))
                {
                    bombCount++;
                }
            }
        }
    }

    return bombCount;
}

int findSafeCase(int grid[GRID_SIZE][GRID_SIZE], int posX, int posY)
{
    int selfBombArround = bombArround(grid, posX, posY);
    int i = 0;
    int j = 0;

    if (grid[posY][posX] == GRID_BOMB_CASE || grid[posY][posX] == GRID_FLAG_BOMB_CASE)
        return 1;
    if (selfBombArround != 0)
    {
        grid[posY][posX] = selfBombArround + GRID_ONE_CASE - 1;
        return 0;
    }

    grid[posY][posX] = GRID_SHOW_CASE;
    for (i = 0; i < 3; i++)
    {
        for (j = 0; j < 3; j++)
        {
            if (i - 1 != 0 || j - 1 != 0)
            {
                if ((posX + i - 1 >= 0 && posX + i - 1 < GRID_SIZE && posY + j - 1 >= 0 && posY + j - 1 < GRID_SIZE) && grid[posY + j - 1][posX + i - 1] == GRID_HIDE_CASE)
                {
                    findSafeCase(grid, posX + i - 1, posY + j - 1);
                }
            }
        }
    }
    return 0;
}

int isVictory(int grid[GRID_SIZE][GRID_SIZE])
{
    int i = 0;
    int j = 0;

    for (i = 0; i < GRID_SIZE; i++)
    {
        for (j = 0; j < GRID_SIZE; j++)
        {
            if (grid[i][j] == GRID_HIDE_CASE)
                return 1;
        }
    }
    return 0;
}