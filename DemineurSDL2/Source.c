#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <SDL.h>
#include "Array.h"

typedef struct stTexture
{
    SDL_Texture* texture;
    SDL_Rect rectangle;
} stTexture;

#define WINDOW_SIZE_X 1050
#define WINDOW_SIZE_Y 700

#define SIDE_PADDING 0

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
    y:20 / x:24$
*/

int menu(SDL_Window* window, SDL_Renderer* renderer, stTexture* TAB_TEXTURE);
void displayMenu(int state, SDL_Window* window, SDL_Renderer* renderer, stTexture* TAB_TEXTURE);

void gameSetup(int** grid, int GRID_SIZE_Y, int GRID_SIZE_X, int BOMB_COUNT, SDL_Window* window, SDL_Renderer* renderer, stTexture* TAB_TEXTURE);
void displayGrid(int** grid, int GRID_SIZE_Y, int GRID_SIZE_X, int loose, int frame, int state, SDL_Window* window, SDL_Renderer* renderer, stTexture* TAB_TEXTURE);
int setBomb(int** grid, int GRID_SIZE_Y, int GRID_SIZE_X, int BOMB_COUNT, int startPosX, int startPosY);
int game(int** grid, int GRID_SIZE_Y, int GRID_SIZE_X, int BOMB_COUNT, SDL_Window* window, SDL_Renderer* renderer, stTexture* TAB_TEXTURE);
void displayFlagCount(int flagCount, int BOMB_COUNT, SDL_Window* window, SDL_Renderer* renderer, stTexture* TAB_TEXTURE);
int findSafeCase(int** grid, int GRID_SIZE_Y, int GRID_SIZE_X, int posX, int posY);
int bombArround(int** grid, int GRID_SIZE_Y, int GRID_SIZE_X, int posX, int posY);
int isVictory(int** grid, int GRID_SIZE_Y, int GRID_SIZE_X);
int replay(SDL_Window* window, SDL_Renderer* renderer, stTexture* TAB_TEXTURE, int loose);

void timer(time, frameRate, step);
void DestroyWindowAndRenderer(SDL_Window* window, SDL_Renderer* renderer, const char* message);
void ExitWithError(const char* message);

int main(void)
{
    int i, j = 0;
    int menuOutput, gameOutput = 0;
    int GRID_SIZE_X = 0;
    int GRID_SIZE_Y = 0;
    int BOMB_COUNT = 0;

    //-----------------SDL_INIT-----------------//

    SDL_Window* window = NULL;
    SDL_Renderer* renderer = NULL;

    if (SDL_Init(SDL_INIT_VIDEO) != 0)
    {
        ExitWithError("Initialisation SDL echouee");
    }

    if (window = SDL_CreateWindowAndRenderer(WINDOW_SIZE_X, WINDOW_SIZE_Y, 0, &window, &renderer) != 0)
    {
        ExitWithError("Creation fenetre et rendu echoues");
    }

    //------------------------------------------//
    // 
    //------------------------------------------// Image init

    char* srcTab[44];
    srcTab[0] = "src/background.bmp";
    srcTab[1] = "src/bomb.bmp";
    srcTab[2] = "src/bomb2.bmp";
    srcTab[3] = "src/bomb3.bmp";
    srcTab[4] = "src/bomb4.bmp";
    srcTab[5] = "src/button.bmp";
    srcTab[6] = "src/button2.bmp";
    srcTab[7] = "src/eight.bmp";
    srcTab[8] = "src/exit.bmp";
    srcTab[9] = "src/five.bmp";
    srcTab[10] = "src/flag.bmp";
    srcTab[11] = "src/flag2.bmp";
    srcTab[12] = "src/four.bmp";
    srcTab[13] = "src/gameover.bmp";
    srcTab[14] = "src/hide.bmp";
    srcTab[15] = "src/hyperball1.bmp";
    srcTab[16] = "src/hyperball2.bmp";
    srcTab[17] = "src/masterball1.bmp";
    srcTab[18] = "src/masterball2.bmp";
    srcTab[19] = "src/one.bmp";
    srcTab[20] = "src/pokeball.bmp";
    srcTab[21] = "src/pokeball1.bmp";
    srcTab[22] = "src/pokeball2.bmp";
    srcTab[23] = "src/retry.bmp";
    srcTab[24] = "src/seven.bmp";
    srcTab[25] = "src/show.bmp";
    srcTab[26] = "src/six.bmp";
    srcTab[27] = "src/superball1.bmp";
    srcTab[28] = "src/superball2.bmp";
    srcTab[29] = "src/three.bmp";
    srcTab[30] = "src/two.bmp";
    srcTab[31] = "src/win.bmp";
    srcTab[32] = "src/zincgrey.bmp";
    srcTab[33] = "src/0.bmp";
    srcTab[34] = "src/1.bmp";
    srcTab[35] = "src/2.bmp";
    srcTab[36] = "src/3.bmp";
    srcTab[37] = "src/4.bmp";
    srcTab[38] = "src/5.bmp";
    srcTab[39] = "src/6.bmp";
    srcTab[40] = "src/7.bmp";
    srcTab[41] = "src/8.bmp";
    srcTab[42] = "src/9.bmp";
    srcTab[43] = "src/less.bmp";

    stTexture TAB_TEXTURE[44];

    SDL_Surface* imageCase = NULL;
    SDL_Texture* texture = NULL;
    SDL_Rect rectangle;

    for (i = 0; i < 44; i++)
    {
        imageCase = SDL_LoadBMP(srcTab[i]);

        if (imageCase == NULL)
            return 1;

        texture = SDL_CreateTextureFromSurface(renderer, imageCase);

        if (SDL_QueryTexture(texture, NULL, NULL, &rectangle.w, &rectangle.h) != 0)
            return 1;

        TAB_TEXTURE[i].texture = texture;
        TAB_TEXTURE[i].rectangle = rectangle;
    }
    SDL_FreeSurface(imageCase);

    //------------------------------------------//
    // 
    //------------------------------------------// difficulty

    while (gameOutput == 0)
    {
        menuOutput = menu(window, renderer, TAB_TEXTURE);

        GRID_SIZE_Y = 8 + 3 * menuOutput;
        int** grid = malloc(sizeof(int*) * (GRID_SIZE_Y));

        if (menuOutput == 0)
        {
            BOMB_COUNT = 10;
            GRID_SIZE_X = 10;
        }
        else if (menuOutput == 1)
        {
            BOMB_COUNT = 40;
            GRID_SIZE_X = 14;
        }
        else if (menuOutput == 2)
        {
            BOMB_COUNT = 50;
            GRID_SIZE_X = 18;
        }
        else if (menuOutput == 3)
        {
            BOMB_COUNT = 368;
            GRID_SIZE_Y = 18;
            GRID_SIZE_X = 21;
        }
        else
        {
            DestroyWindowAndRenderer(window, renderer, "Grille non initialisee");
            break;
        }

        for (int i = 0; i < GRID_SIZE_Y; i++)
        {
            grid[i] = malloc(sizeof(int) * GRID_SIZE_X);
        }
        for (i = 0; i < GRID_SIZE_Y; i++)
        {
            for (j = 0; j < GRID_SIZE_X; j++)
            {
                grid[i][j] = 0;
            }
        }

        //------------------------------------------//
        // 
        //------------------------------------------// Jeu

        gameSetup(grid, GRID_SIZE_Y, GRID_SIZE_X, BOMB_COUNT, window, renderer, TAB_TEXTURE);
        gameOutput = game(grid, GRID_SIZE_Y, GRID_SIZE_X, BOMB_COUNT, window, renderer, TAB_TEXTURE);


        for (i = 0; i < GRID_SIZE_Y; i++)
        {
            free(grid[i]);
        }
        free(grid);
    }

    DestroyWindowAndRenderer(&window, &renderer, "None");
    return 0;
}

int menu(SDL_Window* window, SDL_Renderer* renderer, stTexture* TAB_TEXTURE)
{
    SDL_bool program_launched = SDL_TRUE;
    int tryPlayerPos[2] = { 0 };

    // Step 1
    displayMenu(5, window, renderer, TAB_TEXTURE);

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
                    if ((tryPlayerPos[0] >= (WINDOW_SIZE_X) / 2 - 85 && tryPlayerPos[0] < (WINDOW_SIZE_X) / 2 + 85) && (tryPlayerPos[1] >= (WINDOW_SIZE_Y)-277 && tryPlayerPos[1] < (WINDOW_SIZE_Y)-180))
                    {
                        displayMenu(6, window, renderer, TAB_TEXTURE);
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

    displayMenu(20, window, renderer, TAB_TEXTURE);

    // Step 2 (difficulty)
    while (program_launched)
    {
        SDL_Event event;

        while (SDL_PollEvent(&event))
        {
            switch (event.type)
            {
            case SDL_MOUSEMOTION:
                tryPlayerPos[0] = event.motion.x;
                tryPlayerPos[1] = event.motion.y;
                if (tryPlayerPos[1] >= (WINDOW_SIZE_Y)-320 && tryPlayerPos[1] < (WINDOW_SIZE_Y)-223)
                {
                    if (tryPlayerPos[0] >= 86 - 15 && tryPlayerPos[0] < 183 - 15)
                        displayMenu(21, window, renderer, TAB_TEXTURE);
                    else if (tryPlayerPos[0] >= 356 - 15 && tryPlayerPos[0] < 453 - 15)
                        displayMenu(27, window, renderer, TAB_TEXTURE);
                    else if (tryPlayerPos[0] >= 626 - 15 && tryPlayerPos[0] < 723 - 15)
                        displayMenu(15, window, renderer, TAB_TEXTURE);
                    else if (tryPlayerPos[0] >= 896 - 15 && tryPlayerPos[0] < 993 - 15)
                        displayMenu(17, window, renderer, TAB_TEXTURE);
                    else
                        displayMenu(20, window, renderer, TAB_TEXTURE);
                }
                else
                    displayMenu(20, window, renderer, TAB_TEXTURE);

                break;
            case SDL_MOUSEBUTTONDOWN:
                if (event.button.button == SDL_BUTTON_LEFT)
                {
                    tryPlayerPos[0] = event.button.x;
                    tryPlayerPos[1] = event.button.y;
                    if (tryPlayerPos[1] >= (WINDOW_SIZE_Y)-320 && tryPlayerPos[1] < (WINDOW_SIZE_Y)-223)
                    {
                        if (tryPlayerPos[0] >= 86 - 15 && tryPlayerPos[0] < 183 - 15)
                        {
                            displayMenu(22, window, renderer, TAB_TEXTURE);
                            SDL_Delay(300);
                            return 0;
                        }
                        if (tryPlayerPos[0] >= 356 - 15 && tryPlayerPos[0] < 453 - 15)
                        {
                            displayMenu(28, window, renderer, TAB_TEXTURE);
                            SDL_Delay(300);
                            return 1;
                        }
                        if (tryPlayerPos[0] >= 626 - 15 && tryPlayerPos[0] < 723 - 15)
                        {
                            displayMenu(16, window, renderer, TAB_TEXTURE);
                            SDL_Delay(300);
                            return 2;
                        }
                        if (tryPlayerPos[0] >= 896 - 15 && tryPlayerPos[0] < 993 - 15)
                        {
                            displayMenu(18, window, renderer, TAB_TEXTURE);
                            SDL_Delay(300);
                            return 3;
                        }
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
}

void displayMenu(int state, SDL_Window* window, SDL_Renderer* renderer, stTexture* TAB_TEXTURE)
{
    TAB_TEXTURE[0].rectangle.x = 0;
    TAB_TEXTURE[0].rectangle.y = 0;

    if (SDL_RenderCopy(renderer, TAB_TEXTURE[0].texture, NULL, &TAB_TEXTURE[0].rectangle) != 0)
        DestroyWindowAndRenderer(&window, &renderer, "Impossible d'afficher \"texture\"");


    TAB_TEXTURE[state].rectangle.x = (WINDOW_SIZE_X) / 2 - TAB_TEXTURE[state].rectangle.w / 2;
    TAB_TEXTURE[state].rectangle.y = (WINDOW_SIZE_Y)-180 - TAB_TEXTURE[state].rectangle.h;

    if (SDL_RenderCopy(renderer, TAB_TEXTURE[state].texture, NULL, &TAB_TEXTURE[state].rectangle) != 0)
        DestroyWindowAndRenderer(&window, &renderer, "Impossible d'afficher \"texture\"");

    SDL_RenderPresent(renderer);
}

void gameSetup(int** grid, int GRID_SIZE_Y, int GRID_SIZE_X, int BOMB_COUNT, SDL_Window* window, SDL_Renderer* renderer, stTexture* TAB_TEXTURE)
{
    TAB_TEXTURE[32].rectangle.x = 0;
    TAB_TEXTURE[32].rectangle.y = 0;

    if (SDL_RenderCopy(renderer, TAB_TEXTURE[32].texture, NULL, &TAB_TEXTURE[32].rectangle) != 0)
        DestroyWindowAndRenderer(&window, &renderer, "Impossible d'afficher \"texture\"");

    int tryPlayerPos[2] = { 0 };
    SDL_bool program_launched = SDL_TRUE;

    displayGrid(grid, GRID_SIZE_Y, GRID_SIZE_X, 0, SDL_GetTicks(), 0, window, renderer, TAB_TEXTURE);
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
                    if ((tryPlayerPos[0] >= 0 && tryPlayerPos[0] < GRID_SIZE_X) && (tryPlayerPos[1] >= 0 && tryPlayerPos[1] < GRID_SIZE_Y))
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

    if (setBomb(grid, GRID_SIZE_Y, GRID_SIZE_X, BOMB_COUNT, tryPlayerPos[0], tryPlayerPos[1]) == 1)
    {
        printf("Erreur: setBomb echoue");
        return 1;
    }

    if (findSafeCase(grid, GRID_SIZE_Y, GRID_SIZE_X, tryPlayerPos[0], tryPlayerPos[1]) == 1)
    {
        displayGrid(grid, GRID_SIZE_Y, GRID_SIZE_X, 1, SDL_GetTicks(), 0, window, renderer, TAB_TEXTURE);
        printf("Game Over!");
        SDL_Delay(3000);
        DestroyWindowAndRenderer(&window, &renderer, "None");
        return 1;
    }

    if (isVictory(grid, GRID_SIZE_Y, GRID_SIZE_X) == 0)
    {
        printf("Bravo!");
        SDL_Delay(3000);
        DestroyWindowAndRenderer(&window, &renderer, "None");
        return 0;
    }
}

void displayGrid(int** grid, int GRID_SIZE_Y, int GRID_SIZE_X, int loose, int frame, int state, SDL_Window* window, SDL_Renderer* renderer, stTexture* TAB_TEXTURE)
{
    int i, j, imgNumber = 0;

    for (i = 0; i < GRID_SIZE_Y; i++) {
        for (j = 0; j < GRID_SIZE_X; j++) {

            switch (grid[i][j])
            {
            case GRID_HIDE_CASE:
                imgNumber = 14;
                break;

            case GRID_SHOW_CASE:
                imgNumber = 25;
                break;

            case GRID_BOMB_CASE:
                if (loose == 1)
                    imgNumber = 1 + state;
                else
                    imgNumber = 14;
                break;

            case GRID_FLAG_BOMB_CASE:
                if (loose == 1)
                    imgNumber = 1 + state;
                else
                {
                    if (frame == 0 || frame == 2)
                        imgNumber = 14;
                    else if (frame == 1)
                        imgNumber = 10;
                    else
                        imgNumber = 11;
                }
                break;

            case GRID_FLAG_HIDE_CASE:
                if (frame == 0 || frame == 2)
                    imgNumber = 14;
                else if (frame == 1)
                    imgNumber = 10;
                else
                    imgNumber = 11;
                break;

            case GRID_ONE_CASE:
                imgNumber = 19;
                break;

            case GRID_TWO_CASE:
                imgNumber = 30;
                break;

            case GRID_THREE_CASE:
                imgNumber = 29;
                break;

            case GRID_FOUR_CASE:
                imgNumber = 12;
                break;

            case GRID_FIVE_CASE:
                imgNumber = 9;
                break;

            case GRID_SIX_CASE:
                imgNumber = 26;
                break;

            case GRID_SEVEN_CASE:
                imgNumber = 24;
                break;

            case GRID_EIGHT_CASE:
                imgNumber = 7;
                break;

            default:
                break;
            }

            TAB_TEXTURE[imgNumber].rectangle.y = 50 * i;
            TAB_TEXTURE[imgNumber].rectangle.x = 50 * j + SIDE_PADDING;

            if (SDL_RenderCopy(renderer, TAB_TEXTURE[imgNumber].texture, NULL, &TAB_TEXTURE[imgNumber].rectangle) != 0)
                DestroyWindowAndRenderer(&window, &renderer, "Impossible d'afficher \"texture\"");
        }
    }
    SDL_RenderPresent(renderer);

    if (loose == 1 && state != 3)
    {
        SDL_Delay(200);
        displayGrid(grid, GRID_SIZE_Y, GRID_SIZE_X, loose, frame, state + 1, window, renderer, TAB_TEXTURE);
    }
}

int setBomb(int** grid, int GRID_SIZE_Y, int GRID_SIZE_X, int BOMB_COUNT, int startPosX, int startPosY)
{
    srand(time(NULL));
    const int FREE_CASE_COUNT = (GRID_SIZE_Y) * (GRID_SIZE_X);
    if (FREE_CASE_COUNT < BOMB_COUNT)
        return 1;
    stArray freeIndex = createTab(FREE_CASE_COUNT);
    int randomPos, i, j = 0;

    for (i = 3; i > 0; i--)
    {
        for (j = 3; j > 0; j--)
        {
            if (startPosX + i - 2 >= 0 && startPosX + i - 2 < GRID_SIZE_X && startPosY + j - 2 >= 0 && startPosY + j - 2 < GRID_SIZE_Y)
            {
                removeAt(&freeIndex, (startPosY + i - 2) * GRID_SIZE_X + startPosX + j - 2);
            }

        }
    }

    for (i = 0; i < BOMB_COUNT; i++)
    {
        randomPos = rand() % (freeIndex.size - i - 9);
        grid[freeIndex.point[randomPos] / GRID_SIZE_X][freeIndex.point[randomPos] % GRID_SIZE_X] = GRID_BOMB_CASE;
        removeAt(&freeIndex, randomPos);
    }

    free(freeIndex.point);
    return 0;
}

int game(int** grid, int GRID_SIZE_Y, int GRID_SIZE_X,int BOMB_COUNT, SDL_Window* window, SDL_Renderer* renderer, stTexture* TAB_TEXTURE)
{
    int returnValue = 0;
    int time = 0;
    int frame = 0;
    int flagCount = 0;
    int tryPlayerPos[2] = { 0 };
    SDL_bool program_launched = SDL_TRUE;

    displayGrid(grid, GRID_SIZE_Y, GRID_SIZE_X, 0, SDL_GetTicks(), 0, window, renderer, TAB_TEXTURE);

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
                    if ((tryPlayerPos[0] >= 0 && tryPlayerPos[0] < GRID_SIZE_X) && (tryPlayerPos[1] >= 0 && tryPlayerPos[1] < GRID_SIZE_Y))
                    {
                        if (findSafeCase(grid, GRID_SIZE_Y, GRID_SIZE_X, tryPlayerPos[0], tryPlayerPos[1]) == 1)
                        {
                            displayGrid(grid, GRID_SIZE_Y, GRID_SIZE_X, 1, SDL_GetTicks(), 0, window, renderer, TAB_TEXTURE);
                            SDL_Delay(1000);
                            returnValue = replay(window, renderer, TAB_TEXTURE, 1);
                            return returnValue;
                        }

                        if (isVictory(grid, GRID_SIZE_Y, GRID_SIZE_X) == 0)
                        {
                            SDL_Delay(1000);
                            returnValue = replay(window, renderer, TAB_TEXTURE, 0);
                            return returnValue;
                        }
                        displayGrid(grid, GRID_SIZE_Y, GRID_SIZE_X, 0, SDL_GetTicks(), 0, window, renderer, TAB_TEXTURE);
                    }
                }
                if (event.button.button == SDL_BUTTON_RIGHT)
                {
                    if ((tryPlayerPos[0] >= 0 && tryPlayerPos[0] < GRID_SIZE_X) && (tryPlayerPos[1] >= 0 && tryPlayerPos[1] < GRID_SIZE_Y))
                    {
                        switch (grid[tryPlayerPos[1]][tryPlayerPos[0]])
                        {
                        case GRID_HIDE_CASE:
                            grid[tryPlayerPos[1]][tryPlayerPos[0]] = GRID_FLAG_HIDE_CASE;
                            flagCount++;
                            displayFlagCount(flagCount, BOMB_COUNT, window, renderer, TAB_TEXTURE);
                            break;

                        case GRID_BOMB_CASE:
                            grid[tryPlayerPos[1]][tryPlayerPos[0]] = GRID_FLAG_BOMB_CASE;
                            flagCount++;
                            displayFlagCount(flagCount, BOMB_COUNT, window, renderer, TAB_TEXTURE);
                            break;

                        case GRID_FLAG_HIDE_CASE:
                            grid[tryPlayerPos[1]][tryPlayerPos[0]] = GRID_HIDE_CASE;
                            flagCount--;
                            displayFlagCount(flagCount, BOMB_COUNT, window, renderer, TAB_TEXTURE);
                            break;

                        case GRID_FLAG_BOMB_CASE:
                            grid[tryPlayerPos[1]][tryPlayerPos[0]] = GRID_BOMB_CASE;
                            flagCount--;
                            displayFlagCount(flagCount, BOMB_COUNT, window, renderer, TAB_TEXTURE);
                            break;

                        default:
                            break;
                        }
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
        if (time + 150 <= SDL_GetTicks())
        {
            time = SDL_GetTicks();
            if (frame == 3)
                frame -= 3;
            else
                frame++;
            displayGrid(grid, GRID_SIZE_Y, GRID_SIZE_X, 0, frame, 0, window, renderer, TAB_TEXTURE);
        }
    }
    return 1;
}

void displayFlagCount(int flagCount, int BOMB_COUNT, SDL_Window* window, SDL_Renderer* renderer, stTexture* TAB_TEXTURE)
{
    if (flagCount < BOMB_COUNT)
    {
        TAB_TEXTURE[33 + ((BOMB_COUNT - flagCount) / 100)].rectangle.x = (WINDOW_SIZE_X)-150;
        TAB_TEXTURE[33 + ((BOMB_COUNT - flagCount) / 100)].rectangle.y = 50;

        if (SDL_RenderCopy(renderer, TAB_TEXTURE[33 + ((BOMB_COUNT - flagCount) / 100)].texture, NULL, &TAB_TEXTURE[33 + ((BOMB_COUNT - flagCount) / 100)].rectangle) != 0)
            DestroyWindowAndRenderer(&window, &renderer, "Impossible d'afficher \"texture\"");

        TAB_TEXTURE[33 + (((BOMB_COUNT - flagCount) % 100) / 10)].rectangle.x = (WINDOW_SIZE_X)-100;
        TAB_TEXTURE[33 + (((BOMB_COUNT - flagCount) % 100) / 10)].rectangle.y = 50;

        if (SDL_RenderCopy(renderer, TAB_TEXTURE[33 + (((BOMB_COUNT - flagCount) % 100) / 10)].texture, NULL, &TAB_TEXTURE[33 + (((BOMB_COUNT - flagCount) % 100) / 10)].rectangle) != 0)
            DestroyWindowAndRenderer(&window, &renderer, "Impossible d'afficher \"texture\"");

        TAB_TEXTURE[33 + ((BOMB_COUNT - flagCount) % 10)].rectangle.x = (WINDOW_SIZE_X) - 50;
        TAB_TEXTURE[33 + ((BOMB_COUNT - flagCount) % 10)].rectangle.y = 50;

        if (SDL_RenderCopy(renderer, TAB_TEXTURE[33 + ((BOMB_COUNT - flagCount) % 10)].texture, NULL, &TAB_TEXTURE[33 + ((BOMB_COUNT - flagCount) % 10)].rectangle) != 0)
            DestroyWindowAndRenderer(&window, &renderer, "Impossible d'afficher \"texture\"");
    }
    else
    {
        TAB_TEXTURE[43].rectangle.x = (WINDOW_SIZE_X)-150;
        TAB_TEXTURE[43].rectangle.y = 50;

        if (SDL_RenderCopy(renderer, TAB_TEXTURE[43].texture, NULL, &TAB_TEXTURE[43].rectangle) != 0)
            DestroyWindowAndRenderer(&window, &renderer, "Impossible d'afficher \"texture\"");

        TAB_TEXTURE[33 - (((BOMB_COUNT - flagCount) % 100) / 10)].rectangle.x = (WINDOW_SIZE_X)-100;
        TAB_TEXTURE[33 - (((BOMB_COUNT - flagCount) % 100) / 10)].rectangle.y = 50;

        if (SDL_RenderCopy(renderer, TAB_TEXTURE[33 - (((BOMB_COUNT - flagCount) % 100) / 10)].texture, NULL, &TAB_TEXTURE[33 - (((BOMB_COUNT - flagCount) % 100) / 10)].rectangle) != 0)
            DestroyWindowAndRenderer(&window, &renderer, "Impossible d'afficher \"texture\"");

        TAB_TEXTURE[33 - ((BOMB_COUNT - flagCount) % 10)].rectangle.x = (WINDOW_SIZE_X)-50;
        TAB_TEXTURE[33 - ((BOMB_COUNT - flagCount) % 10)].rectangle.y = 50;

        if (SDL_RenderCopy(renderer, TAB_TEXTURE[33 - ((BOMB_COUNT - flagCount) % 10)].texture, NULL, &TAB_TEXTURE[33 - ((BOMB_COUNT - flagCount) % 10)].rectangle) != 0)
            DestroyWindowAndRenderer(&window, &renderer, "Impossible d'afficher \"texture\"");
    }
}

int findSafeCase(int** grid, int GRID_SIZE_Y, int GRID_SIZE_X, int posX, int posY)
{
    int selfBombArround = bombArround(grid, GRID_SIZE_Y, GRID_SIZE_X, posX, posY);
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
                if ((posX + i - 1 >= 0 && posX + i - 1 < GRID_SIZE_X && posY + j - 1 >= 0 && posY + j - 1 < GRID_SIZE_Y) && grid[posY + j - 1][posX + i - 1] == GRID_HIDE_CASE)
                {
                    findSafeCase(grid, GRID_SIZE_Y, GRID_SIZE_X, posX + i - 1, posY + j - 1);
                }
            }
        }
    }
    return 0;
}

int bombArround(int** grid, int GRID_SIZE_Y, int GRID_SIZE_X, int posX, int posY)
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
                if ((posX + i - 1 >= 0 && posX + i - 1 < GRID_SIZE_X && posY + j - 1 >= 0 && posY + j - 1 < GRID_SIZE_Y) && (grid[posY + j - 1][posX + i - 1] == GRID_BOMB_CASE || grid[posY + j - 1][posX + i - 1] == GRID_FLAG_BOMB_CASE))
                {
                    bombCount++;
                }
            }
        }
    }

    return bombCount;
}

int isVictory(int** grid, int GRID_SIZE_Y, int GRID_SIZE_X)
{
    int i = 0;
    int j = 0;

    for (i = 0; i < GRID_SIZE_Y; i++)
    {
        for (j = 0; j < GRID_SIZE_X; j++)
        {
            if (grid[i][j] == GRID_HIDE_CASE)
                return 1;
        }
    }
    return 0;
}

int replay(SDL_Window* window, SDL_Renderer* renderer, stTexture* TAB_TEXTURE, int loose)
{
    SDL_bool program_launched = SDL_TRUE;
    int tryPlayerPos[2] = { 0 };
    int imgNumber = 0;

    if (loose == 1)
        imgNumber = 13;
    else
        imgNumber = 31;

    TAB_TEXTURE[imgNumber].rectangle.x = (WINDOW_SIZE_X) / 2 - TAB_TEXTURE[imgNumber].rectangle.w / 2;
    TAB_TEXTURE[imgNumber].rectangle.y = (WINDOW_SIZE_Y)-300 - TAB_TEXTURE[imgNumber].rectangle.h;

    if (SDL_RenderCopy(renderer, TAB_TEXTURE[imgNumber].texture, NULL, &TAB_TEXTURE[imgNumber].rectangle) != 0)
        DestroyWindowAndRenderer(&window, &renderer, "Impossible d'afficher \"texture\"");

    TAB_TEXTURE[23].rectangle.x = (WINDOW_SIZE_X) / 2 - TAB_TEXTURE[23].rectangle.w - 20;
    TAB_TEXTURE[23].rectangle.y = (WINDOW_SIZE_Y)-50 - TAB_TEXTURE[23].rectangle.h;

    if (SDL_RenderCopy(renderer, TAB_TEXTURE[23].texture, NULL, &TAB_TEXTURE[23].rectangle) != 0)
        DestroyWindowAndRenderer(&window, &renderer, "Impossible d'afficher \"texture\"");

    TAB_TEXTURE[8].rectangle.x = (WINDOW_SIZE_X) / 2 + 20;
    TAB_TEXTURE[8].rectangle.y = (WINDOW_SIZE_Y)-50 - TAB_TEXTURE[8].rectangle.h;

    if (SDL_RenderCopy(renderer, TAB_TEXTURE[8].texture, NULL, &TAB_TEXTURE[8].rectangle) != 0)
        DestroyWindowAndRenderer(&window, &renderer, "Impossible d'afficher \"texture\"");

    SDL_RenderPresent(renderer);



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
                    if (tryPlayerPos[1] >= (WINDOW_SIZE_Y)-50 - TAB_TEXTURE[23].rectangle.h && tryPlayerPos[1] < (WINDOW_SIZE_Y)-50)
                    {
                        if (tryPlayerPos[0] >= (WINDOW_SIZE_X) / 2 - TAB_TEXTURE[23].rectangle.w - 20 && tryPlayerPos[0] < (WINDOW_SIZE_X) / 2 - 20)
                        {
                            SDL_Delay(100);
                            return 0;
                        }
                        if (tryPlayerPos[0] >= (WINDOW_SIZE_X) / 2 + 20 && tryPlayerPos[0] < (WINDOW_SIZE_X) / 2 + TAB_TEXTURE[23].rectangle.w + 20)
                        {
                            SDL_Delay(100);
                            return 1;
                        }
                    }
                }
                break;

            case SDL_QUIT:
                program_launched = SDL_FALSE;
                DestroyWindowAndRenderer(&window, &renderer, "None");
                return 1;

            default:
                break;
            }
        }
    }
    return 0;
}

void timer(time, frameRate, step)
{
    if (time <= frameRate * step)
        SDL_Delay((frameRate * step) - time);
    return;
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