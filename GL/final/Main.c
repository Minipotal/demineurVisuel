#include <stdio.h> 
#include <SDL.h>
#include <stdlib.h>
#include <conio.h>
#include <time.h>
#include <windows.h>
#include <SDL_mixer.h>

int size = 10;

/* define Tile Struct*/
typedef struct Tile
{
    int type;
    int isbomb;
    int see;
    int autour;
    int check;
    int event;

}Tile;


/* All functions declaration*/
void refill(Tile** grid);
int victory(Tile** grid, int bombe, int finish, int* V, SDL_Window* window, SDL_Renderer* renderer);
void modify(Tile** grid, int bombe);
int play(Tile** grid, int* caseLibres, int* finish, int bombe, int* drapeau, int * result, int* nbrindice, SDL_Window* window, SDL_Renderer* renderer);
void first(Tile** grid, int i, int j, int* caseLibres);
void minesAround(Tile** grid);
void show(Tile** grid, int finish, SDL_Window* window, SDL_Renderer* renderer, SDL_Texture* tvide, SDL_Texture* tbombe, SDL_Texture* tdrapeau, SDL_Texture* tzero,
    SDL_Texture* tun, SDL_Texture* tdeux, SDL_Texture* ttrois, SDL_Texture* tquatre, SDL_Texture* tcinq, SDL_Texture* tsix, SDL_Texture* tsept, SDL_Texture* thuit,
    SDL_Texture* animation[10]);
void exitError(const char* Message);
void destruct(SDL_Renderer* renderer, SDL_Window* window);
void ninvisible(Tile** grid);
void menu(int* bombe);
void game();


/* main function */
int main()
{
    game();
    return 0;
}

/* Scan around each point the amount of mines */
void minesAround(Tile** grid) {

    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            if (grid[i][j].isbomb == 0) {
                int count = 0;
                for (int x = i - 1; x <= i + 1; x++) {
                    for (int y = j - 1; y <= j + 1; y++) {
                        if (x >= 0 && x < size && y >= 0 && y < size && grid[x][y].isbomb == 1) {
                            count++;
                        }
                    }
                }
                grid[i][j].autour = count;
            }
        }
    }
}

/* Say if a victory condition is respected */
int victory(Tile** grid, int bombe, int finish, int* V, SDL_Window* window, SDL_Renderer* renderer) {

    int nobomb = 0;
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++)
        {
            if (grid[i][j].isbomb == 0 && grid[i][j].see == 1) {
                nobomb++;
            }
        }
    }
    if (nobomb == ((size * size) - bombe)) {
        *V = 0;
        return *V;
    }
    else {
        return *V;
    }

}

/* Auto solver */
void tristan(Tile** grid, int* caseLibres)
{
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            if (grid[i][j].isbomb == 0) {
                grid[i][j].see = 1;
                *caseLibres -= 1;
            }
        }
    }
}

/* Complete each tile from the grid with default settings */
void refill(Tile** grid) {
    for (int i = 0; i < size; i++)
    {
        for (int j = 0; j < size; j++)
        {
            grid[i][j].type = 0;
            grid[i][j].isbomb = 0;
            grid[i][j].see = 0;
            grid[i][j].autour = 0;
            grid[i][j].check = 0;
            grid[i][j].event = 0;
        }
    }
}

/* Create bomb placement, increase numbers around bomb location */
void modify(Tile** grid, int bombe) {
    refill(grid);
    srand((unsigned int)time(NULL));
    int compteurBombe = 0;
    while (1)
    {
        int i = rand() % size;
        int j = rand() % size;

        if (grid[i][j].isbomb == 0)
        {
            grid[i][j].isbomb = 1;
            compteurBombe += 1;
        }

        if (compteurBombe == bombe)
        {
            break;
        }
    }
    minesAround(grid);
}

/* Discovery function for every 0 linked to the played tile */
void first(Tile** grid, int i, int j, int* caseLibres)
{
    if (grid[i][j].autour == 0) {
        if ((i - 1) >= 0 && grid[i - 1][j].check == 0)
        {
            if (grid[i - 1][j].isbomb == 0)
            {
                grid[i - 1][j].check = 1;
                grid[i - 1][j].see = 1;
                *caseLibres -= 1;
                if (grid[i - 1][j].autour == 0) {
                    first(grid, i - 1, j, caseLibres);
                }
            }
        }
        if ((i + 1) < size && grid[i + 1][j].check == 0)
        {
            if (grid[i + 1][j].isbomb == 0)
            {
                grid[i + 1][j].check = 1;
                grid[i + 1][j].see = 1;
                *caseLibres -= 1;
                if (grid[i + 1][j].autour == 0) {
                    first(grid, i + 1, j, caseLibres);
                }
            }
        }
        if ((j + 1) < size && grid[i][j + 1].check == 0)
        {
            if (grid[i][j + 1].isbomb == 0)
            {
                grid[i][j + 1].check = 1;
                grid[i][j + 1].see = 1;
                *caseLibres -= 1;
                if (grid[i][j + 1].autour == 0) {
                    first(grid, i, j + 1, caseLibres);
                }
            }
        }
        if ((j - 1) >= 0 && grid[i][j - 1].check == 0)
        {
            if (grid[i][j - 1].isbomb == 0)
            {
                grid[i][j - 1].check = 1;
                grid[i][j - 1].see = 1;
                *caseLibres -= 1;
                if (grid[i][j - 1].autour == 0) {
                    first(grid, i, j - 1, caseLibres);
                }
            }
        }
        if ((i + 1) < size && (j + 1) < size && grid[i + 1][j + 1].check == 0)
        {
            if (grid[i + 1][j + 1].isbomb == 0)
            {
                grid[i + 1][j + 1].check = 1;
                grid[i + 1][j + 1].see = 1;
                *caseLibres -= 1;
                if (grid[i + 1][j + 1].autour == 0) {
                    first(grid, i + 1, j + 1, caseLibres);
                }
            }
        }
        if ((i - 1) >= 0 && (j - 1) >= 0 && grid[i - 1][j - 1].check == 0)
        {
            if (grid[i - 1][j - 1].isbomb == 0)
            {
                grid[i - 1][j - 1].check = 1;
                grid[i - 1][j - 1].see = 1;
                *caseLibres -= 1;
                if (grid[i - 1][j - 1].autour == 0) {
                    first(grid, i - 1, j - 1, caseLibres);
                }
            }
        }
        if ((j - 1) >= 0 && (i + 1) < size && grid[i + 1][j - 1].check == 0)
        {
            if (grid[i + 1][j - 1].isbomb == 0)
            {
                grid[i + 1][j - 1].check = 1;
                grid[i + 1][j - 1].see = 1;
                *caseLibres -= 1;
                if (grid[i + 1][j - 1].autour == 0) {
                    first(grid, i + 1, j - 1, caseLibres);
                }
            }
        }
        if ((i - 1) >= 0 && (j + 1) < size && grid[i - 1][j + 1].check == 0)
        {
            if (grid[i - 1][j + 1].isbomb == 0)
            {
                grid[i - 1][j + 1].check = 1;
                grid[i - 1][j + 1].see = 1;
                *caseLibres -= 1;
                if (grid[i - 1][j + 1].autour == 0) {
                    first(grid, i - 1, j + 1, caseLibres);
                }
            }
        }
    }
}

/* Contain every play case: click, flag and solver*/
int play(Tile** grid, int* finish, int* caseLibres, int bombe, int * result,int* drapeau,int* nbrindice, SDL_Window* window, SDL_Renderer* renderer)
{
    SDL_Event event;
    int V = 1;
    while (SDL_PollEvent(&event))
    {
        switch (event.type) {
        case SDL_QUIT:
            SDL_DestroyWindow(window);
            SDL_Quit();
        case SDL_MOUSEBUTTONDOWN:
            if (event.button.button == SDL_BUTTON_LEFT)
            {
                Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, MIX_DEFAULT_CHANNELS, 1024);
                Mix_Music* musicPlay;
                musicPlay = Mix_LoadMUS("music/playerClick.mp3");
                Mix_PlayMusic(musicPlay, -1);
                Mix_CloseAudio(musicPlay);
                int ligne = event.button.y / 31;
                int col = event.button.x / 31;

                if (grid[ligne][col].isbomb == 1)
                {
                    grid[ligne][col].see = 1;
                    *result = 2;
                    *finish = 1;

                }
                else
                {
                    if (grid[ligne][col].see == 0)
                    {
                        
                        grid[ligne][col].see = 1;
                        *caseLibres -= 1;
                        first(grid, ligne, col, &caseLibres);
                        victory(grid, bombe, finish, &V, window, renderer);
                        if (V == 0)
                        {
                            *result = 1;
                            *finish = 1;
                            return *finish;
                        }
                    }
                    else
                    {
                        *caseLibres += 1;
                    }

                }
            }
            if (event.button.button == SDL_BUTTON_RIGHT)
            {
                int ligne = event.button.y / 31;
                int col = event.button.x / 31;
                {
                    if (*drapeau <= bombe)
                    {
                        if (grid[ligne][col].see == 0)
                        {
                            grid[ligne][col].type = 3;
                            grid[ligne][col].see = 1;
                            *caseLibres -= 1;
                            *drapeau += 1;
                        }
                        else
                        {
                            *caseLibres += 1;
                        }
                    }


                }
            }
        }
        if (SDL_KEYDOWN == event.type)
        {
            if (SDLK_i == event.key.keysym.sym)
            {
                if (*nbrindice != 1)
                {
                    *nbrindice = 1;
                    int produit = 0;
                    int iProduit = 0;
                    int jProduit = 0;
                    while (1)
                    {
                        int i = rand() % size;
                        int j = rand() % size;

                        if (grid[i][j].see == 0 && grid[i][j].isbomb == 0)
                        {
                            grid[i][j].see = 1;
                            iProduit = i + 1;
                            jProduit = j + 1;
                            produit = 1;
                        }

                        if (produit == 1)
                        {
                            break;
                        }
                    }
                    victory(grid, bombe, finish, &V, window, renderer);
                    if (V == 0)
                    {
                        *finish = 1;
                        *result = 1;
                        return *finish;
                    }
                    *caseLibres -= 1;
                }
            }
        }
        if (SDL_KEYDOWN == event.type)
        {
            if (SDLK_t == event.key.keysym.sym)
            {
                tristan(grid, &caseLibres);
                victory(grid, bombe, finish, &V, window, renderer);
                if (V == 0)
                {
                    *finish = 1;
                    *result = 1;
                    return *finish;
                }
            }
        }

    }
    return *finish;
}

/* exit program when error occured*/
void exitError(const char* Message)
{
    SDL_Log("ERREUR : %s > %s\n ", Message, SDL_GetError);
    SDL_Quit();
    exit(EXIT_FAILURE);
}

/* destruct player window after a game*/
void destruct(SDL_Renderer* renderer, SDL_Window* window)
{
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
}

/* show a select menu for players*/
void menu(int* bombe)
{
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS) != 0)
    {
        exitError("Initialisation SDL");
    }

    //Menu
    SDL_Window* menuwindow = NULL;
    SDL_Renderer* menurenderer = NULL;
    int y = 0;
    int True = 1;

    SDL_Rect facileRect = { 0 ,100 , 300 , 100 };
    SDL_Rect moyenRect = { 0 ,200 , 300 , 200 };
    SDL_Rect difficileRect = { 0 ,300 , 300 , 300 };
    SDL_Rect persoRect = { 0 ,400 , 600 , 300 };

    if (SDL_CreateWindowAndRenderer(300, 500, 0, &menuwindow, &menurenderer) != 0)
    {
        exitError("Initialisation fenetre");
    }
    SDL_Event event;
    while (True == 1) {


        SDL_SetRenderDrawColor(menurenderer, 0, 0, 0, 255);
        SDL_RenderClear(menurenderer);
        SDL_SetRenderDrawColor(menurenderer, 0, 255, 0, 255);
        SDL_RenderFillRect(menurenderer, &facileRect);
        SDL_SetRenderDrawColor(menurenderer, 255, 255, 0, 255);
        SDL_RenderFillRect(menurenderer, &moyenRect);
        SDL_SetRenderDrawColor(menurenderer, 255, 0, 0, 255);
        SDL_RenderFillRect(menurenderer, &difficileRect);
        SDL_SetRenderDrawColor(menurenderer, 255, 255, 255, 255);
        SDL_RenderFillRect(menurenderer, &persoRect);

        while (SDL_PollEvent(&event))
        {
            switch (event.type) {
            case SDL_QUIT:
                SDL_DestroyWindow(menuwindow);
                SDL_Quit();
            case SDL_MOUSEBUTTONDOWN:
                if (event.button.button == SDL_BUTTON_LEFT)
                {
                    int y = event.button.y;
                    if (y > 100 && y < 200)
                    {
                        size = 10;
                        *bombe = 10;
                        True = 0;
                    }
                    else if (y > 200 && y < 300)
                    {
                        size = 18;
                        *bombe = 40;
                        True = 0;
                    }
                    else if (y > 300 && y < 400)
                    {
                        size = 24;
                        *bombe = 99;
                        True = 0;
                    }
                    else if (y > 400 && y < 500)
                    {
                        size = 20;
                        int bombe = 10;
                        True = 0;
                    }
                }
            }
        }


        SDL_RenderPresent(menurenderer);
    }
    SDL_DestroyRenderer(menurenderer);
    SDL_DestroyWindow(menuwindow);
    SDL_Quit;
}


/* Initialize, create and show gae window with images */
void show(Tile** grid, int finish, SDL_Window* window, SDL_Renderer* renderer, SDL_Texture* tvide, SDL_Texture* tbombe, SDL_Texture* tdrapeau, SDL_Texture* tzero, 
    SDL_Texture* tun, SDL_Texture* tdeux, SDL_Texture* ttrois, SDL_Texture* tquatre,SDL_Texture* tcinq, SDL_Texture* tsix, SDL_Texture* tsept, SDL_Texture* thuit,
    SDL_Texture* animation[10])
{
    SDL_Texture* texture = NULL;
    SDL_Texture* anim = NULL;


    for (int i = 0; i < size; i++)
    {
        for (int j = 0; j < size; j++)
        {
            if (grid[i][j].see == 0)
            {
                texture = tvide;
            }

            else
            {
                int waf = 0;
                if (grid[i][j].isbomb == 1)
                {
                    texture = tbombe;
                    waf = 1;
                }

                if (grid[i][j].type == 3)
                {
                    texture = tdrapeau;
                    waf = 1;
                }

                if (waf == 0)
                {
                    if (grid[i][j].see == 1 && grid[i][j].event == 0)
                    {
                        grid[i][j].event = 1;
                        int imageCount = 0;
                        while (imageCount != 4)
                        {
                            anim = animation[imageCount];
                            SDL_Rect arectangle;
                            if (SDL_QueryTexture(anim, NULL, NULL, &arectangle.w, &arectangle.h) != 0)
                            {
                                void destruct(renderer, window);
                                exitError("Impossible de charger la texture : premier");
                            }
                            arectangle.x = j * 31;
                            arectangle.y = i * 31;
                            if (SDL_RenderCopy(renderer, anim, NULL, &arectangle) != 0)
                            {
                                void destruct(renderer, window);
                                exitError("Impossible de charger la texture : second");
                            }
                            /*LANCEMENT game*/

                            SDL_RenderPresent(renderer);
                            SDL_Delay(0015);
                            imageCount++;
                        }
                    }
                    if (grid[i][j].autour == 0)
                    {
                        texture = tzero;
                    }
                    if (grid[i][j].autour == 1)
                    {
                        texture = tun;
                    }
                    if (grid[i][j].autour == 2)
                    {
                        texture = tdeux;
                    }
                    if (grid[i][j].autour == 3)
                    {
                        texture = ttrois;
                    }
                    if (grid[i][j].autour == 4)
                    {
                        texture = tquatre;
                    }
                    if (grid[i][j].autour == 5)
                    {
                        texture = tcinq;
                    }
                    if (grid[i][j].autour == 6)
                    {
                        texture = tsix;
                    }
                    if (grid[i][j].autour == 7)
                    {
                        texture = tsept;
                    }
                    if (grid[i][j].autour == 8)
                    {
                        texture = thuit;
                    }

                }
            }

            SDL_Rect rectangle;
            if (SDL_QueryTexture(texture, NULL, NULL, &rectangle.w, &rectangle.h) != 0)
            {
                void destruct(renderer, window);
                exitError("Impossible de charger la texture : premier");
            }
            rectangle.x = j * 31;
            rectangle.y = i * 31;
            if (SDL_RenderCopy(renderer, texture, NULL, &rectangle) != 0)
            {
                void destruct(renderer, window);
                exitError("Impossible de charger la texture : second");
            }

        }
    }

    /*LANCEMENT game*/

    SDL_RenderPresent(renderer);

}


/* Contain game, from initialize to replay or game end */
void game()
{
    int nbrindice = 0;
    int premier = 1;
    int finish = 0;
    int drapeau = 0;
    int bombe = 0;
    int result = 0;

    menu(&bombe);

    Tile** grid = malloc(size * sizeof(Tile*));
    for (int t = 0; t < size; t++)
    {
        grid[t] = malloc(size * sizeof(Tile));
    }

    int caseLibres = size * size;

    //Jeu
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS | SDL_INIT_AUDIO) != 0)
    {
        exitError("Initialisation SDL");
    }
    SDL_Window* window = NULL;
    SDL_Renderer* renderer = NULL;


    if (SDL_CreateWindowAndRenderer(size * 31, size * 31, 0, &window, &renderer) != 0)
    {
        exitError("Initialisation fenetre");
    }

    modify(grid, bombe);

    /*INITIALISATION*/

    SDL_Surface* vide = NULL;
    SDL_Surface* sbombe = NULL;
    SDL_Surface* sdrapeau = NULL;
    SDL_Texture* zero = NULL;
    SDL_Surface* un = NULL;
    SDL_Surface* deux = NULL;
    SDL_Surface* trois = NULL;
    SDL_Surface* quatre = NULL;
    SDL_Surface* cinq = NULL;
    SDL_Surface* six = NULL;
    SDL_Surface* sept = NULL;
    SDL_Surface* huit = NULL;

    /*ANIMATION*/
    SDL_Surface* v1 = NULL;
    SDL_Surface* v2 = NULL;
    SDL_Surface* v3 = NULL;
    SDL_Surface* v4 = NULL;
    SDL_Surface* v5 = NULL;
    SDL_Surface* v6 = NULL;
    SDL_Surface* v7 = NULL;
    SDL_Surface* v8 = NULL;
    SDL_Surface* v9 = NULL;
    SDL_Surface* v10 = NULL;

    /*TEXTURE*/

    SDL_Texture* tvide = NULL;
    SDL_Texture* tbombe = NULL;
    SDL_Texture* tdrapeau = NULL;
    SDL_Texture* tzero = NULL;
    SDL_Texture* tun = NULL;
    SDL_Texture* tdeux = NULL;
    SDL_Texture* ttrois = NULL;
    SDL_Texture* tquatre = NULL;
    SDL_Texture* tcinq = NULL;
    SDL_Texture* tsix = NULL;
    SDL_Texture* tsept = NULL;
    SDL_Texture* thuit = NULL;


    /*ANIMATION*/
    SDL_Texture* animation[10];

    /*CHARGEMENT IMAGE*/

    vide = SDL_LoadBMP("ben10bmp/vide.bmp");
    if (vide == NULL)
    {
        destruct(renderer, window);
        exitError("Impossible de charger l'image : vide ");
    }

    sbombe = SDL_LoadBMP("ben10bmp/bombe.bmp");
    if (sbombe == NULL)
    {
        destruct(renderer, window);
        exitError("Impossible de charger l'image : sbombe ");
    }

    sdrapeau = SDL_LoadBMP("ben10bmp/drapeau.bmp");
    if (sdrapeau == NULL)
    {
        destruct(renderer, window);
        exitError("Impossible de charger l'image : sdrapeau ");
    }

    zero = SDL_LoadBMP("ben10bmp/0.bmp");
    if (zero == NULL)
    {
        destruct(renderer, window);
        exitError("Impossible de charger l'image : zero ");
    }

    un = SDL_LoadBMP("ben10bmp/1.bmp");
    if (un == NULL)
    {
        destruct(renderer, window);
        exitError("Impossible de charger l'image : un ");
    }

    deux = SDL_LoadBMP("ben10bmp/2.bmp");
    if (deux == NULL)
    {
        destruct(renderer, window);
        exitError("Impossible de charger l'image : deux ");
    }

    trois = SDL_LoadBMP("ben10bmp/3.bmp");
    if (trois == NULL)
    {
        destruct(renderer, window);
        exitError("Impossible de charger l'image : trois ");
    }

    quatre = SDL_LoadBMP("ben10bmp/4.bmp");
    if (quatre == NULL)
    {
        destruct(renderer, window);
        exitError("Impossible de charger l'image : quatre ");
    }

    cinq = SDL_LoadBMP("ben10bmp/5.bmp");
    if (cinq == NULL)
    {
        destruct(renderer, window);
        exitError("Impossible de charger l'image : cinq ");
    }

    six = SDL_LoadBMP("ben10bmp/6.bmp");
    if (six == NULL)
    {
        destruct(renderer, window);
        exitError("Impossible de charger l'image : six ");
    }

    sept = SDL_LoadBMP("ben10bmp/7.bmp");
    if (sept == NULL)
    {
        destruct(renderer, window);
        exitError("Impossible de charger l'image : sept ");

    }

    huit = SDL_LoadBMP("ben10bmp/8.bmp");
    if (huit == NULL)
    {
        destruct(renderer, window);
        exitError("Impossible de charger l'image : huit ");
    }


    /*ANIMATION*/
    v1 = SDL_LoadBMP("ben10bmp/v1.bmp");
    if (v1 == NULL)
    {
        destruct(renderer, window);
        exitError("Impossible de charger l'image : v1 ");
    }

    v2 = SDL_LoadBMP("ben10bmp/v2.bmp");
    if (v2 == NULL)
    {
        destruct(renderer, window);
        exitError("Impossible de charger l'image : v2 ");
    }

    v3 = SDL_LoadBMP("ben10bmp/v3.bmp");
    if (v3 == NULL)
    {
        destruct(renderer, window);
        exitError("Impossible de charger l'image : v3 ");

    }

    v4 = SDL_LoadBMP("ben10bmp/v4.bmp");
    if (v4 == NULL)
    {
        destruct(renderer, window);
        exitError("Impossible de charger l'image : v4 ");
    }
    v5 = SDL_LoadBMP("ben10bmp/v5.bmp");
    if (v5 == NULL)
    {
        destruct(renderer, window);
        exitError("Impossible de charger l'image : v5 ");
    }
    v6 = SDL_LoadBMP("ben10bmp/v6.bmp");
    if (v6 == NULL)
    {
        destruct(renderer, window);
        exitError("Impossible de charger l'image : v6 ");
    }
    v7 = SDL_LoadBMP("ben10bmp/v7.bmp");
    if (v7 == NULL)
    {
        destruct(renderer, window);
        exitError("Impossible de charger l'image : v7 ");
    }
    v8 = SDL_LoadBMP("ben10bmp/v8.bmp");
    if (v8 == NULL)
    {
        destruct(renderer, window);
        exitError("Impossible de charger l'image : v8 ");
    }
    v9 = SDL_LoadBMP("ben10bmp/v9.bmp");
    if (v9 == NULL)
    {
        destruct(renderer, window);
        exitError("Impossible de charger l'image : v9 ");
    }
    v10 = SDL_LoadBMP("ben10bmp/v10.bmp");
    if (v10 == NULL)
    {
        destruct(renderer, window);
        exitError("Impossible de charger l'image : v10 ");
    }
    tvide = SDL_CreateTextureFromSurface(renderer, vide);
    SDL_FreeSurface(vide);
    if (tvide == NULL)
    {
        destruct(renderer, window);
        exitError("Impossible de charger la texture : vide");
    }

    tbombe = SDL_CreateTextureFromSurface(renderer, sbombe);
    SDL_FreeSurface(sbombe);
    if (tbombe == NULL)
    {
        destruct(renderer, window);
        exitError("Impossible de charger la texture : bombe");
    }

    tdrapeau = SDL_CreateTextureFromSurface(renderer, sdrapeau);
    SDL_FreeSurface(sdrapeau);
    if (tdrapeau == NULL)
    {
        destruct(renderer, window);
        exitError("Impossible de charger la texture : drapeau");
    }

    tzero = SDL_CreateTextureFromSurface(renderer, zero);
    SDL_FreeSurface(zero);
    if (tzero == NULL)
    {
        destruct(renderer, window);
        exitError("Impossible de charger la texture : zero");
    }

    tun = SDL_CreateTextureFromSurface(renderer, un);
    SDL_FreeSurface(un);
    if (tun == NULL)
    {
        destruct(renderer, window);
        exitError("Impossible de charger la texture : un");
    }

    tdeux = SDL_CreateTextureFromSurface(renderer, deux);
    SDL_FreeSurface(deux);
    if (tdeux == NULL)
    {
        destruct(renderer, window);
        exitError("Impossible de charger la texture : deux");
    }

    ttrois = SDL_CreateTextureFromSurface(renderer, trois);
    SDL_FreeSurface(trois);
    if (ttrois == NULL)
    {
        destruct(renderer, window);
        exitError("Impossible de charger la texture : trois");
    }

    tquatre = SDL_CreateTextureFromSurface(renderer, quatre);
    SDL_FreeSurface(quatre);
    if (tquatre == NULL)
    {
        destruct(renderer, window);
        exitError("Impossible de charger la texture : quatre");
    }

    tcinq = SDL_CreateTextureFromSurface(renderer, cinq);
    SDL_FreeSurface(cinq);
    if (tcinq == NULL)
    {
        destruct(renderer, window);
        exitError("Impossible de charger la texture : cinq");
    }

    tsix = SDL_CreateTextureFromSurface(renderer, six);
    SDL_FreeSurface(six);
    if (tsix == NULL)
    {
        destruct(renderer, window);
        exitError("Impossible de charger la texture : six");
    }

    tsept = SDL_CreateTextureFromSurface(renderer, sept);
    SDL_FreeSurface(sept);
    if (tsept == NULL)
    {
        destruct(renderer, window);
        exitError("Impossible de charger la texture : sept");
    }

    thuit = SDL_CreateTextureFromSurface(renderer, huit);
    SDL_FreeSurface(huit);
    if (thuit == NULL)
    {
        destruct(renderer, window);
        exitError("Impossible de charger la texture : huit");
    }

    /*ANIMATION*/
    animation[0] = SDL_CreateTextureFromSurface(renderer, v1);
    SDL_FreeSurface(v1);
    animation[1] = SDL_CreateTextureFromSurface(renderer, v2);
    SDL_FreeSurface(v2);
    animation[2] = SDL_CreateTextureFromSurface(renderer, v3);
    SDL_FreeSurface(v3);
    animation[3] = SDL_CreateTextureFromSurface(renderer, v4);
    SDL_FreeSurface(v4);
    animation[4] = SDL_CreateTextureFromSurface(renderer, v5);
    SDL_FreeSurface(v5);
    animation[5] = SDL_CreateTextureFromSurface(renderer, v6);
    SDL_FreeSurface(v6);
    animation[6] = SDL_CreateTextureFromSurface(renderer, v7);
    SDL_FreeSurface(v7);
    animation[7] = SDL_CreateTextureFromSurface(renderer, v8);
    SDL_FreeSurface(v8);
    animation[8] = SDL_CreateTextureFromSurface(renderer, v9);
    SDL_FreeSurface(v9);
    animation[9] = SDL_CreateTextureFromSurface(renderer, v10);
    SDL_FreeSurface(v10);

    Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, MIX_DEFAULT_CHANNELS, 1024);
    Mix_Music* musicBCK;
    musicBCK = Mix_LoadMUS("music/mainTheme.mp3");
    Mix_PlayMusic(musicBCK, -1);

    while (caseLibres > 0)
    {

        show(grid, finish, window, renderer, tvide, tbombe, tdrapeau, tzero, tun, tdeux, ttrois, tquatre, tcinq, tsix, tsept, thuit, animation);
        play(grid, &finish, &caseLibres, bombe,&result , &drapeau, &nbrindice, window, renderer);
        if (finish == 1) {
            caseLibres = 0;
        }
    }
    show(grid, finish, window, renderer, tvide, tbombe, tdrapeau, tzero, tun, tdeux, ttrois, tquatre, tcinq, tsix, tsept, thuit, animation);
    SDL_Delay(0150);

    SDL_Window* rwindow = NULL;
    SDL_Renderer* rrenderer = NULL;


    if (SDL_CreateWindowAndRenderer(100, 100, 0, &rwindow, &rrenderer) != 0)
    {
        exitError("Initialisation fenetre");
    }
    if (result == 1)
    {
        SDL_SetRenderDrawColor(rrenderer, 0, 255, 0, 255);
    }
    else
    {
        SDL_SetRenderDrawColor(rrenderer, 255, 0, 0, 255);
    }
    SDL_RenderClear(rrenderer);
    SDL_RenderPresent(rrenderer);
    SDL_Delay(1000);

    SDL_DestroyRenderer(rrenderer);
    SDL_DestroyWindow(rwindow);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    Mix_CloseAudio();
    SDL_QUIT;

    //restart

    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS) != 0)
    {
        exitError("Initialisation SDL");
    }

    //Menu
    SDL_Window* restartwindow = NULL;
    SDL_Renderer* restartrenderer = NULL;
    int y = 0;
    int True = 1;

    SDL_Rect yes = { 25 , 70 , 50 , 50 };
    SDL_Rect no = { 125 , 70 , 50 , 50 };


    if (SDL_CreateWindowAndRenderer(200, 200, 0, &restartwindow, &restartrenderer) != 0)
    {
        exitError("Initialisation fenetre");
    }
    SDL_Event event;
    while (True == 1) {

        SDL_SetRenderDrawColor(restartrenderer, 0, 0, 0, 255);
        SDL_RenderClear(restartrenderer);
        SDL_SetRenderDrawColor(restartrenderer, 0, 255, 0, 255);
        SDL_RenderFillRect(restartrenderer, &yes);
        SDL_SetRenderDrawColor(restartrenderer, 255, 0, 0, 255);
        SDL_RenderFillRect(restartrenderer, &no);


        while (SDL_PollEvent(&event))
        {
            switch (event.type) {
            case SDL_QUIT:
                SDL_DestroyWindow(restartwindow);
                SDL_Quit();
            case SDL_MOUSEBUTTONDOWN:
                if (event.button.button == SDL_BUTTON_LEFT)
                {
                    int y = event.button.y;
                    int x = event.button.x;
                    if ((y > 70 && y < 120) && (x > 25 && x < 75))
                    {
                        for (int t = 0; t < size; t++)
                        {
                            free(grid[t]);
                        }
                        free(grid);
                        SDL_DestroyRenderer(restartrenderer);
                        SDL_DestroyWindow(restartwindow);
                        SDL_Quit;
                        game();
                        True = 0;
                    }
                    else if ((y > 70 && y < 120) && (x > 125 && x < 175))
                    {
                        for (int t = 0; t < size; t++)
                        {
                            free(grid[t]);
                        }
                        free(grid);
                        True = 0;
                    }

                }
            }
        }


        SDL_RenderPresent(restartrenderer);
    }
    SDL_DestroyRenderer(restartrenderer);
    SDL_DestroyWindow(restartwindow);
    SDL_Quit;

}

/* Contain and show the solution grid in the command lines */
void ninvisible(Tile** grid)
{
    printf(" %c |", 3);
    for (int a = 0; a < size; a++)
    {
        if (a < 10)
        {
            printf("%2d ", a + 1);
        }
        else if (a >= 10)
        {
            printf("%d ", a + 1);
        }
    }
    printf("\n");

    printf("___|");
    for (int a = 0; a < size; a++)
        printf("___");
    printf("\n");


    for (int i = 0; i < size; i++)
    {
        if (i < 9)
            printf(" %d |", i + 1);
        else
            printf("%d |", i + 1);
        for (int j = 0; j < size; j++)
        {

            if (grid[i][j].isbomb == 1 || grid[i][j].type == 3) {
                printf(" %c ", grid[i][j].autour);
            }
            else {
                printf("%2d ", grid[i][j].autour);
            }

        }
        printf("\n");
    }
}