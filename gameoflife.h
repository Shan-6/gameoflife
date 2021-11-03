#ifndef GAMEOFLIFE_H
#define GAMEOFLIFE_H

#include <iostream>
#include <unistd.h>
#include "SDL2/SDL.h"

class Gameoflife
{
private:
    int **matrix;
    int **nxtmatrix;
    int **ngbmatrix;
    int size;
    SDL_Window *window;
    SDL_Renderer *renderer;
    
   
    void draw();
    void compute();
    void clear_board();
public:
    void run();
    Gameoflife(int size);
     ~Gameoflife();
};

Gameoflife::Gameoflife(int size)
{
    SDL_Init(SDL_INIT_VIDEO);
    SDL_CreateWindowAndRenderer(((size+1)*10), ((size+1)*10), 0, &window, &renderer);

    this->size = size;
    matrix = new int *[size]; //resizing game matrix
    nxtmatrix = new int *[size];
    ngbmatrix = new int *[size];
    for (int i = 0; i < size; ++i)
    {
        matrix[i] = new int[size];
        nxtmatrix[i] = new int[size];
        ngbmatrix[i] = new int[size];
        for (int j = 0; j < size; j++)
        {
            matrix[i][j] = 0;
            nxtmatrix[i][j] = 0;
            ngbmatrix[i][j] = 0;
        }
    }
}

Gameoflife::~Gameoflife()
{
    for (int i = 0; i < size; ++i)
        delete[] matrix[i];

    delete[] matrix;

    SDL_DestroyWindow(window);
    SDL_Quit();
}
    



void Gameoflife::draw()
{

    SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
    SDL_RenderClear(renderer);
    SDL_SetRenderDrawColor(renderer, 128, 128, 128, SDL_ALPHA_OPAQUE);
    for (int i = 1; i <= size; i++)
    {
        SDL_RenderDrawLine(renderer, 10 * i, 10, 10 * i, 10 * size);
        SDL_RenderDrawLine(renderer, 10, 10 * i, 10 * size, 10 * i);
    }



    

    SDL_SetRenderDrawColor(renderer, 255, 255, 255, SDL_ALPHA_OPAQUE);
    for (int i = 0; i < size; i++)
    {
        for (int j = 0; j < size; j++)
        {
           // SDL_SetRenderDrawColor(renderer,rand()%(256),rand()%(256),rand()%(256), SDL_ALPHA_OPAQUE); //COLORS !!!
            if (matrix[i][j] == 1){
            SDL_Rect r;
            r.x = 10 + (i * 10);
            r.y = 10 + (j * 10);
            r.w = 10;
            r.h = 10;
            
                SDL_RenderFillRect(renderer, &r);}
        }
    }
    SDL_RenderPresent(renderer);
    
}

void Gameoflife::compute()
{

    int matrix2[size + 2][size + 2];

    for (int i = 0; i < size + 2; i++)
    {
        for (int j = 0; j < size + 2; j++)
        {
            matrix2[i][j] = 0;
        }
    }

    for (int i = 1; i < size + 1; i++)
    {
        for (int j = 1; j < size + 1; j++)
        {
            matrix2[i][j] = matrix[i - 1][j - 1];
        }
    }

    int c = 0;
    for (int x = 1; x < size + 1; x++)
    {

        for (int y = 1; y < size + 1; y++)
        {
            c = 0;
            for (int i = x - 1; i < x + 2; i++)
            {
                for (int j = y - 1; j < y + 2; j++)
                {
                    if (matrix2[i][j] == 1 && !(i == x && j == y))
                        c++;
                }
            }
            ngbmatrix[x - 1][y - 1] = c;
        }
    }

    for (int i = 0; i < size; i++)
    {
        for (int j = 0; j < size; j++)
        {
            if (ngbmatrix[i][j] < 2 && matrix[i][j] == 1)
                nxtmatrix[i][j] = 0;
            else if ((ngbmatrix[i][j] == 3 || ngbmatrix[i][j] == 2) && matrix[i][j] == 1)
                nxtmatrix[i][j] = 1;
            else if (ngbmatrix[i][j] > 3 && matrix[i][j] == 1)
                nxtmatrix[i][j] = 0;
            else if (ngbmatrix[i][j] == 3 && matrix[i][j] == 0)
                nxtmatrix[i][j] = 1;
            else
                nxtmatrix[i][j] = 0;
        }
    }

   // std::cout << "nxtmatrix : "<<std::endl;
    for (int i = 0; i < size; i++)
    {
        for (int j = 0; j < size; j++)
        {
            matrix[i][j] = nxtmatrix[i][j];

            //std::cout << nxtmatrix[i][j];
        }
        //std::cout << std::endl;
    }
    // std::cout << std::endl;
    //  std::cout << "nxtmatrix : "<<std::endl;
    // std::cout << std::endl;
}

void Gameoflife::run()
{

    if (window == NULL)
    {
        std::cout << "Could not create window: " << SDL_GetError() << std::endl;
        exit(1);
    }

    SDL_Event e;
    bool quit = false;
    bool pause = true;
    int gen = 0;
    while (!quit)
    {
        if (!pause)compute();
            

       SDL_PollEvent(&e);
        
            if (e.key.keysym.sym == SDLK_q)
                quit = true;

            if (e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_SPACE)
            {
                pause = !pause;
               // std::cout << "PAUSE/PLAY  " << std::endl;
            }

            if (e.type == SDL_MOUSEBUTTONDOWN)
            {
                //std::cout << " x : " << ((e.motion.x - 10) / 10) << " y : " << ((e.motion.y - 10) / 10) << std::endl;
                matrix[((e.motion.x - 10) / 10)][((e.motion.y - 10) / 10)] = 1;
            }


            draw();
            
        

        usleep(100000);
     //   std::cout << "pause : " << pause << " gen : " << gen++ << std::endl;
    }
}

#endif