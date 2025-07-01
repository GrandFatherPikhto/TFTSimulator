#include <SDL2/SDL.h>
#include <stdbool.h>

static SDL_Window* window;
static SDL_Renderer* renderer;

void init_graphics(int w, int h) {
    SDL_Init(SDL_INIT_VIDEO);
    window = SDL_CreateWindow("Display Emulator", 
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        w*2, h*2, SDL_WINDOW_SHOWN);
    renderer = SDL_CreateRenderer(window, -1, 
        SDL_RENDERER_ACCELERATED);
    SDL_RenderSetLogicalSize(renderer, w, h);
}

void render_frame() {
    SDL_RenderPresent(renderer);
    SDL_Delay(16); // ~60 FPS
}
/*
void draw_line(SDL_Renderer *renderer, uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1) {

}
*/

void simulate_display() {
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        printf("SDL_Init Error: %s\n", SDL_GetError());
        return;
    }

    SDL_Window* window = SDL_CreateWindow("Эмулятор", 
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 
        320, 240, 
        SDL_WINDOW_SHOWN);
    
    if (!window) {
        printf("SDL_CreateWindow Error: %s\n", SDL_GetError());
        SDL_Quit();
        return;
    }

    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, 
        SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    
    if (!renderer) {
        printf("SDL_CreateRenderer Error: %s\n", SDL_GetError());
        SDL_DestroyWindow(window);
        SDL_Quit();
        return;
    }

    bool running = true;
    while (running) {
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                running = false;
            }
            else if (event.type == SDL_KEYDOWN) {
                if (event.key.keysym.sym == SDLK_ESCAPE) {
                    running = false;
                }
            }
        }

        // Очистка экрана
        SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 255);
        SDL_RenderClear(renderer);
        
        // Тестируемая графика
        SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
        SDL_RenderDrawLine(renderer, 10, 10, 100, 50);
        
        // Обновление экрана
        SDL_RenderPresent(renderer);
    }

    // Корректное завершение
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

int main(/* int argc, char *argv[], char **penv */) {
    simulate_display();
    return 0;
}