/**
 * @file main.cpp
 * @author Amin Khodabande (whfk233@gmail.com)
 * @brief Conway's game of life
 * @version 1.0
 * @date 2024-05-28
 * 
 * @copyright Copyright (c) 2024
 * 
 */

/*
    There are 4 rules in the universe of game of life

    1. Any live cell with fewer than two live neighbors dies, as if by underpopulation.
    2. Any live cell with two or three live neighbors lives on to the next generation.
    3. Any live cell with more than three live neighbors dies, as if by overpopulation.
    4. Any dead cell with exactly three live neighbors becomes a live cell, as if by reproduction.
*/

#include <SDL2/SDL.h>
#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

const int GAME_WIDTH = 640;
const int GAME_HEIGHT = 480;
const int CELL_SIZE = 5;


// This function checks the surroundings of a cell and apply the game of life rules 
// on the cell
bool isAlive (array<array<int, GAME_HEIGHT>, GAME_WIDTH>& game, const int x, const int y) {
    int alive = 0;

    // Check left
    if (x > 0 && game[x-1][y] == 1) alive += 1;
    // Check right
    if (x < GAME_WIDTH && game[x+1][y] == 1) alive += 1;
    // Check top
    if (y > 0 && game[x][y-1] == 1) alive += 1;
    // Check bottom
    if (x < GAME_WIDTH && game[x][y+1] == 1) alive += 1;

    // Check top left
    if (y > 0 && x > 0 && game[x-1][y-1] == 1) alive += 1;
    // Check bottom left
    if (y < GAME_HEIGHT && x > 0 && game[x-1][y+1]) alive += 1;
    // Check top right
    if (y > 0 && x < GAME_WIDTH && game[x+1][y-1]) alive += 1;
    // CHECK bottom right
    if (y < GAME_HEIGHT && x < GAME_WIDTH && game[x+1][y+1]) alive += 1;


    // Applying rule number one
    if (game[x][y] == 1 && alive < 2) return false;
    // Applying rule number two
    if (game[x][y] == 1 && (alive == 2 || alive == 3)) return true;
    // Applying rule number three
    if (alive > 3) return false;
    // Applying rule number four
    if (alive == 3) return true;

    return false;
}

int main() {

    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        cerr << "SDL could not initialize! SDL_Error: " << SDL_GetError() << endl;
        return 1;
    }

    SDL_Window* window = SDL_CreateWindow(
        "Conway's Game of Life",
        SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
        GAME_WIDTH, GAME_HEIGHT, SDL_WINDOW_SHOWN
    );

    if (!window) {
        cerr << "Window could not be created! SDL_Error: " << SDL_GetError() << endl;
        return 1;
    }

    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (!renderer) {
        cerr << "Renderer could not be created! SDL_Error: " << SDL_GetError() << endl;
        return 1;
    }

    array<array<int, GAME_HEIGHT>, GAME_WIDTH> display {};
    array<array<int, GAME_HEIGHT>, GAME_WIDTH> swap {};

    // Create random alive cells
    for (auto& row : display) {
        generate(row.begin(), row.end(), []() { return rand() % 10 == 0 ? 1 : 0; });
    }

    SDL_Event e;

    // Store the next generation to swap
    while (true) {
        for (int i = 0; i < GAME_WIDTH; ++i) {
            for (int j = 0; j < GAME_HEIGHT; ++j) {
                swap[i][j] = isAlive(display, i, j) ? 1 : 0;
            }
        }


        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        // Draw cells to the screen
        for (int i = 0; i < GAME_WIDTH; ++i) {
            for (int j = 0; j < GAME_HEIGHT; ++j) {
                if (swap[i][j]) {
                    if (swap[i][j]) {
                        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
                        SDL_Rect cell = { i * CELL_SIZE, j * CELL_SIZE, CELL_SIZE, CELL_SIZE };
                        SDL_RenderFillRect(renderer, &cell);
                    }
                }
            }
        }

        SDL_RenderPresent(renderer);

        // Copy swap to the screen
        copy(swap.begin(), swap.end(), display.begin());
    }
        
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Delay(20);
    SDL_Quit();

    return 0;
}
