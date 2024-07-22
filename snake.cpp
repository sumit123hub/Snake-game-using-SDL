#include "inc/SDL.h"
#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#undef main
const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 600;
const int SNAKE_SIZE = 20;
const int FOOD_SIZE = 20;
int a;
int highscore;
struct Point
{
    int x, y;
};
bool init(SDL_Window *&window, SDL_Renderer *&renderer)
{
    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        std::cerr << "SDL could not initialize! SDL_Error: " << SDL_GetError() << std::endl;
        return false;
    }
    window = SDL_CreateWindow("Snake Game", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    if (!window)
    {
        std::cerr << "Window could not be created! SDL_Error: " << SDL_GetError() << std::endl;
        return false;
    }
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (!renderer)
    {
        std::cerr << "Renderer could not be created! SDL_Error: " << SDL_GetError() << std::endl;
        return false;
    }
    return true;
}
std::vector<Point> snake;
Point food;
Point direction = {1, 0}; // Start moving to the right
bool gameOver = false;
Point newHead;

void initGame()
{
    snake.push_back({SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2});
    srand(time(0));
    food = {rand() % (SCREEN_WIDTH / FOOD_SIZE) * FOOD_SIZE, rand() % (SCREEN_HEIGHT / FOOD_SIZE) * FOOD_SIZE};
}
void handleEvents(bool &quit)
{
    SDL_Event e;
    while (SDL_PollEvent(&e) != 0)
    {
        if (e.type == SDL_QUIT)
        {
            quit = true;
        }
        else if (e.type == SDL_KEYDOWN)
        {
            switch (e.key.keysym.sym)
            {
            case SDLK_UP:
                direction = {0, -1};
                break;
            case SDLK_DOWN:
                direction = {0, 1};
                break;
            case SDLK_LEFT:
                direction = {-1, 0};
                break;
            case SDLK_RIGHT:
                direction = {1, 0};
                break;
            }
        }
    }
}
void updateGame()
{
    Point newHead = {snake.front().x + direction.x * SNAKE_SIZE, snake.front().y + direction.y * SNAKE_SIZE};

    if (newHead.x < 0 || newHead.x >= SCREEN_WIDTH || newHead.y < 0 || newHead.y >= SCREEN_HEIGHT)
    {
        if (newHead.x < 0)
        { 
            for (size_t i = 0; i < snake.size(); i++)
            {
                const auto &segment = snake[i];
                snake[i].x = segment.x + SCREEN_WIDTH;
                newHead.x = snake[0].x-20;
                snake[i].x = snake[i].x -SCREEN_WIDTH;
                snake[i].x = segment.x + SCREEN_WIDTH+20;

            }
            
        }
        else if (newHead.x >= SCREEN_WIDTH)
        {
          for (size_t i = 0; i < snake.size(); i++)
            {
                const auto &segment = snake[i];
                snake[i].x = segment.x - SCREEN_WIDTH;
                newHead.x = snake[0].x +20;
                snake[i].x = snake[i].x +SCREEN_WIDTH;
                snake[i].x = segment.x -SCREEN_WIDTH-20;

            }  
        }
        else if (newHead.y <0)
        {
          for (size_t i = 0; i < snake.size(); i++)
            {
                const auto &segment = snake[i];
                snake[i].y = segment.y + SCREEN_HEIGHT;
                newHead.y = snake[0].y -20;
                snake[i].y = snake[i].y -SCREEN_HEIGHT;
                snake[i].y = segment.y +SCREEN_HEIGHT+20;

            }  
        }
        else if (newHead.y >= SCREEN_HEIGHT)
        {
          for (size_t i = 0; i < snake.size(); i++)
            {
                const auto &segment = snake[i];
                snake[i].y = segment.y - SCREEN_HEIGHT;
                newHead.y = snake[0].y +20;
                snake[i].y= snake[i].y +SCREEN_HEIGHT;
                snake[i].y = segment.y -SCREEN_HEIGHT-20;

            }  
        }
    }

    for (const auto &segment : snake)
    {
        if (newHead.x == segment.x && newHead.y == segment.y)
        {
            gameOver = true;
            printf("Game Over\n");
            return;
        }
    }

    snake.insert(snake.begin(), newHead);

    if (newHead.x == food.x && newHead.y == food.y)
    {
        food = {rand() % (SCREEN_WIDTH / FOOD_SIZE) * FOOD_SIZE, rand() % (SCREEN_HEIGHT / FOOD_SIZE) * FOOD_SIZE};
        a++;
        highscore++;
        if (a > 1)
        {
            a = 0;
        }
    }
    else
    {
        snake.pop_back();
    }
}
void renderGame(SDL_Renderer *renderer)
{
    SDL_SetRenderDrawColor(renderer, 221, 221, 221, 255);
    SDL_RenderClear(renderer);

    SDL_SetRenderDrawColor(renderer, 0, 54, 99, 255);
    SDL_Rect foodRect = {food.x, food.y, FOOD_SIZE, FOOD_SIZE};
    SDL_RenderFillRect(renderer, &foodRect);

    if (a == 0)
    {
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        for (size_t i = 1; i < snake.size(); i++)
        {
            const auto &segment = snake[i];
            SDL_Rect fillRect = {segment.x, segment.y, SNAKE_SIZE, SNAKE_SIZE};
            SDL_RenderFillRect(renderer, &fillRect);
        }
        SDL_SetRenderDrawColor(renderer, 255, 102, 51, 255);
        Point head = snake.front();
        SDL_Rect foodRect2 = {head.x - 2, head.y - 2, 25, 25};
        SDL_RenderFillRect(renderer, &foodRect2);
    }
    else if (a == 1)
    {
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        for (size_t i = 1; i < snake.size(); i++)
        {
            const auto &segment = snake[i];
            SDL_Rect fillRect = {segment.x, segment.y, SNAKE_SIZE, SNAKE_SIZE};
            SDL_RenderFillRect(renderer, &fillRect);
        }
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        Point head = snake.front();
        SDL_Rect foodRect2 = {head.x - 2, head.y - 2, 25, 25};
        SDL_RenderFillRect(renderer, &foodRect2);
    }

    SDL_RenderPresent(renderer);
}
int main(int argc, char *args[])
{
    SDL_Window *window = NULL;
    SDL_Renderer *renderer = NULL;

    if (!init(window, renderer))
    {
        std::cerr << "Failed to initialize!" << std::endl;
        return -1;
    }

    initGame();

    bool quit = false;

    while (!quit && !gameOver)
    {
        handleEvents(quit);
        updateGame();
        renderGame(renderer);
        SDL_Delay(100); // Control the game speed
    }
    printf("Score: %d", highscore);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
